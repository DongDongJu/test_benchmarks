/********************************************************/
/* pCompress/pUnCompress                                */
/* A three stage file compressor, using Burrows Wheeler */
/* blocksorting, Run Length Encoding and Arithmetic     */
/* coding to achieve good compression.                  */
/* The fact that the compression is not that good in    */
/* reality is probably my fault.                        */
/* It makes a neat benchmark anyways.                   */
/* ---------------------------------------------------- */
/* This is a part of FreeBench v1 and is only intended  */
/* to be used as a benchmark. The use of this software  */
/* for anyting else (such as compression) is not        */
/* recomended, and certainly not supported. Use gzip or */
/* bzip instead, they are both faster and better.       */
/* Peter Rundberg, April 2001                           */
/********************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "spm_management.h"
FILE *fpi,*fpo;
unsigned int *rot; /* Rotation info per byte */
static unsigned char *in; /* The infile */
unsigned char *bw; /* The B&W:d data */
unsigned char *rle; /* The RLE:d data */
unsigned char *ari; /* The ARI:d data */
static unsigned int size;

static void do_bwe();
static unsigned int do_rle();
unsigned int do_ari(unsigned int insize); /* In "arithmetic.c" */

void compress(char* filename)
{
#ifdef TROI_compress
    printf("TROI+ TROI_compress\n");
#endif
#ifdef stack_func_compress
    #ifdef TRACE_on
    printf("VAROI+ stack_func_compress %p %p\n",STACK_BASE - stack_func_compress_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)STACK_BASE - stack_func_compress_size +1, (unsigned long)STACK_BASE, COPY, MAX_IMPORTANCE, HIGH_PRIORITY);
    #endif
#endif

    char outname[1000];
    struct stat buf;
    unsigned int filesize, outsize;

    fpi=fopen(filename,"r"); /* open the infile */
    if (fpi==NULL) {
    fprintf(stderr,"ERROR: Could not find infile %s\n",filename);
    exit(1);
    }
    fseek(fpi, 0L, SEEK_END);
    filesize=ftell(fpi);
    fseek(fpi, 0L, SEEK_SET);
    strcpy(outname,filename); /* name the outfile */
    strcat(outname,".compr"); /* add the suffix '.compr' */
    fpo=fopen(outname,"w");
    if (fpo==NULL) {
        fprintf(stderr,"ERROR: Could not open outfile (do you have write permission here?)\n");
        exit(1);
    }

    /* Write the infile size to the outfile */
    fwrite(&filesize,sizeof(unsigned int),1,fpo);
    /* Allocate some memory... */
    in=(unsigned char *)malloc(2*filesize*sizeof(unsigned char));
#ifdef heap_array_in
    #ifdef TRACE_on
    printf("VAROI+ heap_array_in %p %p\n",in,in + (sizeof(char)*filesize*2) -1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)in, (unsigned long)in+(sizeof(char)*filesize*2 -1), COPY, MAX_IMPORTANCE, HIGH_PRIORITY);
    #endif
#endif

    bw=(unsigned char *)malloc(filesize*sizeof(unsigned char));
#ifdef heap_array_bw
    #ifdef TRACE_on
    printf("VAROI+ heap_array_bw %p %p\n",bw,bw + (sizeof(char)*filesize) -1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)bw, (unsigned long)bw+(sizeof(char)*filesize -1), COPY, MAX_IMPORTANCE, HIGH_PRIORITY);
    #endif
#endif

    rot=(unsigned int *)malloc(filesize*sizeof(unsigned int));
#ifdef heap_array_rot
    #ifdef TRACE_on
    printf("VAROI+ heap_array_rot %p %p\n",rot,rot + (sizeof(int)*filesize) -1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)rot, (unsigned long)rot+(sizeof(int)*filesize -1), COPY, MAX_IMPORTANCE, HIGH_PRIORITY);
    #endif
#endif

    rle=(unsigned char *)malloc(2*filesize*sizeof(unsigned char));
