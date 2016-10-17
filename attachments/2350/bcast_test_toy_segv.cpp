/*
 * Build with something like:
 *   mpic++ -ggdb -fno-inline -O0 -fno-omit-frame-pointer -std=c++11 -o bcast_test bcast_test_toy_segv.cpp
 * 
 * ####################################
 * Run with something like:
 *   mpirun -np 6 bcast_test 400 70
 */

#include "assert.h"
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <mpi.h>
#include <string>

enum class PayloadValueMode { POSITIONAL, SMALL, TINY };

template <typename PayloadType>
inline PayloadType payload_value(int iteration, int sender_rank, int position,
                                 PayloadValueMode mode = PayloadValueMode::POSITIONAL) noexcept {

    PayloadType ret_val;
    if (std::is_floating_point<PayloadType>::value) {
        ret_val = (((iteration) + 1) * 1.00000001 + (sender_rank)*1.001001 + (position)*1.000001);
    } else if (std::is_integral<PayloadType>::value) {

        if (mode == PayloadValueMode::POSITIONAL) {
            // SCHEME WILL ENCODE USING the following:
            // <iteration+1><00000><sender_rank><00><position i>
            ret_val = (((iteration) + 1) * 100000 + (sender_rank)*100 + (position)*1);
        } else if (mode == PayloadValueMode::SMALL) {
            ret_val = (((iteration) + 1) * 8 + (sender_rank)*4 + (position)*1);
        } else if (mode == PayloadValueMode::TINY) {
            ret_val = (((iteration) + 1) * 1.3 + (sender_rank)*1 + (position)*1);
        } else {
            fprintf(stderr, "Invalid mode %d\n", mode);
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Unsupported PayloadType. Unable to generated payload_value for types other than "
                 "floating point and integral values\n");
        exit(EXIT_FAILURE);
    }

    return ret_val;
}

template <typename PayloadType>
int run_test(int my_rank, int num_ranks, int count, int root_rank, const MPI_Datatype& datatype,
             size_t iterations) {

    assert(my_rank < num_ranks);

    // step 1. create arrays.
    PayloadType* buf = new PayloadType[count];
    PayloadType* check_buf = new PayloadType[count];

    bool             is_root      = (my_rank == root_rank);
    PayloadValueMode payload_mode = PayloadValueMode::TINY;
    int              has_error    = 0;

    ///////////////////////////////////////////////////////////////////////////

    // Step 3a. Initialize buf values (for root) and check_buf (for all ranks)
    for (auto l = 0; l < iterations; ++l) {
        for (auto i = 0; i < count; ++i) {
            PayloadType v = payload_value<PayloadType>(l, root_rank, i, payload_mode);
            if (is_root) {
                buf[i] = v;
            }
            check_buf[i] = v;
        } // for count

        // Step 3b. Execute broadcast
        MPI::COMM_WORLD.Bcast(buf, count, datatype, root_rank);

        // Step 4. Verify correct received results of the bcast channel
        for (auto i = 0; i < count; ++i) {
            assert(buf[i] == check_buf[i]);
        }
    } // for iterations

    // step 5. clean up
    delete[](buf);
    delete[](check_buf);

    return has_error;
} // function run_test

int main(int argc, char* argv[]) {

    int iterations = -1;
    int count      = -1;

    if (argc < 3) {
        fprintf(stderr, "ERROR: usage: %s <num_iterations> <count>", argv[0]);
        exit(-1);
    } else {
        iterations = atoi(argv[1]);
        assert(iterations > 0);
        count = atoi(argv[2]);
        assert(count > 0);
        printf("Running with %d bcast iteration(s) and count %d\n", iterations, count);
    }

    MPI::Init(argc, argv);

    int my_rank   = MPI::COMM_WORLD.Get_rank();
    int num_ranks = MPI::COMM_WORLD.Get_size();

    int num_errors = 0;

    for (int root_rank = 0; root_rank < num_ranks; ++root_rank) {
        num_errors += run_test<int>   (my_rank, num_ranks, count, root_rank, MPI_INT,    iterations);
        num_errors += run_test<double>(my_rank, num_ranks, count, root_rank, MPI_DOUBLE, iterations);
        num_errors += run_test<float> (my_rank, num_ranks, count, root_rank, MPI_FLOAT,  iterations);
    }

    MPI::Finalize();

    return 0;
}
