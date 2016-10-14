#define SCALE_FACTOR    (REPEAT_FACTOR >> 10)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "spm_management.h"

/* various #defines for the C code */
#ifndef true
	#define true 1
	#define false 0
	typedef uint8_t bool;
#endif

#define Var(name, value)				__typeof__(value) name = value
#define Allocate(type, count)				(type *)malloc((count) * sizeof(type))

long Min(const long a, const long b) {
#ifdef TROI_Min
    printf("TROI+ TROI_Min\n");
#endif
#ifdef stack_func_Min
    printf("VAROI+ stack_func_Min %p %p\n",STACK_BASE - stack_func_Min_size +1 , STACK_BASE);
#endif
	if (a < b){
#ifdef stack_func_Min
    printf("VAROI- stack_func_Min %p %p\n",STACK_BASE - stack_func_Min_size +1 , STACK_BASE);
#endif
#ifdef TROI_Min
    printf("TROI- TROI_Min\n");
#endif
        return a;
	}
#ifdef stack_func_Min
    printf("VAROI- stack_func_Min %p %p\n",STACK_BASE - stack_func_Min_size +1 , STACK_BASE);
#endif
#ifdef TROI_Min
    printf("TROI- TROI_Min\n");
#endif
    return b;
}

long Max(const long a, const long b) {
#ifdef TROI_Max
    printf("TROI+ TROI_Max\n");
#endif
#ifdef stack_func_Max
    printf("VAROI+ stack_func_Max %p %p\n",STACK_BASE - stack_func_Max_size +1 , STACK_BASE);
#endif
    if (a > b){
#ifdef stack_func_Max
    printf("VAROI- stack_func_Max %p %p\n",STACK_BASE - stack_func_Max_size +1 , STACK_BASE);
#endif
#ifdef TROI_Max
    printf("TROI- TROI_Max\n");
#endif
        return a;
    }
#ifdef stack_func_Max
    printf("VAROI- stack_func_Max %p %p\n",STACK_BASE - stack_func_Max_size +1 , STACK_BASE);
#endif  
#ifdef TROI_Max
    printf("TROI- TROI_Max\n");
#endif
	return b;
}


/* structure to test stable sorting (index will contain its original index in the array, to make sure it doesn't switch places with other items) */
typedef struct {
	int value;
	int index;
} Test;


typedef bool (*Comparison)(Test, Test);



/* structure to represent ranges within the array */
typedef struct {
	long start;
	long end;
} Range;

long Range_length(Range range) { return range.end - range.start; }

Range MakeRange(const long start, const long end) {
	Range range;
#ifdef TROI_MakeRange
    printf("TROI+ TROI_MakeRange\n");
#endif
#ifdef stack_MakeRangeFfl
    printf("VAROI+ stack_func_MakeRange %p %p\n",STACK_BASE - stack_func_MakeRange_size +1 , STACK_BASE);
#endif
	range.start = start;
	range.end = end;
#ifdef stack_MakeRange
    printf("VAROI- stack_func_MakeRange %p %p\n",STACK_BASE - stack_func_MakeRange_size +1 , STACK_BASE);
#endif
#ifdef TROI_MakeRange
    printf("TROI- TROI_MakeRange\n");
#endif
	return range;
}


/* toolbox functions used by the sorter */

/* swap value1 and value2 */
#define Swap(value1, value2) { \
	Var(a, &(value1)); \
	Var(b, &(value2)); \
	\
	Var(c, *a); \
	*a = *b; \
	*b = c; \
}
static inline bool TestCompare(Test item1, Test item2) { return (item1.value < item2.value); }

/* 63 -> 32, 64 -> 64, etc. */
/* apparently this comes from Hacker's Delight? */
long FloorPowerOfTwo (const long value) {
	long* x;
	long re;
#ifdef TROI_FloorPowerOfTwo
    printf("TROI+ TROI_FloorPowerOfTwo\n");
#endif
#ifdef stack_func_FloorPowerOfTwo
    printf("VAROI+ stack_func_FloorPowerOfTwo %p %p\n",STACK_BASE - stack_func_FloorPowerOfTwo_size +1 , STACK_BASE);
#endif
    	x=(long*)malloc(sizeof(long));
#ifdef heap_array_floor_value
    printf("VAROI+ heap_array_floor_value %p %p\n",x,x + (sizeof(long)) -1);
#endif
    	*x=value;
	(*x) = (*x) | ((*x) >> 1);
	(*x) = (*x) | ((*x) >> 2);
	(*x) = (*x) | ((*x) >> 4);
	(*x) = (*x) | ((*x) >> 8);
	(*x) = (*x) | ((*x) >> 16);

#if __LP64__
	(*x) = (*x) | ((*x) >> 32);
#endif    	
    	re= (*x) - ((*x) >> 1);

    	free(x);
#ifdef heap_array_floor_value
    printf("VAROI- heap_array_floor_value %p %p\n",x,x + (sizeof(long)) -1);
#endif
#ifdef stack_func_FloorPowerOfTwo
    printf("VAROI- stack_func_FloorPowerOfTwo %p %p\n",STACK_BASE - stack_func_FloorPowerOfTwo_size +1 , STACK_BASE);
#endif
#ifdef TROI_FloorPowerOfTwo
    printf("TROI- TROI_FloorPowerOfTwo\n");
#endif

	return re;
}

/* find the index of the first value within the range that is equal to array[index] */
long BinaryFirst(const Test array[], const long index, const Range range, const Comparison compare) {
	long start = range.start, end = range.end - 1;
#ifdef TROI_BinaryFirst
    printf("TROI+ TROI_BinaryFirst\n");
#endif
#ifdef stack_func_BinaryFirst
    printf("VAROI+ stack_func_BinaryFirst %p %p\n",STACK_BASE - stack_func_BinaryFirst_size +1 , STACK_BASE);
#endif
	while (start < end) {
		long mid = start + (end - start)/2;
		if (compare(array[mid], array[index]))
			start = mid + 1;
		else
			end = mid;
	}
	if (start == range.end - 1 && compare(array[start], array[index])) start++;
#ifdef stack_func_BinaryFirst
    printf("VAROI- stack_func_BinaryFirst %p %p\n",STACK_BASE - stack_func_BinaryFirst_size +1 , STACK_BASE);
#endif
#ifdef TROI_BinaryFirst
    printf("TROI- TROI_BinaryFirst\n");
#endif
	return start;
}

