/********************************************************/
/* pCompress/pUnCompress                                */
/* A three stage file compressor, using Burrows Wheeler */
/* blocksorting, Run Length Encoding and Arithmetic     */
/* coding to achieve good compression.                  */
/* The fact that the compression is not that good in    */
/* reality is probably my fault.                        */
/* It makes a neat benchmark anyways.                   */
/* ---------------------------------------------------- */
/* This is a part of FreeBench v1 and is only intended  */
/* to be used as a benchmark. The use of this software  */
/* for anyting else (such as compression) is not        */
/* recomended, and certainly not supported. Use gzip or */
/* bzip instead, they are both faster and better.       */
/* Peter Rundberg, April 2001                           */
/********************************************************/

#define BENCHMARK

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "compress.h"
#include "uncompress.h"
#include "spm_management.h"

int main(int argc, char *argv[])
{
#ifdef TROI_main
    printf("TROI+ TROI_main\n");
#endif
#ifdef stack_func_main
    #ifdef TRACE_on
    printf("VAROI+ stack_func_main %p %p\n",STACK_BASE - stack_func_main_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)STACK_BASE - stack_func_main_size +1, (unsigned long)STACK_BASE, COPY, MAX_IMPORTANCE, HIGH_PRIORITY);
    #endif
#endif

    char* filename;
    char buff[128];
    int level;
    int opt;
    enum { COMPRESS_MODE, DECOMPRESS_MODE } mode = COMPRESS_MODE;


    while (( opt = getopt(argc,argv,"c:d:L:")) != -1)
    {
        switch(opt){
            case 'L':
            level= atoi(optarg);
            if(level ==1){
                compress("./data/small.in");
                uncompress("./data/small.in.compr");
            }else if(level == 2){
                compress("./data/small.in");
                uncompress("./data/small.in.compr");
                compress("./data/mid.in");
                uncompress("./data/mid.in.compr");
            }else if(level==3){
                compress("./data/small.in");
                uncompress("./data/small.in.compr");
                compress("./data/mid.in");
                uncompress("./data/mid.in.compr");
                compress("./data/large.in");
                uncompress("./data/large.in.compr");
            }
            break;

            case 'c':
            filename= optarg;
            compress(filename);
            break;

            case 'd':
            filename=optarg;
            uncompress(filename);
            break;
        }
    }
#ifdef stack_func_main
    #ifdef TRACE_on
    printf("VAROI- stack_func_main %p %p\n",STACK_BASE - stack_func_main_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)STACK_BASE - stack_func_main_size +1, (unsigned long)STACK_BASE, WRITE_BACK);
    #endif
#endif
#ifdef TROI_main
  printf("TROI- TROI_main\n");
#endif
  return 0;
}

