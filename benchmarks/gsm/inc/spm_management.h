#define STACK_BASE __builtin_frame_address(0)

//gsm_create.c
#define TROI_gsm_create

#define stack_func_gsm_create
#define stack_func_gsm_create_size

#define heap_array_gsm_state // need free  func : gsm_create


//gsm_destroy.c

#define TROI_gsm_destroy

#define stack_func_gsm_destroy
#define stack_func_gsm_destroy_size


//toast.c

#define TROI_main

#define stack_func_main
#define stack_func_main_size


#define TROI_process

#define stack_func_process
#define stack_func_process_size

#define TROI_process_encode

#define stack_func_process_encode
#define stack_func_process_encode_size