/* find the index of the last value within the range that is equal to array[index], plus 1 */
long BinaryLast(const Test array[], const long index, const Range range, const Comparison compare) {
	long start = range.start, end = range.end - 1;
#ifdef TROI_BinaryLast
    printf("TROI+ TROI_BinaryLast\n");
#endif
#ifdef stack_func_BinaryLast
    printf("VAROI+ stack_func_BinaryLast %p %p\n",STACK_BASE - stack_func_BinaryLast_size +1 , STACK_BASE);
#endif
	while (start < end) {
		long mid = start + (end - start)/2;
		if (!compare(array[index], array[mid]))
			start = mid + 1;
		else
			end = mid;
	}
	if (start == range.end - 1 && !compare(array[index], array[start])) start++;
#ifdef stack_func_BinaryLast
    printf("VAROI- stack_func_BinaryLast %p %p\n",STACK_BASE - stack_func_BinaryLast_size +1 , STACK_BASE);
#endif
#ifdef TROI_BinaryLast
    printf("TROI- TROI_BinaryLast\n");
#endif
	return start;
}

/* n^2 sorting algorithm used to sort tiny chunks of the full array */
void InsertionSort(Test array[], const Range range, const Comparison compare) {
	long i;
#ifdef TROI_InsertionSort
    printf("TROI+ TROI_InsertionSort\n");
#endif
#ifdef stack_func_InsertionSort
    printf("VAROI+ stack_func_InsertionSort %p %p\n",STACK_BASE - stack_func_InsertionSort_size +1 , STACK_BASE);
#endif    
	for (i = range.start + 1; i < range.end; i++) {
		Test temp=array[i];
		long j;
		for (j = i; j > range.start && compare(temp, array[j - 1]); j--)
			array[j] = array[j - 1];
		array[j] = temp;
	}
#ifdef stack_func_InsertionSort
    printf("VAROI- stack_func_InsertionSort %p %p\n",STACK_BASE - stack_func_InsertionSort_size +1 , STACK_BASE);
#endif    	
#ifdef TROI_InsertionSort
    printf("TROI- TROI_InsertionSort\n");
#endif
}

/* reverse a range within the array */
void Reverse(Test array[], const Range range) {
	long index;
#ifdef TROI_Reverse
    printf("TROI+ TROI_Reverse\n");
#endif
#ifdef stack_func_Reverse
    printf("VAROI+ stack_func_Reverse %p %p\n",STACK_BASE - stack_func_Reverse_size +1 , STACK_BASE);
#endif        
	for (index = Range_length(range)/2 - 1; index >= 0; index--)
		Swap(array[range.start + index], array[range.end - index - 1]);
#ifdef stack_func_Reverse
    printf("VAROI- stack_func_Reverse %p %p\n",STACK_BASE - stack_func_Reverse_size +1 , STACK_BASE);
#endif 
#ifdef TROI_Reverse
    printf("TROI- TROI_Reverse\n");
#endif
}

/* swap a series of values in the array */
void BlockSwap(Test array[], const long start1, const long start2, const long block_size) {
	long index;
#ifdef TROI_BlockSwap
    printf("TROI+ TROI_BlockSwap\n");
#endif
#ifdef stack_func_BlockSwap
    printf("VAROI+ stack_func_BlockSwap %p %p\n",STACK_BASE - stack_func_BlockSwap_size +1 , STACK_BASE);
#endif 
	for (index = 0; index < block_size; index++)
		Swap(array[start1 + index], array[start2 + index]);
#ifdef stack_func_BlockSwap
    printf("VAROI- stack_func_BlockSwap %p %p\n",STACK_BASE - stack_func_BlockSwap_size +1 , STACK_BASE);
#endif 
#ifdef TROI_BlockSwap
    printf("TROI- TROI_BlockSwap\n");
#endif
}

/* rotate the values in an array ([0 1 2 3] becomes [1 2 3 0] if we rotate by 1) */
void Rotate(Test array[], const long amount, const Range range, Test cache[], const long cache_size) {
	long split; Range range1, range2;
#ifdef TROI_Rotate
    printf("TROI+ TROI_Rotate\n");
#endif
#ifdef stack_func_Rotate
    printf("VAROI+ stack_func_Rotate %p %p\n",STACK_BASE - stack_func_Rotate_size +1 , STACK_BASE);
#endif     
	if (Range_length(range) == 0){
        return;
#ifdef stack_func_Rotate
    printf("VAROI- stack_func_Rotate %p %p\n",STACK_BASE - stack_func_Rotate_size +1 , STACK_BASE);
#endif     
#ifdef TROI_Rotate
    printf("TROI- TROI_Rotate\n");
#endif
    }
	if (amount >= 0)
		split = range.start + amount;
	else
		split = range.end + amount;

	range1 = MakeRange(range.start, split);
	range2 = MakeRange(split, range.end);

	/* if the smaller of the two ranges fits into the cache, it's *slightly* faster copying it there and shifting the elements over */
	if (Range_length(range1) <= Range_length(range2)) {
		if (Range_length(range1) <= cache_size) {
			memcpy(&cache[0], &array[range1.start], Range_length(range1) * sizeof(array[0]));
			memmove(&array[range1.start], &array[range2.start], Range_length(range2) * sizeof(array[0]));
			memcpy(&array[range1.start + Range_length(range2)], &cache[0], Range_length(range1) * sizeof(array[0]));
#ifdef stack_func_Rotate
    printf("VAROI- stack_func_Rotate %p %p\n",STACK_BASE - stack_func_Rotate_size +1 , STACK_BASE);
#endif     
#ifdef TROI_Rotate
    printf("TROI- TROI_Rotate\n");
#endif
			return;
		}
	} else {
		if (Range_length(range2) <= cache_size) {
			memcpy(&cache[0], &array[range2.start], Range_length(range2) * sizeof(array[0]));
			memmove(&array[range2.end - Range_length(range1)], &array[range1.start], Range_length(range1) * sizeof(array[0]));
			memcpy(&array[range1.start], &cache[0], Range_length(range2) * sizeof(array[0]));
#ifdef stack_func_Rotate
    printf("VAROI- stack_func_Rotate %p %p\n",STACK_BASE - stack_func_Rotate_size +1 , STACK_BASE);
#endif     
#ifdef TROI_Rotate
    printf("TROI- TROI_Rotate\n");
#endif
			return;
		}
	}

	Reverse(array, range1);
	Reverse(array, range2);
	Reverse(array, range);
#ifdef stack_func_Rotate
    printf("VAROI- stack_func_Rotate %p %p\n",STACK_BASE - stack_func_Rotate_size +1 , STACK_BASE);
#endif     	
#ifdef TROI_Rotate
    printf("TROI- TROI_Rotate\n");
#endif

}

