/********************************************************************/
/* parameters of different tests                                    */
/* Note that changing parameters of tests can affect the number of  */
/* nodes on which the tests can be executed. Therefore update also  */
/* diag.h!                                                          */
/********************************************************************/

#ifndef DATA_ACCESS_NBLOCK_PARAMS
#define DATA_ACCESS_NBLOCK_PARAMS 

#define F_IREAD_AT_1_SIZE 100   /* Number of elements of array */
#define F_IREAD_AT_2_SIZE 100   /* Number of elements of array */
#define F_IREAD_AT_3_SIZE 100   /* Number of elements of array */
#define F_IREAD_AT_4_SIZE 100   /* Number of elements of array */
#define F_IREAD_AT_5_SIZE 100   /* Number of elements of array */
#define F_IREAD_AT_6_SIZE 100   /* Number of elements of array */

#define F_IREAD_AT_7_SIZE 100   /* Number of elements of array */
#define F_IREAD_AT_7_CONC_READS 20 /* Number of concurrent nonblocking */
                                   /* read-operations in one process */

#define F_IREAD_AT_8_SIZE 100      /* Number of elements of array */
#define F_IREAD_AT_8_CONC_READS 20 /* Number of concurrent nonblocking */
                                   /* read-operations in one process */

/*------------------------------------------------------------------*/

#define F_IWRITE_AT_1_BLKSIZE 100 /* Max Number of elem. of one written blk */
#define F_IWRITE_AT_2_BLKSIZE 100 /* Max Number of elem. of one written blk */
#define F_IWRITE_AT_3_BLKSIZE 100 /* Max Number of elem. of one written blk */
#define F_IWRITE_AT_4_BLKSIZE 100 /* Max Number of elem. of one written blk */
#define F_IWRITE_AT_5_BLKSIZE 100 /* Max Number of elem. of one written blk */
#define F_IWRITE_AT_6_BLKSIZE 100 /* Max Number of elem. of one written blk */

#define F_IWRITE_AT_7_BLKSIZE 100 /* Max Number of elem. of one written blk */
#define F_IWRITE_AT_7_CONC_WRITES 20 /* Number of concurrent nonblocking */
                                     /* write-operations in one process  */

#define F_IWRITE_AT_8_BLKSIZE 100 /* Max Number of elem. of one written blk */
#define F_IWRITE_AT_8_CONC_WRITES 20 /* Number of concurrent nonblocking */
                                     /* write-operations in one process  */

/*------------------------------------------------------------------*/

#endif /* DATA_ACCESS_NBLOCK_PARAMS */
