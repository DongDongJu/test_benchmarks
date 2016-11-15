#ifndef __MINIMP3_H_INCLUDED__
#define __MINIMP3_H_INCLUDED__

#define MP3_MAX_SAMPLES_PER_FRAME (1152*2)
#define MP3_FRAME_SIZE 1152
#define MP3_MAX_CODED_FRAME_SIZE 1792
#define MP3_MAX_CHANNELS 2
#define SBLIMIT 32

#define MP3_STEREO  0
#define MP3_JSTEREO 1
#define MP3_DUAL    2
#define MP3_MONO    3

#define SAME_HEADER_MASK \
   (0xffe00000 | (3 << 17) | (0xf << 12) | (3 << 10) | (3 << 19))

#define FRAC_BITS   15
#define WFRAC_BITS  14

#define OUT_MAX (32767)
#define OUT_MIN (-32768)
#define OUT_SHIFT (WFRAC_BITS + FRAC_BITS - 15)

#define MODE_EXT_MS_STEREO 2
#define MODE_EXT_I_STEREO  1

#define FRAC_ONE    (1 << FRAC_BITS)
#define FIX(a)   ((int)((a) * FRAC_ONE))
#define FIXR(a)   ((int)((a) * FRAC_ONE + 0.5))
#define FRAC_RND(a) (((a) + (FRAC_ONE/2)) >> FRAC_BITS)
#define FIXHR(a) ((int)((a) * (1LL<<32) + 0.5))

#ifndef _MSC_VER
    #define MULL(a,b) (((int64_t)(a) * (int64_t)(b)) >> FRAC_BITS)
    #define MULH(a,b) (((int64_t)(a) * (int64_t)(b)) >> 32)
#else
    static INLINE int MULL(int a, int b) {
        int res;
        __asm {
            mov eax, a
            imul b
            shr eax, 15
            shl edx, 17
            or eax, edx
            mov res, eax
        }
        return res;
    }
    static INLINE int MULH(int a, int b) {
        int res;
        __asm {
            mov eax, a
            imul b
            mov res, edx
        }
        return res;
    }
#endif
#define MULS(ra, rb) ((ra) * (rb))

#define ISQRT2 FIXR(0.70710678118654752440)

#define HEADER_SIZE 4
#define BACKSTEP_SIZE 512
#define EXTRABYTES 24

#define VLC_TYPE int16_t

////////////////////////////////////////////////////////////////////////////////

struct _granule;

typedef struct _bitstream {
    const uint8_t *buffer, *buffer_end;
    int index;
    int size_in_bits;
} bitstream_t;

typedef struct _vlc {
    int bits;
    VLC_TYPE (*table)[2]; ///< code, bits
    int table_size, table_allocated;
} vlc_t;

typedef struct _mp3_context {
    uint8_t last_buf[2*BACKSTEP_SIZE + EXTRABYTES];
    int last_buf_size;
    int frame_size;
    uint32_t free_format_next_header;
    int error_protection;
    int sample_rate;
    int sample_rate_index;
    int bit_rate;
    bitstream_t gb;
    bitstream_t in_gb;
    int nb_channels;
    int mode;
    int mode_ext;
    int lsf;
    int16_t synth_buf[MP3_MAX_CHANNELS][512 * 2];
    int synth_buf_offset[MP3_MAX_CHANNELS];
    int32_t sb_samples[MP3_MAX_CHANNELS][36][SBLIMIT];
    int32_t mdct_buf[MP3_MAX_CHANNELS][SBLIMIT * 18];
    int dither_state;
} mp3_context_t;

typedef struct _granule {
    uint8_t scfsi;
    int part2_3_length;
    int big_values;
    int global_gain;
    int scalefac_compress;
    uint8_t block_type;
    uint8_t switch_point;
    int table_select[3];
    int subblock_gain[3];
    uint8_t scalefac_scale;
    uint8_t count1table_select;
    int region_size[3];
    int preflag;
    int short_start, long_end;
    uint8_t scale_factors[40];
    int32_t sb_hybrid[SBLIMIT * 18];
} granule_t;

typedef struct _huff_table {
    int xsize;
    const uint8_t *bits;
    const uint16_t *codes;
} huff_table_t;


typedef struct _mp3_info {
    int sample_rate;
    int channels;
    int audio_bytes;  // generated amount of audio per frame
} mp3_info_t;

typedef void* mp3_decoder_t;


extern mp3_decoder_t mp3_create(void);
extern int mp3_decode(mp3_decoder_t *dec, void *buf, int bytes, signed short *out, mp3_info_t *info);


#define mp3_free(dec) do { mp3_done(dec); dec = NULL; } while(0)

#endif//__MINIMP3_H_INCLUDED__
