//#include <spm_api.h>

#define STACK_BASE __builtin_frame_address(0)

#define TRACE_on
//#define SPM_on


#ifdef TRACE_on
#define TROI_WikiSort
#define TROI_benchmark
#define TROI_main
#define TROI_WikiMerge
#endif

#define stack_func_WikiMerge
#define stack_func_WikiSort
#define stack_func_benchmark
#define stack_func_main

#define heap_array_array1
#define heap_array_item
#define heap_array_total
#define heap_array_index
#define heap_array_test_case
#define heap_array_sort_temp
#define heap_array_levels_
#define heap_array_cache
#define heap_array_wiki_temp
#define heap_array_wiki_l_temp

#ifdef ARM64_ARCH
#elif ALPHA_ARCH
#elif ARM32_ARCH
#define stack_func_WikiMerge_size 80
#define stack_func_WikiSort_size 304
#define stack_func_benchmark_size 312
#define stack_func_main_size 32
#elif X86_ARCH
#endif
