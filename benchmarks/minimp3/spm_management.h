//#include <spm_api.h>

#define STACK_BASE __builtin_frame_address(0)
#define PRINT_TROI_MINUS(x)   printf("TROI- TROI_%s\n",x)
#define PRINT_TROI_PLUS(x)     printf("TROI+ TROI_%s\n",x)
#define PRINT_VAROI_FUNC_PLUS(x,size)    printf("VAROI+ stack_func_%s %p %p\n",x,STACK_BASE - size + 1,STACK_BASE)
#define PRINT_VAROI_FUNC_MINUS(x,size)    printf("VAROI- stack_func_%s %p %p\n",x,STACK_BASE - size +1,STACK_BASE)
#define PRINT_VAROI_HEAP_PLUS(x,start,size)    printf("VAROI+ heap_array_%s %p %p\n",x,start,start+size-1)
#define PRINT_VAROI_HEAP_MINUS(x,start,size)    printf("VAROI- heap_array_%s %p %p\n",x,start,start+size-1)
#define SPM_ALLOC_STACK(size)   SPM_ALLOC((unsigned long)STACK_BASE, (unsigned long)STACK_BASE + size - 1, COPY, MAX_IMPORTANCE, HIGH_PRIORITY)
#define SPM_FREE_STACK(size)  SPM_FREE((unsigned long)STACK_BASE, (unsigned long)STACK_BASE + size - 1, WRITE_BACK)
#define SPM_ALLOC_HEAP(start,size)   SPM_ALLOC((unsigned long)start, (unsigned long)start - size + 1, COPY, MAX_IMPORTANCE, HIGH_PRIORITY)
#define SPM_FREE_HEAP(start,size)  SPM_FREE((unsigned long)start, (unsigned long)start - size + 1, WRITE_BACK)

#define TRACE_on
//#define SPM_on


#ifdef TRACE_on
#define TROI_main
#define TROI_build_table
#define TROI_huffman_decode
#endif

#define stack_func_main
#define stack_func_build_table
#define stack_func_huffman_decode
#define heap_array_file_data

#define stack_func_main_size 4704
#define stack_func_build_table_size 112
#define stack_func_huffman_decode_size 104


#define stack_func_mp3_synth_filter_size 200
#define stack_func_mp3_create_size 16
#define stack_func_mp3_decode_size 48
#define stack_func_mp3_decode_frame_size 40
#define stack_func_mp3_decode_main_size 64
#define stack_func_mp_decode_layer3_size 144
#define minimp3_stack_size
#define stack_func_imdct36_size 336
#define stack_func_reorder_block_size 2344
