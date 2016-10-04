/*
 * patricia.c
 *
 * Patricia trie implementation.
 *
 * Functions for inserting nodes, removing nodes, and searching in
 * a Patricia trie designed for IP addresses and netmasks.  A
 * head node must be created with (key,mask) = (0,0).
 *
 * NOTE: The fact that we keep multiple masks per node makes this
 *       more complicated/computationally expensive then a standard
 *       trie.  This is because we need to do longest prefix matching,
 *       which is useful for computer networks, but not as useful
 *       elsewhere.
 *
 * Matthew Smart <mcsmart@eecs.umich.edu>
 *
 * Copyright (c) 2000
 * The Regents of the University of Michigan
 * All rights reserved
 *
 * $Id: patricia.c,v 1.1.1.1 2000/11/06 19:53:17 mguthaus Exp $
 */

#include <stdlib.h>	/* free(), malloc() */
#include <string.h>	/* bcopy() */
#include "patricia.h"
#include "spm_management.h"

/*
 * Private function used to return whether
 * or not bit 'i' is set in 'key'.
 */
static __inline
unsigned long
bit(int i, unsigned long key)
{
	return key & (1 << (31-i));
}


/*
 * Count the number of masks (and therefore entries)
 * in the Patricia trie.
 */
static int
pat_count(struct ptree *t, int b)
{
	int count;
	
	if (t->p_b <= b) return 0;

	count = t->p_mlen;
	
	count += pat_count(t->p_left,  t->p_b);
	count += pat_count(t->p_right, t->p_b);

	return count;
}


/*
 * Private function used for inserting a node recursively.
 */
static struct ptree *
insertR(struct ptree *h, struct ptree *n, int d, struct ptree *p)
{
#ifdef TROI_insertR
    printf("TROI+ TROI_insertR\n")
#endif
#ifdef stack_func_insertR
    printf("VAROI+ stack_func_insertR %p %p\n",__builtin_frame_address(0) - stack_func_insertR_size +! , __builtin_frame_address(0));
#endif

	if ((h->p_b >= d) || (h->p_b <= p->p_b)) {
		n->p_b = d;
		n->p_left = bit(d, n->p_key) ? h : n;
		n->p_right = bit(d, n->p_key) ? n : h;
#ifdef stack_func_insertR
    printf("VAROI- stack_func_insertR %p %p\n",__builtin_frame_address(0) - stack_func_insertR_size +! , __builtin_frame_address(0));
#endif

#ifdef TROI_insertR
    printf("TROI- TROI_insertR\n");
#endif
		return n;
	}

	if (bit(h->p_b, n->p_key))
		h->p_right = insertR(h->p_right, n, d, h);
	else
		h->p_left = insertR(h->p_left, n, d, h);
#ifdef stack_func_insertR
    printf("VAROI- stack_func_insertR %p %p\n",__builtin_frame_address(0) - stack_func_insertR_size +! , __builtin_frame_address(0));
#endif

#ifdef TROI_insertR
    printf("TROI- TROI_insertR\n");
#endif

	return h;
}


/*
 * Patricia trie insert.
 *
 * 1) Go down to leaf.
 * 2) Determine longest prefix match with leaf node.
 * 3) Insert new internal node at appropriate location and
 *    attach new external node.
 */