#ifdef heap_array_rle
    #ifdef TRACE_on
    printf("VAROI+ heap_array_rle %p %p\n",rle,rle + (sizeof(char)*filesize*2) -1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)rle, (unsigned long)rle+(sizeof(char)*filesize*2 -1), COPY, MAX_IMPORTANCE, HIGH_PRIORITY);
    #endif
#endif

    ari=(unsigned char *)malloc(2*filesize*sizeof(unsigned char));
#ifdef heap_array_ari
    #ifdef TRACE_on
    printf("VAROI+ heap_array_ari %p %p\n",ari,ari + (sizeof(char)*filesize*2) -1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)ari, (unsigned long)ari+(sizeof(char)*filesize*2 -1), COPY, MAX_IMPORTANCE, HIGH_PRIORITY);
    #endif
#endif

  if (!in || !bw || !rot || !rle || !ari) {
    fprintf(stderr,"ERROR: Out of memory\n");
    exit(1);
  }

  if (fread(in,sizeof(unsigned char),filesize,fpi)!=filesize) {
    printf("Something is fishy regarding the file size\n");
    exit(1);
  }

    size=filesize;
    /* Do the Burrows Wheeler encoding */
    do_bwe();

    free(in); /* We can get rid of 'in' now */
#ifdef heap_array_in
    #ifdef TRACE_on
    printf("VAROI- heap_array_in %p %p\n",in,in + (sizeof(char)*filesize*2) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)in, (unsigned long)in+(sizeof(char)*filesize*2 -1), WRITE_BACK);
    #endif
#endif

    free(rot); /* We can get rid of 'rot' now */
#ifdef heap_array_rot
    #ifdef TRACE_on
    printf("VAROI- heap_array_rot %p %p\n",rot,rot + (sizeof(int)*filesize) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)rot, (unsigned long)rot+(sizeof(int)*filesize -1), WRITE_BACK);
    #endif
#endif
  /* Do the RLE */
    outsize=do_rle();
    free(bw); /* We can get rid of 'bw' now */
#ifdef heap_array_bw
    #ifdef TRACE_on
    printf("VAROI- heap_array_bw %p %p\n",bw,bw + (sizeof(char)*filesize) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)bw, (unsigned long)bw+(sizeof(char)*filesize -1), WRITE_BACK);
    #endif
#endif

  /* Do the arithmetic encoding */
    outsize=do_ari(outsize);
    free(rle); /* We can get rid of 'rle' now */
#ifdef heap_array_rle
    #ifdef TRACE_on
    printf("VAROI- heap_array_rle %p %p\n",rle,rle + (sizeof(char)*filesize*2) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)rle, (unsigned long)rle+(sizeof(char)*filesize*2 -1), WRITE_BACK);
    #endif
#endif

  /* Write to file */
    fwrite(ari,sizeof(unsigned char),outsize,fpo);
    free(ari); /* We can get rid of 'ari' now */
#ifdef heap_array_ari
    #ifdef TRACE_on
    printf("VAROI- heap_array_ari %p %p\n",ari,ari + (sizeof(char)*filesize*2) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)ari, (unsigned long)ari+(sizeof(char)*filesize*2 -1), WRITE_BACK);
    #endif
#endif
    fclose(fpi);
    fclose(fpo);

#ifdef stack_func_compress
    #ifdef TRACE_on
    printf("VAROI- stack_func_compress %p %p\n",STACK_BASE - stack_func_compress_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)STACK_BASE - stack_func_compress_size +1, (unsigned long)STACK_BASE, WRITE_BACK);
    #endif
#endif

#ifdef TROI_compress
    printf("TROI- TROI_compress\n");
#endif
}

/* Compare two strings */
static int compare(const void *a, const void *b)
{
    unsigned int *first=(unsigned int *)a;
    unsigned int *sec=(unsigned int *)b;

  /* Compare strings using memcmp */
    return (memcmp(in+*first,in+*sec,size));
}