/* standard merge operation using an internal or external buffer */
void WikiMerge(Test array[], const Range buffer, const Range A, const Range B, const Comparison compare, Test cache[], const long cache_size) {
	/* if A fits into the cache, use that instead of the internal buffer */
#ifdef TROI_WikiMerge
    printf("TROI+ TROI_WikiMerge\n");
#endif

#ifdef stack_func_WikiMerge
    #ifdef TRACE_on
    printf("VAROI+ stack_func_WikiMerge %p %p\n",STACK_BASE - stack_func_WikiMerge_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)(STACK_BASE - stack_func_WikiMerge_size +1),(unsigned long)STACK_BASE, COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif
#endif     
	Test **temp;
	long* l_temp;
	temp= (Test**)malloc(sizeof(Test*)*5);

#ifdef heap_array_wiki_temp
    #ifdef TRACE_on
    printf("VAROI+ heap_array_wiki_temp %p %p\n",temp, temp + (sizeof(Test*)* 5) -1);
    #endif
    #ifdef SPM_on
   SPM_ALLOC((unsigned long)temp,(unsigned long)(temp + (sizeof(Test*)* 5)) -1, COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif
#endif

    l_temp=(long*)malloc(sizeof(long)*3);
#ifdef heap_array_wiki_l_temp
    #ifdef TRACE_on
    printf("VAROI+ heap_array_wiki_l_temp %p %p\n",l_temp, l_temp + (sizeof(long)*3) -1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)l_temp,(unsigned long)( l_temp + (sizeof(long)*3) -1), COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif
#endif
	if (Range_length(A) <= cache_size) {
		temp[0] = &cache[0];
		temp[1] = &array[B.start];
		temp[2] = &array[A.start];
		temp[3] = &cache[Range_length(A)];
		temp[4] = &array[B.end];
		if (Range_length(B) > 0 && Range_length(A) > 0) {
			while (true) {
				if (!compare(*temp[1], *temp[0])) {
					*temp[2] = *temp[0];
					temp[0]++;
					temp[2]++;
					if (temp[0] == temp[3]) break;
				} else {
					*temp[2] = *temp[1];
					temp[1]++;
					temp[2]++;
					if (temp[1] == temp[4]) break;
				}
			}
		}
		/* copy the remainder of A into the final array */
		memcpy(temp[2], temp[0], (temp[3] - temp[0]) * sizeof(array[0]));
	} else {
		/* whenever we find a value to add to the final array, swap it with the value that's already in that spot */
		/* when this algorithm is finished, 'buffer' will contain its original contents, but in a different order */
		l_temp[0]=0;
		l_temp[1]=0;
		l_temp[2]=0;
		if (Range_length(B) > 0 && Range_length(A) > 0) {
			while (true) {
				if (!compare(array[B.start + l_temp[1]], array[buffer.start + l_temp[0]])) {
					Swap(array[A.start + l_temp[2]], array[buffer.start + l_temp[0]]);
					l_temp[0]++;
					l_temp[2]++;
					if (l_temp[0] >= Range_length(A)) break;
				} else {
					Swap(array[A.start + l_temp[2]], array[B.start + l_temp[1]]);
					l_temp[1]++;
					l_temp[2]++;
					if (l_temp[1] >= Range_length(B)) break;
				}
			}
		}

		/* swap the remainder of A into the final array */
		BlockSwap(array, buffer.start + l_temp[0], A.start + l_temp[2], Range_length(A) -l_temp[0]);
		printf("sec__\n");		
	}
	free(l_temp);
#ifdef heap_array_wiki_l_temp
    #ifdef TRACE_on
    printf("VAROI- heap_array_wiki_l_temp %p %p\n",l_temp, l_temp + (sizeof(long)*3) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)l_temp,(unsigned long)( l_temp + (sizeof(long)*3) -1),WRITE_BACK);
    #endif
#endif
	free(temp);
#ifdef heap_array_wiki_temp
    #ifdef TRACE_on
    printf("VAROI- heap_array_wiki_temp %p %p\n",temp, temp + (sizeof(Test*)* 5) -1);
    #endif
    #ifdef SPM_on
   SPM_FREE((unsigned long)temp,(unsigned long)(temp + (sizeof(Test*)* 5)) -1, WRITE_BACK);
    #endif
#endif

#ifdef stack_func_WikiMerge
    #ifdef TRACE_on
    printf("VAROI- stack_func_WikiMerge %p %p\n",STACK_BASE - stack_func_WikiMerge_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)(STACK_BASE - stack_func_WikiMerge_size +1),(unsigned long)STACK_BASE,WRITE_BACK);
    #endif
#endif
#ifdef TROI_WikiMerge
    printf("TROI- TROI_WikiMerge\n");
