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


#ifdef TRACE_on
#define TROI_main
#endif

#define stack_func_main
#define stack_func_tjei_encode_main
#define stack_func_tjei_encode_and_write_MCU
#define stack_func_tjei_fdct
#define stack_func_stbi__bmp_load
#define stack_func_tje_encode_with_func
#define stack_func_tjei_huff_expand
// #define stack_func_stbi__get8
// #define stack_func_tjei_calculate_variable_length_int
// #define stack_func_tjei_write_bits
// #define stack_func_tjei_write
#define heap_array_data



#define stack_func_main_size 48
#define stack_func_tjei_encode_main_size 1536
#define stack_func_tjei_encode_and_write_MCU_size 680
#define stack_func_tjei_fdct_size 112
#define stack_func_stbi__bmp_load_size 1224
#define stack_func_tje_encode_with_func_size 4352
#define stack_func_tjei_huff_expand_size 3160
#define stack_func_tjei_calculate_variable_length_int_size 24
#define stack_func_tjei_write_bits_size 32
#define stack_func_tjei_write_size 32
#define stack_func_stbi__get8_size 16