static void do_bwe()
{

#ifdef TROI_do_bwe
    printf("TROI+ TROI_do_bwe\n");
#endif

#ifdef stack_func_do_bwe
    #ifdef TRACE_on
    printf("VAROI+ stack_func_do_bwe %p %p\n",STACK_BASE - stack_func_do_bwe_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)STACK_BASE - stack_func_do_bwe_size +1, (unsigned long)STACK_BASE, COPY, MAX_IMPORTANCE, HIGH_PRIORITY);
    #endif
#endif
    unsigned int i;

  /*
   * Put a copy of the string at the end of the string,
   * this speeds up rotating.
   */
    memcpy(in+size,in,size);

    for (i=0;i<size;i++)  /* Initialize 'rot' vector... */
      rot[i]=i;

  /* sort the strings using STDLIB qsort */
    qsort(rot,size,sizeof(unsigned int),(*compare));

  /* make BW array... */
    for (i=0;i<size;i++) {
        bw[i]=in[(rot[i]+size-1)%size];
    }

  /* Find place of original string, and write it to the outfile*/
    for (i=0;i<size;i++) {
        if (rot[i]==0) {
            fwrite(&i,sizeof(unsigned int),1,fpo);
            break;
        }
    }
#ifdef stack_func_do_bwe
    #ifdef TRACE_on
    printf("VAROI- stack_func_do_bwe %p %p\n",STACK_BASE - stack_func_do_bwe_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)STACK_BASE - stack_func_do_bwe_size +1, (unsigned long)STACK_BASE, WRITE_BACK);
    #endif
#endif

#ifdef TROI_do_bwe
    printf("TROI- TROI_do_bwe\n");
#endif
}

static unsigned int do_rle()
{
#ifdef TROI_do_rle
    printf("TROI+ TROI_do_rle\n");
#endif
#ifdef stack_func_do_rle
    #ifdef TRACE_on
    printf("VAROI+ stack_func_do_rle %p %p\n",STACK_BASE - stack_func_do_rle_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)STACK_BASE - stack_func_do_rle_size +1, (unsigned long)STACK_BASE, COPY, MAX_IMPORTANCE, HIGH_PRIORITY);
    #endif
#endif

    unsigned int i, c, rlepos=0;
    unsigned char teck, count;
  /* RLE --
   * If the same byte occurs twice or more in s row, put a byte
   * before to show number of repeats. If different bytes occur in a row
   * put a byte before to show how large the block of unique bytes is.
   * A set (1) bit 7 in the describer byte indicates repeats, a cleared
   * bit 7 indicates block of unique bytes.
   */
    for (i=0;i<size;) {
      c=1;
      teck=bw[i];
      while ((i+c)<size && teck==bw[i+c]) { /* How many repeats? */
          c++;
          if (c>=127)
              break;
      }
      if (c==1) { /* No repeats */
          if ((i+c)<size) {
	       while (bw[i+c-1]!=bw[i+c] && bw[i+c]!=bw[i+c+1])
	           c++;
          }
          count=(unsigned char)c & 0x7f;
          rle[rlepos++]=count;
          memcpy(rle+rlepos,bw+i,c);
          rlepos+=c;
          i+=c;
      } else {  /* c repeats */
          if ((rlepos+2)>2*size) {
	       fprintf(stderr,"PANIC: RLE buf larger than %d bytes needed (repeat)\n",size);
	       exit(1);
          }
          count=(unsigned char)c | 0x80;
          rle[rlepos]=count;
          rle[rlepos+1]=teck;
          rlepos+=2;
          i+=c;
      }
    }
#ifdef stack_func_do_rle
    #ifdef TRACE_on
    printf("VAROI- stack_func_do_rle %p %p\n",STACK_BASE - stack_func_do_rle_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)STACK_BASE - stack_func_do_rle_size +1, (unsigned long)STACK_BASE, WRITE_BACK);
    #endif
#endif
#ifdef TROI_do_rle
    printf("TROI- TROI_do_rle\n");
#endif
    return rlepos;
}