#endif
}
void WikiSort(Test array[], const long size, const Comparison compare) {
	/* use a small cache to speed up some of the operations */
	/* since the cache size is fixed, it's still O(1) memory! */
	/* just keep in mind that making it too small ruins the point (nothing will fit into it), */
	/* and making it too large also ruins the point (so much for "low memory"!) */
	/* removing the cache entirely still gives 70% of the performance of a standard merge */

	/* also, if you change this to dynamically allocate a full-size buffer, */
	/* the algorithm seamlessly degenerates into a standard merge sort! */
	#define CACHE_SIZE 512
	const long cache_size = CACHE_SIZE;
//	Test cache[CACHE_SIZE];
//add
    Test* cache;
    long* Wtemp;
    int i=0;
    Range** levels_;
#ifdef TROI_WikiSort
    printf("TROI+ TROI_WikiSort\n");
#endif

#ifdef stack_func_WikiSort
    #ifdef TRACE_on
    printf("VAROI+ stack_func_WikiSort %p %p\n",STACK_BASE - stack_func_WikiSort_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)STACK_BASE - stack_func_WikiMerge_size +1,(unsigned long)STACK_BASE, COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif
#endif     


	Wtemp=(long*)malloc(sizeof(long)*13);
#ifdef heap_array_sort_temp
    #ifdef TRACE_on
    printf("VAROI+ heap_array_sort_temp %p %p\n",Wtemp, Wtemp + (sizeof(long)* 11) -1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)Wtemp,(unsigned long)(Wtemp + (sizeof(long)* 11) -1), COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif
#endif


//add    
    cache=(Test*)malloc(sizeof(Test)*CACHE_SIZE);
#ifdef heap_array_cache
    #ifdef TRACE_on
    printf("VAROI+ heap_array_cache %p %p\n",cache, cache + (sizeof(Test)* CACHE_SIZE) -1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)cache,(unsigned long)(cache + (sizeof(Test)* CACHE_SIZE) -1), COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif    
#endif


    levels_ =(Range**)malloc(sizeof(Range*)*4);
    for(i=0;i<4;i++)
    	levels_[i]=(Range*)malloc(sizeof(Range));
#ifdef heap_array_levels_
    #ifdef TRACE_on
    printf("VAROI+ heap_array_levels_ %p %p\n",levels_, levels_ + (sizeof(Range)* 4) -1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)levels_,(unsigned long)(levels_ + (sizeof(Range)* 4) -1), COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif        
#endif


	/* if there are 32 or fewer items, just insertion sort the entire array */
	if (size <= 32) {
		InsertionSort(array, MakeRange(0, size), compare);
    free(levels_);
#ifdef heap_array_levels_
    #ifdef TRACE_on
    printf("VAROI- heap_array_levels_ %p %p\n",levels_, levels_ + (sizeof(Range)* 4) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)levels_,(unsigned long)(levels_ + (sizeof(Range)* 4) -1), WRITE_BACK);
    #endif        
#endif


        free(Wtemp);
#ifdef heap_array_sort_temp
    #ifdef TRACE_on
    printf("VAROI- heap_array_sort_temp %p %p\n",Wtemp, Wtemp + (sizeof(long)* 11) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)Wtemp,(unsigned long)(Wtemp + (sizeof(long)* 11) -1), WRITE_BACK);
    #endif
#endif


//add
        free(cache);
#ifdef heap_array_cache
    #ifdef TRACE_on
    printf("VAROI- heap_array_cache %p %p\n",cache, cache + (sizeof(Test)* CACHE_SIZE) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)cache,(unsigned long)(cache + (sizeof(Test)* CACHE_SIZE) -1), WRITE_BACK);
    #endif    
#endif


#ifdef stack_func_WikiSort
    #ifdef TRACE_on
    printf("VAROI- stack_func_WikiSort %p %p\n",STACK_BASE - stack_func_WikiSort_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)STACK_BASE - stack_func_WikiMerge_size +1,(unsigned long)STACK_BASE,WRITE_BACK);
    #endif
#endif      


#ifdef TROI_WikiSort
    printf("TROI- TROI_WikiSort\n");
