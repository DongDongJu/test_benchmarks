#define STACK_BASE __builtin_frame_address(0)

#define PRINT_TROI_MINUS(x)   printf("TROI- %s\n",x)
#define PRINT_TROI_PLUS(x)     printf("TROI+ %s\n",x)


#define PRINT_VAROI_FUNC_PLUS(x,start,end)    printf("VAROI+ stack_func_%s %p %p\n",x,start,end)
#define PRINT_VAROI_FUNC_MINUS(x,start,end)    printf("VAROI- stack_func_%s %p %p\n",x,start,end)

#define PRINT_VAROI_HEAP_PLUS(x,start,end)    printf("VAROI+ heap_array_%s %p %p\n",x,start,end)
#define PRINT_VAROI_HEAP_MINUS(x,start,end)    printf("VAROI- heap_array_%s %p %p\n",x,start,end)

#define stack_func_mp3_synth_filter_size 200
#define stack_func_huffman_decode_size 104
#define stack_func_mp3_create_size 16
#define stack_func_mp3_decode_size 48
#define stack_func_mp3_decode_frame_size 40
#define stack_func_mp3_decode_main_size 64
#define stack_func_mp_decode_layer3_size 144
#define stack_func_build_table_size 112
#define stack_func_main_size 4688
#define minimp3_stack_size
#define stack_func_imdct36_size 336
#define stack_func_reorder_block_size 2344
