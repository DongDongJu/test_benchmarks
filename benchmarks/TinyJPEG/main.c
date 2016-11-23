#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define TJE_IMPLEMENTATION
#include "tiny_jpeg.h"

int main()
{
    int width, height, num_components;
    unsigned char* data = stbi_load("in.bmp", &width, &height, &num_components, 0);
    if ( !data ) {
        puts("Could not find file");
        return EXIT_FAILURE;
    }

    if ( !tje_encode_to_file("out.jpg", width, height, num_components, data) ) {
        fprintf(stderr, "Could not write JPEG\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