#endif


		return;
	}
	/* calculate how to scale the Wtemp[0] value to the range within the array */
	/* (this is essentially fixed-point math, where we manually check for and handle overflow) */
	Wtemp[7] = FloorPowerOfTwo(size);
	Wtemp[8] = Wtemp[7]/16;
	Wtemp[9] = size % Wtemp[8];
	Wtemp[10] = size/Wtemp[8];

	/* first insertion sort everything the lowest level, which is 16-31 items at a time */
	Wtemp[6] = 0; Wtemp[5] = 0;
	while (Wtemp[6] < size) {
		Wtemp[2] = Wtemp[6];

		Wtemp[6] += Wtemp[10];
		Wtemp[5] += Wtemp[9];
		if (Wtemp[5] >= Wtemp[8]) { Wtemp[5] -= Wtemp[8]; Wtemp[6] += 1; }

		Wtemp[4] = Wtemp[6];

		InsertionSort(array, MakeRange(Wtemp[2], Wtemp[4]), compare);
	}
	/* then merge sort the higher levels, which can be 32-63, 64-127, 128-255, etc. */
	for (Wtemp[1] = 16; Wtemp[1] < Wtemp[7]; Wtemp[1] += Wtemp[1]) {
		Wtemp[11] = sqrt(Wtemp[10]);
		Wtemp[12] = Wtemp[10]/Wtemp[11] + 1;

		/* as an optimization, we really only need to pull out an internal buffer once for each level of merges */
		/* after that we can reuse the same buffer over and over, then redistribute it when we're finished with this level */
		*(levels_[0]) = MakeRange(0, 0);
		*(levels_[1]) = MakeRange(0, 0);
		*(levels_[2]) = MakeRange(0, 0);
		*(levels_[3]) = MakeRange(0, 0);
		Wtemp[6] = Wtemp[5] = 0;
		while (Wtemp[6] < size) {
			Wtemp[2] = Wtemp[6];

			Wtemp[6] += Wtemp[10];
			Wtemp[5] += Wtemp[9];
			if (Wtemp[5] >= Wtemp[8]) { Wtemp[5] -= Wtemp[8]; Wtemp[6] += 1; }

			Wtemp[3] = Wtemp[6];

			Wtemp[6] += Wtemp[10];
			Wtemp[5] += Wtemp[9];
			if (Wtemp[5] >= Wtemp[8]) { Wtemp[5] -= Wtemp[8]; Wtemp[6] += 1; }

			Wtemp[4] = Wtemp[6];

			if (compare(array[Wtemp[4] - 1], array[Wtemp[2]])) {
				/* the two ranges are in reverse order, so a simple rotation should fix it */
				Rotate(array, Wtemp[3] - Wtemp[2], MakeRange(Wtemp[2],Wtemp[4]), cache, cache_size);
			} else if (compare(array[Wtemp[3]], array[Wtemp[3] - 1])) {
				Range bufferA, bufferB, buffer1, buffer2, blockA, blockB, firstA, lastA, lastB;
				long indexA, minA, findA;
				Test min_value;

				/* these two ranges weren't already in order, so we'll need to merge them! */
				Range A = MakeRange(Wtemp[2], Wtemp[3]), B = MakeRange(Wtemp[3], Wtemp[4]);

				/* try to fill up two buffers with unique values in ascending order */
				if (Range_length(A) <= cache_size) {
					memcpy(&cache[0], &array[A.start], Range_length(A) * sizeof(array[0]));
					WikiMerge(array, MakeRange(0, 0), A, B, compare, cache, cache_size);
					continue;
				}

				if (Range_length(*(levels_[0])) > 0) {
					/* reuse the buffers we found in a previous iteration */
					bufferA = MakeRange(A.start, A.start);
					bufferB = MakeRange(B.end, B.end);
					buffer1 = *(levels_[0]);
					buffer2 = *(levels_[1]);

				} else {
					long count, length;

					/* the first item is always going to be the first unique value, so let's.start searching at the next Wtemp[0] */
					count = 1;
					for (buffer1.start = A.start + 1; buffer1.start < A.end; buffer1.start++)
						if (compare(array[buffer1.start - 1], array[buffer1.start]) || compare(array[buffer1.start], array[buffer1.start - 1]))
							if (++count == Wtemp[12])
								break;
					buffer1.end = buffer1.start + count;

					/* if the size of each block fits into the cache, we only need one buffer for tagging the A blocks */
					/* this is because the other buffer is used as a swap space for merging the A blocks into the B values that follow it, */
					/* but we can just use the cache as the buffer instead. this skips some memmoves and an insertion sort */
					if (Wtemp[12] <= cache_size) {
						buffer2 = MakeRange(A.start, A.start);

						if (Range_length(buffer1) == Wtemp[12]) {
							/* we found enough values for the buffer in A */
							bufferA = MakeRange(buffer1.start, buffer1.start + Wtemp[12]);
							bufferB = MakeRange(B.end, B.end);
							buffer1 = MakeRange(A.start, A.start + Wtemp[12]);

						} else {
							/* we were unable to find enough unique values in A, so try B */
							bufferA = MakeRange(buffer1.start, buffer1.start);
							buffer1 = MakeRange(A.start, A.start);

							/* the last value is guaranteed to be the first unique value we encounter, so we can.start searching at the next Wtemp[0] */
							count = 1;
							for (buffer1.start = B.end - 2; buffer1.start >= B.start; buffer1.start--)
								if (compare(array[buffer1.start], array[buffer1.start + 1]) || compare(array[buffer1.start + 1], array[buffer1.start]))
									if (++count == Wtemp[12])
										break;
							buffer1.end = buffer1.start + count;

							if (Range_length(buffer1) == Wtemp[12]) {
								bufferB = MakeRange(buffer1.start, buffer1.start + Wtemp[12]);
								buffer1 = MakeRange(B.end - Wtemp[12], B.end);
							}
						}
					} else {
						/* the first item of the second buffer isn't guaranteed to be the first unique value, so we need to find the first unique item too */
						count = 0;
						for (buffer2.start = buffer1.start + 1; buffer2.start < A.end; buffer2.start++)
							if (compare(array[buffer2.start - 1], array[buffer2.start]) || compare(array[buffer2.start], array[buffer2.start - 1]))
								if (++count == Wtemp[12])
									break;
						buffer2.end = buffer2.start + count;

						if (Range_length(buffer2) == Wtemp[12]) {
							/* we found enough values for both buffers in A */
							bufferA = MakeRange(buffer2.start, buffer2.start + Wtemp[12] * 2);
							bufferB = MakeRange(B.end, B.end);
							buffer1 = MakeRange(A.start, A.start + Wtemp[12]);
							buffer2 = MakeRange(A.start + Wtemp[12], A.start + Wtemp[12] * 2);

						} else if (Range_length(buffer1) == Wtemp[12]) {
							/* we found enough values for one buffer in A, so we'll need to find one buffer in B */
							bufferA = MakeRange(buffer1.start, buffer1.start + Wtemp[12]);
							buffer1 = MakeRange(A.start, A.start + Wtemp[12]);

							/* like before, the last value is guaranteed to be the first unique value we encounter, so we can.start searching at the next Wtemp[0] */
							count = 1;
							for (buffer2.start = B.end - 2; buffer2.start >= B.start; buffer2.start--)
								if (compare(array[buffer2.start], array[buffer2.start + 1]) || compare(array[buffer2.start + 1], array[buffer2.start]))
									if (++count == Wtemp[12])
										break;
							buffer2.end = buffer2.start + count;

							if (Range_length(buffer2) == Wtemp[12]) {
								bufferB = MakeRange(buffer2.start, buffer2.start + Wtemp[12]);
								buffer2 = MakeRange(B.end - Wtemp[12], B.end);

							} else buffer1.end = buffer1.start; /* failure */
						} else {
							/* we were unable to find a single buffer in A, so we'll need to find two buffers in B */
							count = 1;
							for (buffer1.start = B.end - 2; buffer1.start >= B.start; buffer1.start--)
								if (compare(array[buffer1.start], array[buffer1.start + 1]) || compare(array[buffer1.start + 1], array[buffer1.start]))
									if (++count == Wtemp[12])
										break;
							buffer1.end = buffer1.start + count;

							count = 0;
							for (buffer2.start = buffer1.start - 1; buffer2.start >= B.start; buffer2.start--)
								if (compare(array[buffer2.start], array[buffer2.start + 1]) || compare(array[buffer2.start + 1], array[buffer2.start]))
									if (++count == Wtemp[12])
										break;
							buffer2.end = buffer2.start + count;

							if (Range_length(buffer2) == Wtemp[12]) {
								bufferA = MakeRange(A.start, A.start);
								bufferB = MakeRange(buffer2.start, buffer2.start + Wtemp[12] * 2);
								buffer1 = MakeRange(B.end - Wtemp[12], B.end);
								buffer2 = MakeRange(buffer1.start - Wtemp[12], buffer1.start);

							} else buffer1.end = buffer1.start; /* failure */
						}
					}

					if (Range_length(buffer1) < Wtemp[12]) {
						/* we failed to fill both buffers with unique values, which implies we're merging two subarrays with a lot of the same values repeated */
						/* we can use this knowledge to write a merge operation that is optimized for arrays of repeating values */
						while (Range_length(A) > 0 && Range_length(B) > 0) {
							/* find the first place in B where the first item in A needs to be inserted */
							Wtemp[3] = BinaryFirst(array, A.start, B, compare);

							/* rotate A into place */
							long amount = Wtemp[3] - A.end;
							Rotate(array, -amount, MakeRange(A.start, Wtemp[3]), cache, cache_size);

							/* calculate the new A and B ranges */
							B.start = Wtemp[3];
							A = MakeRange(BinaryLast(array, A.start + amount, A, compare), B.start);
						}

						continue;
					}

					/* move the unique values to the.start of A if needed */
					length = Range_length(bufferA);
					count = 0;
					for (Wtemp[0] = bufferA.start; count < length; Wtemp[0]--) {
						if (Wtemp[0] == A.start || compare(array[Wtemp[0] - 1], array[Wtemp[0]]) || compare(array[Wtemp[0]], array[Wtemp[0] - 1])) {
							Rotate(array, -count, MakeRange(Wtemp[0] + 1, bufferA.start + 1), cache, cache_size);
							bufferA.start = Wtemp[0] + count; count++;
						}
					}
					bufferA = MakeRange(A.start, A.start + length);

					/* move the unique values to the.end of B if needed */
					length = Range_length(bufferB);
					count = 0;
					for (Wtemp[0] = bufferB.start; count < length; Wtemp[0]++) {
						if (Wtemp[0] == B.end - 1 || compare(array[Wtemp[0]], array[Wtemp[0] + 1]) || compare(array[Wtemp[0] + 1], array[Wtemp[0]])) {
							Rotate(array, count, MakeRange(bufferB.start, Wtemp[0]), cache, cache_size);
							bufferB.start = Wtemp[0] - count; count++;
						}
					}
					bufferB = MakeRange(B.end - length, B.end);

					/* reuse these buffers next time! */
					*(levels_[0]) = buffer1;
					*(levels_[1]) = buffer2;
					*(levels_[2]) = bufferA;
					*(levels_[3]) = bufferB;
				}

				/* break the remainder of A into blocks. firstA is the uneven-sized first A block */
				blockA = MakeRange(bufferA.end, A.end);
				firstA = MakeRange(bufferA.end, bufferA.end + Range_length(blockA) % Wtemp[11]);

				/* swap the second value of each A block with the value in buffer1 */
				Wtemp[0] = 0;
				for (indexA = firstA.end + 1; indexA < blockA.end; Wtemp[0]++, indexA += Wtemp[11])
					Swap(array[buffer1.start + Wtemp[0]], array[indexA]);

				/* Wtemp[2] rolling the A blocks through the B blocks! */
				/* whenever we leave an A block behind, we'll need to merge the previous A block with any B blocks that follow it, so track that information as well */
				lastA = firstA;
				lastB = MakeRange(0, 0);
				blockB = MakeRange(B.start, B.start + Min(Wtemp[11], Range_length(B) - Range_length(bufferB)));
				blockA.start += Range_length(firstA);

				minA = blockA.start;
				min_value = array[minA];
				indexA = 0;

				if (Range_length(lastA) <= cache_size)
					memcpy(&cache[0], &array[lastA.start], Range_length(lastA) * sizeof(array[0]));
				else
					BlockSwap(array, lastA.start, buffer2.start, Range_length(lastA));

				while (true) {
					/* if there's a previous B block and the first value of the minimum A block is <= the last value of the previous B block */
					if ((Range_length(lastB) > 0 && !compare(array[lastB.end - 1], min_value)) || Range_length(blockB) == 0) {
						/* figure out where to split the previous B block, and rotate it at the split */
						long B_split = BinaryFirst(array, minA, lastB, compare);
						long B_remaining = lastB.end - B_split;

						/* swap the minimum A block to the beginning of the rolling A blocks */
						BlockSwap(array, blockA.start, minA, Wtemp[11]);

						/* we need to swap the second item of the previous A block back with its original value, which is stored in buffer1 */
						/* since the firstA block did not have its value swapped out, we need to make sure the previous A block is not unevenly sized */
						Swap(array[blockA.start + 1], array[buffer1.start + indexA++]);

						/* locally merge the previous A block with the B values that follow it, using the buffer as swap space */
						WikiMerge(array, buffer2, lastA, MakeRange(lastA.end, B_split), compare, cache, cache_size);

						/* copy the previous A block into the cache or buffer2, since that's where we need it to be when we go to merge it anyway */
						if (Wtemp[11] <= cache_size)
							memcpy(&cache[0], &array[blockA.start], Wtemp[11] * sizeof(array[0]));
						else
							BlockSwap(array, blockA.start, buffer2.start, Wtemp[11]);

						/* this is equivalent to rotating, but faster */
						/* the area normally taken up by the A block is either the contents of buffer2, or data we don't need anymore since we memcopied it */
						/* either way, we don't need to retain the order of those items, so instead of rotating we can just block swap B to where it belongs */
						BlockSwap(array, B_split, blockA.start + Wtemp[11] - B_remaining, B_remaining);

						/* now we need to update the ranges and stuff */
						lastA = MakeRange(blockA.start - B_remaining, blockA.start - B_remaining + Wtemp[11]);
						lastB = MakeRange(lastA.end, lastA.end + B_remaining);
						blockA.start += Wtemp[11];
						if (Range_length(blockA) == 0)
							break;

						/* search the second value of the remaining A blocks to find the new minimum A block (that's why we wrote unique values to them!) */
						minA = blockA.start + 1;
						for (findA = minA + Wtemp[11]; findA < blockA.end; findA += Wtemp[11])
							if (compare(array[findA], array[minA])) minA = findA;
						minA = minA - 1; /* decrement once to get back to the.start of that A block */
						min_value = array[minA];

					} else if (Range_length(blockB) < Wtemp[11]) {
						/* move the last B block, which is unevenly sized, to before the remaining A blocks, by using a rotation */
						/* (using the cache is disabled since we have the contents of the previous A block in it!) */
						Rotate(array, -Range_length(blockB), MakeRange(blockA.start, blockB.end), cache, 0);
						lastB = MakeRange(blockA.start, blockA.start + Range_length(blockB));
						blockA.start += Range_length(blockB);
						blockA.end += Range_length(blockB);
						minA += Range_length(blockB);
						blockB.end = blockB.start;

					} else {
						/* roll the leftmost A block to the.end by swapping it with the next B block */
						BlockSwap(array, blockA.start, blockB.start, Wtemp[11]);
						lastB = MakeRange(blockA.start, blockA.start + Wtemp[11]);
						if (minA == blockA.start)
							minA = blockA.end;

						blockA.start += Wtemp[11];
						blockA.end += Wtemp[11];
						blockB.start += Wtemp[11];
						blockB.end += Wtemp[11];
						if (blockB.end > bufferB.start)
							blockB.end = bufferB.start;
					}
				}

				/* merge the last A block with the remaining B blocks */
				WikiMerge(array, buffer2, lastA, MakeRange(lastA.end, B.end - Range_length(bufferB)), compare, cache, cache_size);
			}
		}
		if (Range_length(*(levels_[0])) > 0) {
			long level_start;

			/* when we're finished with this step we should have b1 b2 left over, where one of the buffers is all jumbled up */
			/* insertion sort the jumbled up buffer, then redistribute them back into the array using the opposite process used for creating the buffer */
			InsertionSort(array, *(levels_[1]), compare);

			/* redistribute bufferA back into the array */
			level_start = (*(levels_[2])).start;
			for (Wtemp[0] = (*(levels_[2])).end; Range_length(*(levels_[2])) > 0; Wtemp[0]++) {
				if (Wtemp[0] == (*(levels_[3])).start || !compare(array[Wtemp[0]], array[(*(levels_[2])).start])) {
					long amount = Wtemp[0] - (*(levels_[2])).end;
					Rotate(array, -amount, MakeRange((*(levels_[2])).start, Wtemp[0]), cache, cache_size);
					(*(levels_[2])).start += (amount + 1);
					(*(levels_[2])).end += amount;
					Wtemp[0]--;
				}
			}

			/* redistribute bufferB back into the array */
			for (Wtemp[0] = (*(levels_[3])).start; Range_length(*(levels_[3])) > 0; Wtemp[0]--) {
				if (Wtemp[0] == level_start || !compare(array[(*(levels_[3])).end - 1], array[Wtemp[0] - 1])) {
					long amount = (*(levels_[3])).start - Wtemp[0];
					Rotate(array, amount, MakeRange(Wtemp[0], ((*(levels_[3])).end)), cache, cache_size);
					(*(levels_[3])).start -= amount;
					(*(levels_[3])).end -= (amount + 1);
					Wtemp[0]++;
				}
			}
		}

		Wtemp[10] += Wtemp[10];
		Wtemp[9] += Wtemp[9];
		if (Wtemp[9] >= Wtemp[8]) {
			Wtemp[9] -= Wtemp[8];
			Wtemp[10] += 1;
		}
	}
    free(levels_);
