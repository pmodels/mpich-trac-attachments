/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>

#include <mpi.h>
#include "mcs-mutex.h"

/* TODO: Make these mutex operations no-ops for sequential runs */

/** Create an MCS mutex.  Collective on comm.
  *
  * @param[out] comm communicator containing all processes that will use the
  *                  mutex
  * @param[out] tail_rank rank of the process in comm that holds the tail
  *                  pointer
  * @param[out] hdl  handle to the mutex
  * @return          MPI status
  */
int MCS_Mutex_create(int tail_rank, MPI_Comm comm, MCS_Mutex * hdl_out)
{
    int rank, nproc;
    MCS_Mutex hdl;

    hdl = malloc(sizeof(struct mcs_mutex_s));
    assert(hdl != NULL);

    MPI_Comm_dup(comm, &hdl->comm);

    MPI_Comm_rank(hdl->comm, &rank);
    MPI_Comm_size(hdl->comm, &nproc);

    hdl->tail_rank = tail_rank;

    MPI_Alloc_mem(2*sizeof(int), MPI_INFO_NULL, &hdl->base);
    assert(hdl->base != NULL);

    hdl->base[0] = -1;
    hdl->base[1] = -1;

    MPI_Win_create(hdl->base, 2*sizeof(int), sizeof(int), MPI_INFO_NULL,
                   hdl->comm, &hdl->window);

    MPI_Win_lock_all(0, hdl->window);

    *hdl_out = hdl;
    return MPI_SUCCESS;
}


/** Free an MCS mutex.  Collective on ranks in the communicator used at the
  * time of creation.
  *
  * @param[in] hdl handle to the group that will be freed
  * @return        MPI status
  */
int MCS_Mutex_free(MCS_Mutex * hdl_ptr)
{
    MCS_Mutex hdl = *hdl_ptr;

    MPI_Win_unlock_all(hdl->window);

    MPI_Win_free(&hdl->window);
    MPI_Free_mem(hdl->base);
    MPI_Comm_free(&hdl->comm);

    free(hdl);
    hdl_ptr = NULL;

    return MPI_SUCCESS;
}


/** Lock a mutex.
  *
  * @param[in] hdl   Handle to the mutex
  * @return          MPI status
  */
int MCS_Mutex_lock(MCS_Mutex hdl)
{
    int rank, nproc;
    int prev;

    MPI_Comm_rank(hdl->comm, &rank);
    MPI_Comm_size(hdl->comm, &nproc);

    /* This store is safe, since it cannot happen concurrently with a remote
     * write */
    hdl->base[MCS_MTX_ELEM_DISP] = -1;
    MPI_Win_sync(hdl->window);

    MPI_Fetch_and_op(&rank, &prev, MPI_INT, hdl->tail_rank, MCS_MTX_TAIL_DISP,
                     MPI_REPLACE, hdl->window);
    MPI_Win_flush(hdl->tail_rank, hdl->window);

    /* If there was a previous tail, update their next pointer and wait for
     * notification.  Otherwise, the mutex was successfully acquired. */
    if (prev != -1) {
        /* Wait for notification */
        MPI_Status status;

        MPI_Accumulate(&rank, 1, MPI_INT, prev, MCS_MTX_ELEM_DISP, 1, MPI_INT, MPI_REPLACE, hdl->window);
        MPI_Win_flush(prev, hdl->window);

        debug_print("%2d: LOCK   - waiting for notification from %d\n", rank, prev);
        MPI_Recv(NULL, 0, MPI_BYTE, prev, MPI_MUTEX_TAG, hdl->comm, &status);
    }

    debug_print("%2d: LOCK   - lock acquired\n", rank);

    return MPI_SUCCESS;
}


/** Attempt to acquire a mutex.
  *
  * @param[in] hdl   Handle to the mutex
  * @param[out] success Indicates whether the mutex was acquired
  * @return          MPI status
  */
int MCS_Mutex_trylock(MCS_Mutex hdl, int *success)
{
    int rank, nproc;
    int tail, nil = -1;

    MPI_Comm_rank(hdl->comm, &rank);
    MPI_Comm_size(hdl->comm, &nproc);

    /* Check if the lock is available and claim it if it is. */
    MPI_Compare_and_swap(&rank, &nil, &tail, MPI_INT, hdl->tail_rank,
                         MCS_MTX_TAIL_DISP, hdl->window);
    MPI_Win_flush(hdl->tail_rank, hdl->window);

    /* If the old tail was -1, we have claimed the mutex */
    *success = (tail == nil);

    debug_print("%2d: TRYLOCK - %s\n", rank, (*success) ? "Success" : "Non-success");

    return MPI_SUCCESS;
}


/** Unlock a mutex.
  *
  * @param[in] hdl   Handle to the mutex
  * @return          MPI status
  */
int MCS_Mutex_unlock(MCS_Mutex hdl)
{
    int rank, nproc, next;

    MPI_Comm_rank(hdl->comm, &rank);
    MPI_Comm_size(hdl->comm, &nproc);

    MPI_Win_sync(hdl->window);

    /* Read my next pointer.  FOP is used since another process may write to
     * this location concurrent with this read. */
    MPI_Fetch_and_op(NULL, &next, MPI_INT, rank, MCS_MTX_ELEM_DISP, MPI_NO_OP,
                     hdl->window);

    if ( next == -1) {
        int tail;
        int nil = -1;

        /* Check if we are the at the tail of the lock queue.  If so, we're
         * done.  If not, we need to send notification. */
        MPI_Compare_and_swap(&nil, &rank, &tail, MPI_INT, hdl->tail_rank,
                             MCS_MTX_TAIL_DISP, hdl->window);
        MPI_Win_flush(hdl->tail_rank, hdl->window);

        if (tail != rank) {
            debug_print("%2d: UNLOCK - waiting for next pointer (tail = %d)\n", rank, tail);

            for (;;) {
                int flag;

                MPI_Fetch_and_op(NULL, &next, MPI_INT, rank, MCS_MTX_ELEM_DISP,
                                 MPI_NO_OP, hdl->window);
                if (next != -1) break;

                MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag,
                           MPI_STATUS_IGNORE);
            }
        }
    }

    /* Notify the next waiting process */
    if (next != -1) {
        debug_print("%2d: UNLOCK - notifying %d\n", rank, next);
        MPI_Send(NULL, 0, MPI_BYTE, next, MPI_MUTEX_TAG, hdl->comm);
    }

    debug_print("%2d: UNLOCK - lock released\n", rank);

    return MPI_SUCCESS;
}
