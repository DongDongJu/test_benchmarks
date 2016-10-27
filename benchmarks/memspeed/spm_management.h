//#include <spm_api.h>

#define STACK_BASE __builtin_frame_address(0)

#define TRACE_on
//#define SPM_on
#define ARM32_ARCH

#ifdef TRACE_on
#define TROI_main
#endif


#define stack_func_main
#define heap_array_mem


#ifdef ARM32_ARCH
#define stack_func_main_size 88
#elif ARM64_ARCH
#elif ALPHA_ARCH
#elif X86_ARCH
#endif