#ifdef heap_array_levels_
    #ifdef TRACE_on
    printf("VAROI- heap_array_levels_ %p %p\n",levels_, levels_ + (sizeof(Range)* 4) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)levels_,(unsigned long)(levels_ + (sizeof(Range)* 4) -1), WRITE_BACK);
    #endif        
#endif
    free(Wtemp);
#ifdef heap_array_sort_temp
    #ifdef TRACE_on
    printf("VAROI- heap_array_sort_temp %p %p\n",Wtemp, Wtemp + (sizeof(long)* 11) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)Wtemp,(unsigned long)(Wtemp + (sizeof(long)* 11) -1), WRITE_BACK);
    #endif
#endif
    free(cache);

#ifdef heap_array_cache
    #ifdef TRACE_on
    printf("VAROI- heap_array_cache %p %p\n",cache, cache + (sizeof(Test)* CACHE_SIZE) -1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)cache,(unsigned long)(cache + (sizeof(Test)* CACHE_SIZE) -1), WRITE_BACK);
    #endif    
#endif

#ifdef stack_func_WikiSort
    #ifdef TRACE_on
    printf("VAROI- stack_func_WikiSort %p %p\n",STACK_BASE - stack_func_WikiSort_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)STACK_BASE - stack_func_WikiMerge_size +1,(unsigned long)STACK_BASE,WRITE_BACK);
    #endif
