#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define TJE_IMPLEMENTATION
#include "tiny_jpeg.h"
#include "spm_management.h"
#include <string.h>
int main(int argc, char* argv[])
{
#ifdef TROI_main
    PRINT_TROI_PLUS("main");
#endif
#ifdef stack_func_main
    #ifdef TRACE_on
        PRINT_VAROI_FUNC_PLUS("main",stack_func_main_size);
    #endif
    #ifdef SPM_on
        SPM_ALLOC_STACK(stack_func_main_size);
    #endif
#endif

    int width, height, num_components;
    char* in;
    if (argc == 2){
        in=argv[1];
    }else{
        in="in.bmp";
    }

    unsigned char* data = stbi_load(in, &width, &height, &num_components, 0);

    if ( !data ) {
        puts("Could not find file");
        return EXIT_FAILURE;
    }

    if ( !tje_encode_to_file("out.jpg", width, height, num_components, data) ) {
        fprintf(stderr, "Could not write JPEG\n");
        return EXIT_FAILURE;
    }

    free(data);
#ifdef heap_array_data
    #ifdef TRACE_on
        PRINT_VAROI_HEAP_MINUS("data",data,sizeof(unsigned char)*len_of_out);
    #endif
    #ifdef SPM_on
        SPM_FREE_HEAP(data,sizeof(unsigned char)*len_of_out);
    #endif
#endif

#ifdef stack_func_main
    #ifdef TRACE_on
        PRINT_VAROI_FUNC_MINUS("main",stack_func_main_size);
    #endif
    #ifdef SPM_on
        SPM_FREE_STACK(stack_func_main_size);
    #endif
#endif
#ifdef TROI_main
    PRINT_TROI_MINUS("main");
#endif

    return EXIT_SUCCESS;
}
