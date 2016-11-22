//#include <spm_api.h>

#define STACK_BASE __builtin_frame_address(0)
#define PRINT_TROI_MINUS(x)   printf("TROI- TROI_%s\n",x)
#define PRINT_TROI_PLUS(x)     printf("TROI+ TROI_%s\n",x)
#define PRINT_VAROI_FUNC_PLUS(x,size)    printf("VAROI+ stack_func_%s %p %p\n",x,STACK_BASE - size + 1,STACK_BASE)
#define PRINT_VAROI_FUNC_MINUS(x,size)    printf("VAROI- stack_func_%s %p %p\n",x,STACK_BASE - size +1,STACK_BASE)
#define PRINT_VAROI_ARRAY_PLUS(x,start,size)    printf("VAROI+ stack_array_%s %p %p\n",x,start - size + 1,start)
#define PRINT_VAROI_ARRAY_MINUS(x,start,size)    printf("VAROI- stack_array_%s %p %p\n",x,start - size +1,start)
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
#define TROI_mp3_create
#define TROI_mp3_done
#define TROI_mp3_decode
#define TROI_mp3_decode_frame
#define TROI_mp3_decode_init
#define TROI_mp3_decode_main
#define TROI_mp_decode_layer3
#define TROI_imdct36
#define TROI_dct32
#define TROI_build_table
#define TROI_huffman_decode
#define TROI_exponents_from_scale_factors
#define TROI_reorder_block
#endif

#define stack_func_main
#define stack_func_mp3_create
#define stack_func_mp3_done
#define stack_func_mp3_decode
#define stack_func_mp3_decode_frame
#define stack_func_mp3_decode_init
#define stack_func_mp3_decode_main
#define stack_func_mp_decode_layer3
#define stack_func_imdct36
#define stack_func_build_table
#define stack_func_dct32
#define stack_func_huffman_decode
#define stack_func_exponents_from_scale_factors
#define stack_func_reorder_block


#define heap_array_file_data
#define heap_array_dec
#define heap_array_table_4_3_exp
#define heap_array_table_4_3_value
//#define heap_array_huff_vlc
//#define heap_array_huff_quad_vlc
#define heap_array_band_index_long
#define heap_array_exp_table
#define heap_array_expval_table
#define heap_array_is_table
#define heap_array_is_table_lsf
#define heap_array_csa_table
#define heap_array_csa_table_float
#define heap_array_mdct_win


#define stack_func_main_size 4688
#define stack_func_mp3_create_size 16
#define stack_func_mp3_done_size 24
#define stack_func_mp3_decode_size 48
#define stack_func_mp3_decode_frame_size 40
#define stack_func_mp3_decode_init_size 1720
#define stack_func_mp3_decode_main_size 64
#define stack_func_mp_decode_layer3_size 10976
#define stack_func_imdct36_size 336
#define stack_func_build_table_size 112
#define stack_func_dct32_size 672
#define stack_func_huffman_decode_size 104
#define stack_func_exponents_from_scale_factors_size 80
#define stack_func_reorder_block_size 2344