#endif      
	#undef CACHE_SIZE
#ifdef TROI_WikiSort
    printf("TROI- TROI_WikiSort\n");
#endif

}

long max_size = 30000;
//original ones;
//Test array1[400];

//add
Test* array1;
int benchmark(Test* item) {
	//long total, index, test_case;
	long* total;
	long* index;
	long* test_case;
	Comparison compare = TestCompare;

#ifdef TROI_benchmark
	printf("TROI+ TROI_benchmark\n");
#endif
#ifdef stack_func_benchmark
    #ifdef TRACE_on
    printf("VAROI+ stack_func_benchmark %p %p\n",STACK_BASE - stack_func_benchmark_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)(STACK_BASE - stack_func_benchmark_size +1),(unsigned long)(STACK_BASE), COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif
#endif
	/* initialize the random-number generator */
	srand(0);
	/*srand(10141985);*/ /* in case you want the same random numbers */
	total=(long*)malloc(sizeof(long));
#ifdef heap_array_total
    #ifdef TRACE_on
    printf("VAROI+ heap_array_total %p %p\n",total,total+(sizeof(long))-1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)total,(unsigned long)(total+(sizeof(long))-1), COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif      
#endif
	index=(long*)malloc(sizeof(index));
#ifdef heap_array_index
    #ifdef TRACE_on
    printf("VAROI+ heap_array_index %p %p\n",index,index+(sizeof(long))-1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)index,(unsigned long)(index+(sizeof(long))-1), COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif
#endif
	test_case=(long*)malloc(sizeof(test_case));
#ifdef heap_array_test_case
    #ifdef TRACE_on
    printf("VAROI+ heap_array_test_case %p %p\n",test_case,test_case+(sizeof(long))-1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)test_case,(unsigned long)(test_case+(sizeof(long))-1), COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif
#endif
	(*total) = max_size;
	for ((*test_case) = 0; (*test_case) < 6; (*test_case)++) {

		for ((*index) = 0; (*index) < (*total); (*index)++) {
			if((*index)==0)
				(*item).value = 1000 + rand() * 1.0/RAND_MAX * 4;
			if((*index)==1)
				(*item).value = (rand() * 1.0/RAND_MAX <= 0.9) ? index : (index - 2);
			if((*index)==2)
				(*item).value = 1000;
			if((*index)==3)
				(*item).value = 1000 + rand() * 1.0/RAND_MAX * 4;
			if((*index)==4)
				(*item).value = (rand() * 1.0/RAND_MAX <= 0.9) ? index : (index - 2);
			if((*index)==5)
				(*item).value = 1000;

			(*item).index = (*index);

			array1[(*index)] = (*item);
		}
		WikiSort(array1, (*total), compare);
	}
	free(test_case);
#ifdef heap_array_test_case
    #ifdef TRACE_on
    printf("VAROI- heap_array_test_case %p %p\n",test_case,test_case+(sizeof(long))-1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)test_case,(unsigned long)(test_case+(sizeof(long))-1),WRITE_BACK);
    #endif
