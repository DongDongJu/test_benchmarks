// minimp3 example player application for Linux/OSS
// this file is public domain -- do with it whatever you want!
#include "libc.h"
#include "minimp3.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include "spm_management.h"

size_t strlen(const char *s);
#define out(text) write(1, (const void *) text, strlen(text))

int main(int argc, char *argv[]) {
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

    mp3_decoder_t mp3;
    mp3_info_t info;
    int pcm;
    void *file_data;
    int leng;
    unsigned char *stream_pos;
    signed short sample_buf[MP3_MAX_SAMPLES_PER_FRAME];
    int bytes_left;
    int frame_size;
    int value;
    char* new_buffer;
    int fd;
    char idx=0;
    if (argc < 2) {
        out("Error: no input file specified!\n");
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        out("Error: cannot open `");
        out(argv[1]);
        out("'!\n");
        return 1;
    }

    bytes_left = lseek(fd, 0, SEEK_END);
    leng=bytes_left;

    file_data = mmap(0, bytes_left, PROT_READ, MAP_PRIVATE, fd, 0);
#ifdef heap_array_file_data
    #ifdef TRACE_on
        PRINT_VAROI_HEAP_PLUS("file_data",file_data,bytes_left);
    #endif
    #ifdef SPM_on
        SPM_ALLOC_HEAP(file_data,bytes_left);
    #endif
#endif
    stream_pos = (unsigned char *) file_data;
    bytes_left -= 100;

    mp3 = mp3_create();
    frame_size = mp3_decode(mp3, stream_pos, bytes_left, sample_buf, &info);

    if (!frame_size) {
        out("\nError: not a valid MP3 audio file!\n");

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

        return 1;
    }

    #define FAIL(msg) { \
        out("\nError: " msg "\n"); \
        return 1; \
    }


    while ((bytes_left >= 0) && (frame_size > 0)) {
        stream_pos += frame_size;
        bytes_left -= frame_size;
//        write(pcm, (const void *) sample_buf, info.audio_bytes);
        frame_size = mp3_decode(mp3, stream_pos, bytes_left, sample_buf, NULL);
    }

    close(pcm);
    munmap(file_data,leng);
#ifdef heap_array_file_data
    #ifdef TRACE_on
        PRINT_VAROI_HEAP_MINUS("file_data",file_data,leng);
    #endif
    #ifdef SPM_on
        SPM_FREE_HEAP(file_data,bytes_left);
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
    return 0;
}
