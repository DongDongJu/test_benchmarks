/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: /home/mguthaus/.cvsroot/mibench/telecomm/gsm/src/gsm_destroy.c,v 1.1.1.1 2000/11/06 19:54:25 mguthaus Exp $ */


#include "gsm.h"
#include "private.h"
#include "config.h"
#include "spm_management.h"
#include "proto.h"


#ifdef	HAS_STDLIB_H
#	include	<stdlib.h>
#else
#	ifdef	HAS_MALLOC_H
#		include 	<malloc.h>
#	else
		extern void free();
#	endif
#endif

void gsm_destroy P1((S), gsm S)
{
#ifdef TROI_gsm_destroy
	printf("TROI+ TROI_gsm_destroy\n");
#endif
#ifdef stack_func_gsm_destroy
	printf("VAROI+ stack_func_gsm_destroy %p %p\n", STACK_BASE - stack_func_gsm_destroy_size +1 , STACK_BASE);
#endif
	if (S){
		free((char *)S);
#ifdef heap_array_gsm_state
	printf("VAROI- heap_array_gsm_state %p %p ",S, S + sizeof(struct gsm_state) -1);
#endif
	}


#ifdef stack_func_gsm_destroy
	printf("VAROI- stack_func_gsm_destroy %p %p\n", STACK_BASE - stack_func_gsm_destroy_size +1 , STACK_BASE);
#endif
#ifdef TROI_gsm_destroy
	printf("TROI- TROI_gsm_destroy\n");
#endif

}