#endif
	free(index);
#ifdef heap_array_index
    #ifdef TRACE_on
    printf("VAROI- heap_array_index %p %p\n",index,index+(sizeof(long))-1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)index,(unsigned long)(index+(sizeof(long))-1),WRITE_BACK);
    #endif
#endif
	free(total);
#ifdef heap_array_total
    #ifdef TRACE_on
    printf("VAROI+ heap_array_total %p %p\n",total,total+(sizeof(long))-1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)total,(unsigned long)(total+(sizeof(long))-1),WRITE_BACK);
    #endif      
#endif

#ifdef stack_func_benchmark
    #ifdef TRACE_on
    printf("VAROI- stack_func_benchmark %p %p\n",STACK_BASE - stack_func_benchmark_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)(STACK_BASE - stack_func_benchmark_size +1),(unsigned long)(STACK_BASE), WRITE_BACK);
    #endif
#endif

#ifdef TROI_benchmark
	printf("TROI- TROI_benchmark\n");
#endif
	return 0;
}

int main(int argc, char* argv[]){

#ifdef TROI_main
    printf("TROI+ TROI_main\n");
#endif
#ifdef stack_func_benchmark
    #ifdef TRACE_on
    printf("VAROI+ stack_func_main %p %p\n",STACK_BASE - stack_func_main_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)(STACK_BASE - stack_func_main_size +1),(unsigned long)(STACK_BASE), COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif
#endif
    Test* item;
    //add
    array1=(Test*)malloc(sizeof(Test)*max_size);
    item = (Test*)malloc(sizeof(Test));
#ifdef heap_array_item
    #ifdef TRACE_on
    printf("VAROI+ heap_array_item %p %p\n",item,item+(sizeof(Test))-1);
    #endif
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)item,(unsigned long)(item+(sizeof(Test))-1), COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif
#endif
#ifdef heap_array_array1
    #ifdef TRACE_on
    printf("VAROI+ heap_array_array1 %p %p\n",array1,array1+(sizeof(Test)*max_size)-1);
    #endif 
    #ifdef SPM_on
    SPM_ALLOC((unsigned long)array1,(unsigned long)(array1+(sizeof(Test)*max_size)-1), COPY,MAX_IMPORTANCE,HIGH_PRIORITY);
    #endif
#endif
    benchmark(item);    
    //add
    free(item);
    free(array1);
#ifdef heap_array_item
    #ifdef TRACE_on
    printf("VAROI- heap_array_item %p %p\n",item,item+(sizeof(Test))-1);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)item,(unsigned long)(item+(sizeof(Test))-1), WRITE_BACK);
    #endif
#endif
#ifdef heap_array_array1
    #ifdef TRACE_on
    printf("VAROI- heap_array_array1 %p %p\n",array1,array1+(sizeof(Test)*max_size)-1);
    #endif 
    #ifdef SPM_on
    SPM_FREE((unsigned long)array1,(unsigned long)(array1+(sizeof(Test)*max_size)-1), WRITE_BACK);
    #endif
#endif
#ifdef stack_func_benchmark
    #ifdef TRACE_on
    printf("VAROI- stack_func_main %p %p\n",STACK_BASE - stack_func_main_size +1 , STACK_BASE);
    #endif
    #ifdef SPM_on
    SPM_FREE((unsigned long)(STACK_BASE - stack_func_main_size +1),(unsigned long)(STACK_BASE),WRITE_BACK);
    #endif
#endif
#ifdef TROI_main
    printf("TROI- TROI_main\n");
#endif
    return 1;
}