struct ptree *
pat_insert(struct ptree *n, struct ptree *head)
{
	struct ptree *t;
	struct ptree_mask *buf, *pm;
	int i, copied;
#ifdef TROI_pat_insert
    printf("TROI+ TROI_pat_insert\n");
#endif

#ifdef stack_func_pat_insert
    printf("VAROI+ stack_func_pat_insert %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_insert_size +1, __builtin_frame_address(0));
#endif
	if (!head || !n || !n->p_m)
    {
#ifdef stack_func_pat_insert
    printf("VAROI- stack_func_pat_insert %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_insert_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_insert
    printf("TROI- TROI_pat_insert\n");
#endif
		return 0;
    }
	/*
	 * Make sure the key matches the mask.
	 */
	n->p_key &= n->p_m->pm_mask;

	/*
	 * Find closest matching leaf node.
	 */
	t = head;
	do {
		i = t->p_b;
		t = bit(t->p_b, n->p_key) ? t->p_right : t->p_left;
	} while (i < t->p_b);

	/*
	 * If the keys are the same we need to check the masks.
	 */
	if (n->p_key == t->p_key) {
		/*
		 * If we have a duplicate mask, replace the entry
		 * with the new one.
		 */
		for (i=0; i < t->p_mlen; i++) {
			if (n->p_m->pm_mask == t->p_m[i].pm_mask) {
				t->p_m[i].pm_data = n->p_m->pm_data;
                free(n->p_m->pm_data);
#ifdef heap_array_main_p_pm_data
    printf("VAROI- heap_array_main_p_pm_data %p %p\n",n->p_m->pm_data , n->p_m->pm_data+sizeof(struct MyNode)-1);
#endif
		        free(n->p_m);
#ifdef heap_array_main_p_pm
    printf("VAROI- heap_array_main_p_pm %p %p\n",n->p_m, n->p_m+sizeof(struct ptree_mask)-1);
#endif
		        free(n);
#ifdef heap_array_main_p
    printf("VAROI- heap_array_main_p %p %p\n",n,n+sizeof(struct ptree)-1);
#endif

				n = 0;
#ifdef stack_func_pat_insert
    printf("VAROI- stack_func_pat_insert %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_insert_size +1, __builtin_frame_address(0));
#endif


#ifdef TROI_pat_insert
    printf("TROI- TROI_pat_insert\n");
#endif
				return t;
			}
		}
		
		/*
		 * Allocate space for a new set of masks.
		 */
		buf = (struct ptree_mask *)malloc(
		       sizeof(struct ptree_mask)*(t->p_mlen+1));
		/*
		 * Insert the new mask in the proper order from least
		 * to greatest mask.
		 */
		copied = 0;
		for (i=0, pm=buf; i < t->p_mlen; pm++) {
			if (n->p_m->pm_mask > t->p_m[i].pm_mask) {
				bcopy(t->p_m + i, pm, sizeof(struct ptree_mask));
				i++;
			}
			else {
				bcopy(n->p_m, pm, sizeof(struct ptree_mask));
				n->p_m->pm_mask = 0xffffffff;
				copied = 1;
			}
		}
		if (!copied) {
			bcopy(n->p_m, pm, sizeof(struct ptree_mask));
		}
 
        free(n->p_m->pm_data);
#ifdef heap_array_main_p_pm_data
    printf("VAROI- heap_array_main_p_pm_data %p %p\n",n->p_m->pm_data , n->p_m->pm_data+sizeof(struct MyNode)-1);
#endif
		free(n->p_m);
#ifdef heap_array_main_p_pm
    printf("VAROI- heap_array_main_p_pm %p %p\n",n->p_m, n->p_m+sizeof(struct ptree_mask)-1);
#endif
		free(n);
#ifdef heap_array_main_p
    printf("VAROI- heap_array_main_p %p %p\n",n,n+sizeof(struct ptree)-1);
#endif
		n = 0;
		t->p_mlen++;

		/*
		 * Free old masks and point to new ones.
		 */
        
        free(t->p_m->pm_data);
#ifdef heap_array_main_phead_pm_data
    printf("VAROI- heap_array_main_phead_pm_data %p %p\n",t->p_m->pm_data,t->p_m->pm_data + sizeof(struct MyNode)-1);
#endif
		free(t->p_m);
#ifdef heap_array_main_phead_pm
    printf("VAROI- heap_array_main_phead_pm %p %p\n",t->p_m,t->p_m+sizeof(ptree_mask)-1);
#endif
		t->p_m = buf;

#ifdef stack_func_pat_insert
    printf("VAROI- stack_func_pat_insert %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_insert_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_insert
    printf("TROI- TROI_pat_insert\n");
#endif		
		return t;
	}

	/*
	 * Find the first bit that differs.
	 */
	for (i=1; i < 32 && bit(i, n->p_key) == bit(i, t->p_key); i++); 

	/*
	 * Recursive step.
	 */
	if (bit(head->p_b, n->p_key))
		head->p_right = insertR(head->p_right, n, i, head);
	else
		head->p_left = insertR(head->p_left, n, i, head);

#ifdef stack_func_pat_insert
    printf("VAROI- stack_func_pat_insert %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_insert_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_insert
    printf("TROI- TROI_pat_insert\n");
#endif	
	return n;
}


/*
 * Remove an entry given a key in a Patricia trie.
 */
int
pat_remove(struct ptree *n, struct ptree *head)
{
	struct ptree *p, *g, *pt, *pp, *t;
	struct ptree_mask *buf, *pm;
	int i;
#ifdef TROI_pat_remove
    printf("TROI+ TROI_pat_remove\n");
#endif
#ifdef stack_func_pat_remove
    printf("VAROI+ stack_func_pat_remove %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_remove_size +1, __builtin_frame_address(0));
#endif

	if (!n || !n->p_m || !t)
    {
#ifdef stack_func_pat_remove
    printf("VAROI- stack_func_pat_remove %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_remove_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_remove
    printf("TROI- TROI_pat_remove\n");
#endif
		return 0;
    }
	/*
	 * Search for the target node, while keeping track of the
	 * parent and grandparent nodes.
	 */
	g = p = t = head;
	do {
		i = t->p_b;
		g = p;
		p = t;
		t = bit(t->p_b, n->p_key) ? t->p_right : t->p_left;
	} while (i < t->p_b);
	/*
	 * For removal, we need an exact match.
	 */
	if (t->p_key != n->p_key)
    {
#ifdef stack_func_pat_remove
    printf("VAROI- stack_func_pat_remove %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_remove_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_remove
    printf("TROI- TROI_pat_remove\n");
#endif
	return 0;
    }
	/*
	 * If there is only 1 mask, we can remove the entire node.
	 */
	if (t->p_mlen == 1) {
		/*
		 * Don't allow removal of the default entry.
		 */
		if (t->p_b == 0)
        {
#ifdef stack_func_pat_remove
    printf("VAROI- stack_func_pat_remove %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_remove_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_remove
    printf("TROI- TROI_pat_remove\n");
#endif
			return 0;
		}
		/*
		 * Must match on the mask.
		 */
		if (t->p_m->pm_mask != n->p_m->pm_mask)
        {
#ifdef stack_func_pat_remove
    printf("VAROI- stack_func_pat_remove %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_remove_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_remove
    printf("TROI- TROI_pat_remove\n");
#endif

			return 0;
		}
		/*
		 * Search for the node that points to the parent, so
		 * we can make sure it doesn't get lost.
		 */
		pp = pt = p;
		do {
			i = pt->p_b;
			pp = pt;
			pt = bit(pt->p_b, p->p_key) ? pt->p_right : pt->p_left;
		} while (i < pt->p_b);

		if (bit(pp->p_b, p->p_key))
			pp->p_right = t;
		else
			pp->p_left = t;

		/*
		 * Point the grandparent to the proper node.
		 */
		if (bit(g->p_b, n->p_key))
			g->p_right = bit(p->p_b, n->p_key) ?
				p->p_left : p->p_right;
		else
			g->p_left = bit(p->p_b, n->p_key) ?
				p->p_left : p->p_right;
	
		/*
		 * Delete the target's data and copy in its parent's
		 * data, but not the bit value.
		 */
		free(t->p_m->pm_data);
#ifdef heap_array_main_phead_pm_data
    printf("VAROI- heap_array_main_phead_pm_data %p %p\n",t->p_m->pm_data,t->p_m->pm_data + sizeof(struct MyNode)-1);
#endif
		free(t->p_m);
#ifdef heap_array_main_phead_pm
    printf("VAROI- heap_array_main_phead_pm %p %p\n",t->p_m,t->p_m+sizeof(ptree_mask)-1);
#endif

		if (t != p) {
			t->p_key = p->p_key;
			t->p_m = p->p_m;
			t->p_mlen = p->p_mlen;
		}
		free(p);
#ifdef heap_array_main_phead
    printf("VAROI- heap_array_main_phead_pm %p %p\n",p,p+sizeof(ptree)-1);
#endif

#ifdef stack_func_pat_remove
    printf("VAROI- stack_func_pat_remove %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_remove_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_remove
    printf("TROI- TROI_pat_remove\n");
#endif
		return 1;
	}

	/*
	 * Multiple masks, so we need to find the one to remove.
	 * Return if we don't match on any of them.
	 */
	for (i=0; i < t->p_mlen; i++)
		if (n->p_m->pm_mask == t->p_m[i].pm_mask)
			break;
	if (i >= t->p_mlen)
    {
#ifdef stack_func_pat_remove
    printf("VAROI- stack_func_pat_remove %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_remove_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_remove
    printf("TROI- TROI_pat_remove\n");
#endif
		return 0;
    }	
	/*
	 * Allocate space for a new set of masks.
	 */
	buf = (struct ptree_mask *)malloc(
	       sizeof(struct ptree_mask)*(t->p_mlen-1));

	for (i=0, pm=buf; i < t->p_mlen; i++) {
		if (n->p_m->pm_mask != t->p_m[i].pm_mask) {
			bcopy(t->p_m + i, pm++, sizeof(struct ptree_mask));
		}
	}
		
	/*
	 * Free old masks and point to new ones.
	 */
	t->p_mlen--;
	free(t->p_m);
	t->p_m = buf;

#ifdef stack_func_pat_remove
    printf("VAROI- stack_func_pat_remove %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_remove_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_remove
    printf("TROI- TROI_pat_remove\n");
#endif
	return 1;
}


/*
 * Find an entry given a key in a Patricia trie.
 */
struct ptree *
pat_search(unsigned long key, struct ptree *head)
{
	struct ptree *p = 0, *t = head;
	int i;
#ifdef TROI_pat_search
    printf("TROI+ TROI_pat_search\n");
#endif
#ifdef stack_func_pat_search
    printf("VAROI+ stack_func_pat_search %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_serarch_size +1, __builtin_frame_address(0));
#endif

	if (!t)
    {
#ifdef stack_func_pat_search
    printf("VAROI- stack_func_pat_search %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_serarch_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_search
    printf("TROI- TROI_pat_search\n");
#endif
		return 0;
    }
	/*
	 * Find closest matching leaf node.
	 */
	do {
		/*
		 * Keep track of most complete match so far.
		 */
		if (t->p_key == (key & t->p_m->pm_mask)) {
			p = t;
		}
		
		i = t->p_b;
		t = bit(t->p_b, key) ? t->p_right : t->p_left;
	} while (i < t->p_b);

	/*
	 * Compare keys (and masks) to see if this
	 * is really the node we want.
	 */
#ifdef stack_func_pat_search
    printf("VAROI- stack_func_pat_search %p %p\n",__builtin_frame_address(0) \
            - stack_func_pat_serarch_size +1, __builtin_frame_address(0));
#endif

#ifdef TROI_pat_search
    printf("TROI- TROI_pat_search\n");
#endif
	return (t->p_key == (key & t->p_m->pm_mask)) ? t : p;
}
