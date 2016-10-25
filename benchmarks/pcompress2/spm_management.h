//#include <spm_api.h>

#define STACK_BASE __builtin_frame_address(0)

#define TRACE_on
//#define SPM_on
#define ARM32_ARCH

#ifdef TRACE_on
#define TROI_main
#define TROI_compress
#define TROI_do_rle
#define TROI_uncompress
#define TROI_do_debwe
#define TROI_do_ari
#define TROI_do_deari
#define TROI_decode_model
#define TROI_update_model
#endif


#define stack_func_main
#define stack_func_compress
#define stack_func_uncompress
#define stack_func_do_debwe
#define stack_func_do_ari
#define stack_func_do_deari
#define stack_func_decode_model
#define stack_func_update_model


#define heap_array_in
#define heap_array_bw
#define heap_array_rot
#define heap_array_rle
#define heap_array_ari
#define heap_array_un_in
#define heap_array_deari
#define heap_array_derle
#define heap_array_debw



#ifdef ARM32_ARCH
#define stack_func_do_ari_size 1088
#define stack_func_main_size 24
#define stack_func_compress_size 1072
#define stack_func_do_debwe_size 2080
#define stack_func_uncompress_size 32
#define stack_func_do_deari_size 40
#define stack_func_decode_model_size 40
#define stack_func_update_model_size 40
#elif ARM64_ARCH
#elif ALPHA_ARCH
#elif X86_ARCH
#endif
