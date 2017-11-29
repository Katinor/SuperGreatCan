/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the lib_ubiclib component of the Ubinos.

  GNU General Public License Usage
  This file may be used under the terms of the GNU
  General Public License version 3.0 as published by the Free Software
  Foundation and appearing in the file license_gpl3.txt included in the
  packaging of this file. Please review the following information to
  ensure the GNU General Public License version 3.0 requirements will be
  met: http://www.gnu.org/copyleft/gpl.html.

  GNU Lesser General Public License Usage
  Alternatively, this file may be used under the terms of the GNU Lesser
  General Public License version 2.1 as published by the Free Software
  Foundation and appearing in the file license_lgpl.txt included in the
  packaging of this file. Please review the following information to
  ensure the GNU Lesser General Public License version 2.1 requirements
  will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.

  Commercial Usage
  Alternatively, licensees holding valid commercial licenses may
  use this file in accordance with the commercial license agreement
  provided with the software or, alternatively, in accordance with the
  terms contained in a written agreement between you and rightful owner.
*/

#include "../ubiconfig.h"

#if (1 != EXCLUDE_HEAP)

#include <stdlib.h>
#include <stdio.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/type.h"
#include "itf_ubinos/itf/ubiclib.h"
#include "itf_ubinos/itf/ubiclib_edlist.h"
#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubik.h"
#include "itf_ubinos/itf/ubik_task.h"
#include "itf_ubinos/itf/ubik_mutex.h"

#undef	LOGM_CATEGORY
#define LOGM_CATEGORY 				LOGM_CATEGORY__HEAP
#undef	LOGM_TAG

#if !defined(EXCLUDE_HEAP_BOUNDARY_CHECK)
	#if (1 != HEAP_BOUNDARY_CHECK)
		#define EXCLUDE_HEAP_BOUNDARY_CHECK	1
	#else
		#define EXCLUDE_HEAP_BOUNDARY_CHECK	0
	#endif
#endif

#if (1 != EXCLUDE_HEAP_BOUNDARY_CHECK)

	#if		(2 == INT_SIZE)
		#define HEAP_BOUNDARY_PATTERN		0xA5B4
	#elif	(4 == INT_SIZE)
		#define HEAP_BOUNDARY_PATTERN		0xA5B4C75A
	#else
		#error "Unsupported int size"
	#endif

	#define BOUNDARY_SIZE					INT_SIZE

	typedef struct _heap_block_t {
		unsigned int			boundary;
		struct _heap_block_t *	next;
		unsigned int			size;
	} heap_block_t;

	#define HEAP_BLOCK_HEADER_SIZE		MEM_ALIGN(sizeof(heap_block_t))

	#define set_header_boundary(block)			\
		(*((unsigned int *)(	(unsigned int) block														)) = HEAP_BOUNDARY_PATTERN)
	#define set_top_boundary(block)				\
		(*((unsigned int *)(	(unsigned int) block + HEAP_BLOCK_HEADER_SIZE								)) = HEAP_BOUNDARY_PATTERN)
	#define set_bottom_boundary(block)			\
		(*((unsigned int *)(	(unsigned int) block + HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE + block->size	)) = HEAP_BOUNDARY_PATTERN)
	#define set_boundary(block) { 				\
		set_header_boundary(block);				\
		set_top_boundary(block);				\
		set_bottom_boundary(block);				\
	}

	#define check_header_boundary(block)			\
		((HEAP_BOUNDARY_PATTERN == *((unsigned int *)(	(unsigned int) block														))) ? 1 : 0)
	#define check_top_boundary(block)				\
		((HEAP_BOUNDARY_PATTERN == *((unsigned int *)(	(unsigned int) block + HEAP_BLOCK_HEADER_SIZE								))) ? 1 : 0)
	#define check_bottom_boundary(block) 			\
		((HEAP_BOUNDARY_PATTERN == *((unsigned int *)(	(unsigned int) block + HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE + block->size	))) ? 1 : 0)
	#define check_boundary(block)					\
		(check_header_boundary(block) && check_top_boundary(block) && check_bottom_boundary(block))

	#define check_boundary_and_abort(block) {												\
		if (!check_header_boundary(block)) {												\
			logme("memory(0x%08X) is polluted (head boundary)",								\
					(unsigned int) block + HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE);			\
			bsp_abortsystem();																\
		}																					\
		else if(!check_top_boundary(block)) {												\
			logme("memory(0x%08X) is polluted (top boundary)", 								\
					(unsigned int) block + HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE);			\
			bsp_abortsystem();																\
		}																					\
		else if(!check_bottom_boundary(block)) {											\
			logme("memory(0x%08X) is polluted (bottom boundary)", 							\
					(unsigned int) block + HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE);			\
			bsp_abortsystem();																\
		}																					\
	}

#else

	#define BOUNDARY_SIZE						0

	typedef struct _heap_block_t {
		struct _heap_block_t *	next;
		unsigned int			size;
	} heap_block_t;

	#define HEAP_BLOCK_HEADER_SIZE		MEM_ALIGN(sizeof(heap_block_t))

	#define set_header_boundary(block)
	#define set_top_boundary(block)
	#define set_bottom_boundary(block)
	#define set_boundary(block)

	#define check_header_boundary(block)		1
	#define check_top_boundary(block)			1
	#define check_bottom_boundary(block)		1
	#define check_boundary(block)				1

	#define check_boundary_and_abort(block)

#endif /* (1 != EXCLUDE_HEAP_BOUNDARY_CHECK) */

typedef heap_block_t * heap_block_pt;

typedef struct {
	unsigned int			count;
	unsigned int			count_max;
	heap_block_pt			head;
	heap_block_pt			tail;
} heap_blist_t;

typedef heap_blist_t * heap_blist_pt;

static int heap_blist_init(heap_blist_pt list);
static int heap_blist_removenext(heap_blist_pt list, heap_block_pt block);
static int heap_blist_insertnext(heap_blist_pt list, heap_block_pt block, heap_block_pt newblock);
static int heap_blist_inserttail(heap_blist_pt list, heap_block_pt block);
static int heap_blist_insertsizeorder(heap_blist_pt list, heap_block_pt block);

typedef struct {
	unsigned int		type			:  8;
	unsigned int		valid			:  1;
	unsigned int		reserved		:  7;
	unsigned int		reserved2		: 16;

	unsigned int 		addr;
	unsigned int		size;

	unsigned int		allocsize;
	unsigned int		allocsize_max;
	unsigned int		alloccount;
	unsigned int		alloccount_max;

	unsigned int		nend;
	heap_blist_t		nfblist;
	heap_blist_t		nablist;

	unsigned int		nallocsize;
	unsigned int 		nallocsize_max;

	unsigned int		rend;
	heap_blist_t		rfblist;
	heap_blist_t		rablist;

	unsigned int		rallocsize;
	unsigned int 		rallocsize_max;

	mutex_pt			mutex;
} heap_t;

typedef heap_t * _heap_pt;

static int heap_init(_heap_pt heap, unsigned int addr, unsigned int size);

static int 		heap_mergeblock_bestfits(_heap_pt heap, heap_block_pt block, int direction);
static void * 	heap_allocblock_bestfits(_heap_pt heap, unsigned int size, int direction);
static int 		heap_freeblock_bestfits(_heap_pt heap, void * ptr);

#define heap_assert(heap) {													\
	if (NULL == heap) {														\
		logme("heap is NULL");												\
		return -2;															\
	}																		\
	if (OBJTYPE__UBICLIB_HEAP != heap->type) {								\
		logme("heap is not OBJTYPE__UBICLIB_HEAP");							\
		return -2;															\
	}																		\
	if (1 != heap->valid) {													\
		logme("heap is not valid");											\
		return -2;															\
	}																		\
}

#define heap_assert_return_null(heap) {										\
	if (NULL == heap) {														\
		logme("heap is NULL");												\
		return NULL;														\
	}																		\
	if (OBJTYPE__UBICLIB_HEAP != heap->type) {								\
		logme("heap is not OBJTYPE__UBICLIB_HEAP");							\
		return NULL;														\
	}																		\
	if (1 != heap->valid) {													\
		logme("heap is not valid");											\
		return NULL;														\
	}																		\
}

_heap_pt _ubiclib_heap = NULL;

#if (0 == UBI_INCLUDE_STDLIB)

heap_t _ubiclib_defaultheap;

int ubiclib_heap_comp_init(unsigned int addr, unsigned int size) {
	#define LOGM_TAG	"ubiclib_heap_comp_init"
	int r;

	if (0 >= size) {
		logme("size is 0");
		return -3;
	}

	r = heap_init(&_ubiclib_defaultheap, addr, size);
	if (0 != r) {
		logme("fail at heap_init");
		return -1;
	}

	_ubiclib_heap = &_ubiclib_defaultheap;

	return 0;
	#undef LOGM_TAG
}


int ubiclib_heap_comp_init_reent(void) {
	#define LOGM_TAG	"ubiclib_heap_comp_init_reent"
	int r;

	if (NULL == _ubiclib_heap) {
		logme("_ubiclib_heap is NULL");
		return -1;
	}
	if (OBJTYPE__UBICLIB_HEAP != _ubiclib_heap->type) {
		logme("_ubiclib_heap is not OBJTYPE__UBICLIB_HEAP");
		return -1;
	}
	if (1 != _ubiclib_heap->valid) {
		logme("_ubiclib_heap is not valid");
		return -1;
	}

	r = mutex_create(&_ubiclib_heap->mutex);
	if (0 != r) {
		logme("fail at mutex_create");
		return -1;
	}

	return 0;
	#undef LOGM_TAG
}

void * malloc(size_t size) {
	#define LOGM_TAG	"malloc"
	if (0 == ubik_isrt()) {
		return heap_allocblock_bestfits(_ubiclib_heap, SIZETOUINT(size), 0);
	}
	else {
		return heap_allocblock_bestfits(_ubiclib_heap, SIZETOUINT(size), 1);
	}
	#undef LOGM_TAG
}

void * mallocn(size_t size) {
	#define LOGM_TAG	"mallocn"
	return heap_allocblock_bestfits(_ubiclib_heap, SIZETOUINT(size), 0);
	#undef LOGM_TAG
}

void * mallocr(size_t size) {
	#define LOGM_TAG	"mallocr"
	return heap_allocblock_bestfits(_ubiclib_heap, SIZETOUINT(size), 1);
	#undef LOGM_TAG
}

void free(void * ptr) {
	#define LOGM_TAG	"free"
	heap_free((heap_pt) _ubiclib_heap, ptr);
	#undef LOGM_TAG
}

#else

void * mallocn(size_t size) {
	#define LOGM_TAG	"mallocn"
	return malloc(size);
	#undef LOGM_TAG
}

void * mallocr(size_t size) {
	#define LOGM_TAG	"mallocr"
	return malloc(size);
	#undef LOGM_TAG
}

#endif /* (0 == UBI_INCLUDE_STDLIB) */

int heap_create(heap_pt * heap_p, unsigned int addr, unsigned int size) {
	#define LOGM_TAG	"heap_create"
	_heap_pt heap;
	int r;

	if (NULL == heap_p) {
		logme("heap_p is NULL");
		return -2;
	}

	heap = malloc(sizeof(heap_t));
	if (NULL == heap) {
		logme("fail at malloc");
		return -1;
	}

	r = heap_init(heap, addr, size);
	if (0 != r) {
		logme("fail at heap_init");
		free(heap);
		return -1;
	}

	r = mutex_create(&heap->mutex);
	if (0 != r) {
		logme("fail at mutex_create");
		free(heap);
		return -1;
	}

	*heap_p = (heap_pt) heap;

	return 0;
	#undef LOGM_TAG
}

int heap_delete(heap_pt * heap_p) {
	#define LOGM_TAG	"heap_delete"
	_heap_pt heap;
	int r;

	if (NULL == heap_p) {
		logme("heap_p is NULL");
		return -2;
	}

	heap = (_heap_pt) *heap_p;
	heap_assert(heap);

	if (NULL != heap->mutex) {
		r = mutex_lock(heap->mutex);
		if (0 != r) {
			logme("fail at mutex_lock");
			return -1;
		}
	}

	r = 0;

	heap->valid		= 0;
	heap->type		= 0;

	*heap_p = NULL;
	if (NULL != heap->mutex) {
		r = mutex_delete(&heap->mutex);
		if (0 != r) {
			logme("fail at mutex_delete");
		}
	}
	free(heap);
	if (0 != r) {
		return -1;
	}

	return 0;
	#undef LOGM_TAG
}

void * heap_malloc(heap_pt heap, unsigned int size) {
	#define LOGM_TAG	"heap_malloc"
	if (0 == ubik_isrt()) {
		return heap_allocblock_bestfits((_heap_pt) heap, size, 0);
	}
	else {
		return heap_allocblock_bestfits((_heap_pt) heap, size, 1);
	}
	#undef LOGM_TAG
}

void * heap_mallocn(heap_pt heap, unsigned int size) {
	#define LOGM_TAG	"heap_mallocn"
	return heap_allocblock_bestfits((_heap_pt) heap, size, 0);
	#undef LOGM_TAG
}

void * heap_mallocr(heap_pt heap, unsigned int size) {
	#define LOGM_TAG	"heap_mallocr"
	return heap_allocblock_bestfits((_heap_pt) heap, size, 1);
	#undef LOGM_TAG
}

int heap_free(heap_pt heap, void * ptr) {
	#define LOGM_TAG	"heap_free"
	return heap_freeblock_bestfits((_heap_pt) heap, ptr);
	#undef LOGM_TAG
}

int heap_checkboundary(void * ptr) {
	#define LOGM_TAG	"heap_checkboundary"
	int r;
	heap_block_pt ab;

	if (NULL == ptr) {
		logme("ptr is NULL");
		return -2;
	}

	ab = (heap_block_pt) ((unsigned int) ptr - HEAP_BLOCK_HEADER_SIZE - BOUNDARY_SIZE);

	if (!check_top_boundary(ab)) {
		r = HEAP_ERR__POLLUTED_TOP;
	}
	else if (!check_bottom_boundary(ab)) {
		r = HEAP_ERR__POLLUTED_BOTTOM;
	}
	else if (!check_header_boundary(ab)) {
		r = HEAP_ERR__POLLUTED_HEADER;
	}
	else {
		r = 0;
	}

	return r;
	#undef LOGM_TAG
}

int heap_checkboundaryall(heap_pt _heap) {
	#define LOGM_TAG	"heap_checkboundaryall"
	int r;
	int i;
	_heap_pt heap = (_heap_pt) _heap;
	heap_blist_pt ablist = NULL;
	heap_blist_pt fblist = NULL;
	heap_block_pt mb = NULL;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (0 == ubik_iscrit() && NULL != heap->mutex) {
		r = mutex_lock(heap->mutex);
		if (0 != r) {
			logme("fail at mutex_lock");
			return -1;
		}
	}

	for (i=0; i<2; i++) {
		switch (i) {
		case 0: // regular
			ablist = &heap->nablist;
			fblist = &heap->nfblist;
			break;
		case 1: // reverse
			ablist = &heap->rablist;
			fblist = &heap->rfblist;
			break;
		}

		mb = ablist->head;
		for (;;) {
			if (NULL == mb) {
				break;
			}
			check_boundary_and_abort(mb);
			mb = mb->next;
		}

		mb = fblist->head;
		for (;;) {
			if (NULL == mb) {
				break;
			}
			check_boundary_and_abort(mb);
			mb = mb->next;
		}
	}

	if (0 == ubik_iscrit() && NULL != heap->mutex) {
		r = mutex_unlock(heap->mutex);
		if (0 != r) {
			logme("fail at mutex_unlock");
			return -1;
		}
	}

	return 0;
	#undef LOGM_TAG
}

int heap_getsize(void * ptr, unsigned int * size) {
	#define LOGM_TAG	"heap_getsize"
	heap_block_pt ab;

	if (NULL == ptr) {
		logme("ptr is NULL");
		return -2;
	}

	if (NULL == size) {
		logme("size is NULL");
		return -3;
	}

	ab = (heap_block_pt) ((unsigned int) ptr - HEAP_BLOCK_HEADER_SIZE - BOUNDARY_SIZE);

	check_boundary_and_abort(ab);

	*size = ab->size;

	return 0;
	#undef LOGM_TAG
}

int heap_getheapsize(heap_pt _heap, unsigned int * size_p) {
	#define LOGM_TAG	"heap_getheapsize"
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (NULL == size_p) {
		logme("size_p is NULL");
		return -3;
	}

	*size_p = heap->size;

	return 0;
	#undef LOGM_TAG
}

int heap_getallocsize(heap_pt _heap, unsigned int * size_p) {
	#define LOGM_TAG	"heap_getallocsize"
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (NULL == size_p) {
		logme("size_p is NULL");
		return -3;
	}

	*size_p = (heap->nallocsize + heap->rallocsize);

	return 0;
	#undef LOGM_TAG
}

int heap_getallocsize_ext(heap_pt _heap, unsigned int * nsize_p, unsigned int * rsize_p) {
	#define LOGM_TAG	"heap_getallocsize_ext"
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (NULL != nsize_p) {
		*nsize_p = heap->nallocsize;
	}

	if (NULL != rsize_p) {
		*rsize_p = heap->rallocsize;
	}

	return 0;
	#undef LOGM_TAG
}

int heap_getallocsizemax(heap_pt _heap, unsigned int * size_p) {
	#define LOGM_TAG	"heap_getallocsizemax"
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (NULL == size_p) {
		logme("size_p is NULL");
		return -3;
	}

	*size_p = heap->allocsize_max;

	return 0;
	#undef LOGM_TAG
}

int heap_getallocsizemax_ext(heap_pt _heap, unsigned int * nsize_p, unsigned int * rsize_p) {
	#define LOGM_TAG	"heap_getallocsizemax_ext"
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (NULL != nsize_p) {
		*nsize_p = heap->nallocsize_max;
	}

	if (NULL != rsize_p) {
		*rsize_p = heap->rallocsize_max;
	}

	return 0;
	#undef LOGM_TAG
}

int heap_getalloccount(heap_pt _heap, unsigned int * count_p) {
	#define LOGM_TAG	"heap_getalloccount"
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (NULL == count_p) {
		logme("count_p is NULL");
		return -3;
	}

	*count_p = (heap->nablist.count + heap->rablist.count);

	return 0;
	#undef LOGM_TAG
}

int heap_getalloccount_ext(heap_pt _heap, unsigned int * ncount_p, unsigned int * rcount_p) {
	#define LOGM_TAG	"heap_getalloccount_ext"
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (NULL != ncount_p) {
		*ncount_p = heap->nablist.count;
	}

	if (NULL != rcount_p) {
		*rcount_p = heap->rablist.count;
	}

	return 0;
	#undef LOGM_TAG
}

int heap_getalloccountmax(heap_pt _heap, unsigned int * count_p) {
	#define LOGM_TAG	"heap_getalloccountmax"
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (NULL == count_p) {
		logme("count_p is NULL");
		return -3;
	}

	*count_p = heap->alloccount_max;

	return 0;
	#undef LOGM_TAG
}

int heap_getalloccountmax_ext(heap_pt _heap, unsigned int * ncount_p, unsigned int * rcount_p) {
	#define LOGM_TAG	"heap_getalloccountmax_ext"
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (NULL != ncount_p) {
		*ncount_p = heap->nablist.count_max;
	}

	if (NULL != rcount_p) {
		*rcount_p = heap->rablist.count_max;
	}

	return 0;
	#undef LOGM_TAG
}

int heap_getblockoverhead(heap_pt _heap, unsigned int * overhead_p) {
	#define LOGM_TAG	"heap_getblockoverhead"
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (NULL == overhead_p) {
		logme("overhead_p is NULL");
		return -3;
	}

	*overhead_p = HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE * 2;

	return 0;
	#undef LOGM_TAG
}

#if (1 != EXCLUDE_HEAP_SPRINTHEAPINFO)
int heap_sprintheapinfo(heap_pt _heap, char * buf, int max) {
	#define LOGM_TAG	"heap_sprintheapinfo"
	_heap_pt heap = (_heap_pt) _heap;
	int r;
	unsigned int boh = 0;
	unsigned int size = 0;
	unsigned int count = 0;
	unsigned int total = 0;
	unsigned int nsize = 0;
	unsigned int ncount = 0;
	unsigned int rsize = 0;
	unsigned int rcount = 0;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	if (NULL == buf) {
		r = -2;
		goto end0;
	}

	if (max <= 0) {
		r = 0;
		goto end0;
	}

	r = heap_getheapsize((heap_pt) heap, &size);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	if (total < max) total += snprintf(&buf[total], max-total, "heap size                            : 0x%08x (%8d bytes)\r\n", size, size);

	r = heap_getblockoverhead((heap_pt) heap, &boh);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	if (total < max) total += snprintf(&buf[total], max-total, "block overhead                       : 0x%08x (%8d bytes)\r\n", boh, boh);

	if (total < max) total += snprintf(&buf[total], max-total, "\r\n");

	r = heap_getallocsize_ext((heap_pt) heap, &nsize, &rsize);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	size = nsize + rsize;
	if (total < max) total += snprintf(&buf[total], max-total, "allocated heap size       (total)    : 0x%08x (%8d bytes)\r\n", size, size);
	if (total < max) total += snprintf(&buf[total], max-total, "                          (normal)   : 0x%08x (%8d bytes)\r\n", nsize, nsize);
	if (total < max) total += snprintf(&buf[total], max-total, "                          (reverse)  : 0x%08x (%8d bytes)\r\n", rsize, rsize);

	r = heap_getalloccount_ext((heap_pt) heap, &ncount, &rcount);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	count = ncount + rcount;
	if (total < max) total += snprintf(&buf[total], max-total, "allocated block count     (total)    : 0x%08x (%8d bytes)\r\n", count, count);
	if (total < max) total += snprintf(&buf[total], max-total, "                          (normal)   : 0x%08x (%8d bytes)\r\n", ncount, ncount);
	if (total < max) total += snprintf(&buf[total], max-total, "                          (reverse)  : 0x%08x (%8d bytes)\r\n", rcount, rcount);

	size = size + (count * boh);
	if (total < max) total += snprintf(&buf[total], max-total, "ram usage (dynamic)                  : 0x%08x (%8d bytes)\r\n", size, size);

	if (total < max) total += snprintf(&buf[total], max-total, "\r\n");

	r = heap_getallocsizemax((heap_pt) heap, &size);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	r = heap_getallocsizemax_ext((heap_pt) heap, &nsize, &rsize);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	if (total < max) total += snprintf(&buf[total], max-total, "allocated heap size max   (total)    : 0x%08x (%8d bytes)\r\n", size, size);
	if (total < max) total += snprintf(&buf[total], max-total, "                          (normal)   : 0x%08x (%8d bytes)\r\n", nsize, nsize);
	if (total < max) total += snprintf(&buf[total], max-total, "                          (reverse)  : 0x%08x (%8d bytes)\r\n", rsize, rsize);

	r = heap_getalloccountmax((heap_pt) heap, &count);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	r = heap_getalloccountmax_ext((heap_pt) heap, &ncount, &rcount);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	if (total < max) total += snprintf(&buf[total], max-total, "allocated block count max (total)    : 0x%08x (%8d bytes)\r\n", count, count);
	if (total < max) total += snprintf(&buf[total], max-total, "                          (normal)   : 0x%08x (%8d bytes)\r\n", ncount, ncount);
	if (total < max) total += snprintf(&buf[total], max-total, "                          (reverse)  : 0x%08x (%8d bytes)\r\n", rcount, rcount);

	size = size + (count * boh);
	if (total < max) total += snprintf(&buf[total], max-total, "ram usage max (dynamic)              : 0x%08x (%8d bytes)\r\n", size, size);

	r = total;

end0:
	return r;
	#undef LOGM_TAG
}
#endif /* (1 != EXCLUDE_HEAP_SPRINTHEAPINFO) */

#if (1 != EXCLUDE_HEAP_PRINTHEAPINFO)
int heap_printheapinfo(heap_pt _heap) {
	#define LOGM_TAG	"heap_printheapinfo"
	_heap_pt heap = (_heap_pt) _heap;
	int r;
	unsigned int boh = 0;
	unsigned int size = 0;
	unsigned int count = 0;
	unsigned int nsize = 0;
	unsigned int ncount = 0;
	unsigned int rsize = 0;
	unsigned int rcount = 0;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_assert(heap);

	r = heap_getheapsize((heap_pt) heap, &size);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("heap size                            : 0x%08x (%8d bytes)\r\n", size, size);

	r = heap_getblockoverhead((heap_pt) heap, &boh);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("block overhead                       : 0x%08x (%8d bytes)\r\n", boh, boh);

	printf("\r\n");

	r = heap_getallocsize_ext((heap_pt) heap, &nsize, &rsize);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	size = nsize + rsize;
	printf("allocated heap size       (total)    : 0x%08x (%8d bytes)\r\n", size, size);
	printf("                          (normal)   : 0x%08x (%8d bytes)\r\n", nsize, nsize);
	printf("                          (reverse)  : 0x%08x (%8d bytes)\r\n", rsize, rsize);

	r = heap_getalloccount_ext((heap_pt) heap, &ncount, &rcount);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	count = ncount + rcount;
	printf("allocated block count     (total)    : 0x%08x (%8d      )\r\n", count, count);
	printf("                          (normal)   : 0x%08x (%8d      )\r\n", ncount, ncount);
	printf("                          (reverse)  : 0x%08x (%8d      )\r\n", rcount, rcount);

	size = size + (count * boh);
	printf("ram usage (dynamic)                  : 0x%08x (%8d bytes)\r\n", size, size);

	printf("\r\n");

	r = heap_getallocsizemax((heap_pt) heap, &size);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	r = heap_getallocsizemax_ext((heap_pt) heap, &nsize, &rsize);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("allocated heap size max   (total)    : 0x%08x (%8d bytes)\r\n", size, size);
	printf("                          (normal)   : 0x%08x (%8d bytes)\r\n", nsize, nsize);
	printf("                          (reverse)  : 0x%08x (%8d bytes)\r\n", rsize, rsize);

	r = heap_getalloccountmax((heap_pt) heap, &count);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	r = heap_getalloccountmax_ext((heap_pt) heap, &ncount, &rcount);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("allocated block count max (total)    : 0x%08x (%8d      )\r\n", count, count);
	printf("                          (normal)   : 0x%08x (%8d      )\r\n", ncount, ncount);
	printf("                          (reverse)  : 0x%08x (%8d      )\r\n", rcount, rcount);

	size = size + (count * boh);
	printf("ram usage max (dynamic)              : 0x%08x (%8d bytes)\r\n", size, size);

	r = 0;

end0:
	return r;
	#undef LOGM_TAG
}
#endif /* (1 != EXCLUDE_HEAP_PRINTHEAPINFO) */

static int heap_blist_init(heap_blist_pt list) {
	#define LOGM_TAG	"heap_blist_init"
	list->head		= NULL;
	list->tail		= NULL;
	list->count		= 0;
	list->count_max	= 0;

	return 0;
	#undef LOGM_TAG
}

static int heap_blist_removenext(heap_blist_pt list, heap_block_pt block) {
	#define LOGM_TAG	"heap_blist_removenext"
	if (0 == list->count) {
		return -1;
	}

	if (NULL == block) {
		list->head = list->head->next;

		if (1 == list->count) {
			list->tail = NULL;
		}
	}
	else {
		if (NULL == block->next) {
			return -1;
		}

		block->next = block->next->next;

		if (NULL == block->next) {
			list->tail = block;
		}
	}

	list->count--;

	return 0;
	#undef LOGM_TAG
}

static int heap_blist_insertnext(heap_blist_pt list, heap_block_pt block, heap_block_pt newblock) {
	#define LOGM_TAG	"heap_blist_insertnext"
	if (NULL == block) {
		newblock->next = list->head;
		list->head = newblock;

		if (0 == list->count) {
			list->tail = newblock;
		}
	}
	else {
		newblock->next = block->next;
		block->next = newblock;

		if (NULL == newblock->next) {
			list->tail = newblock;
		}
	}

	list->count++;
	if (list->count_max < list->count) {
		list->count_max = list->count;
	}

	return 0;
	#undef LOGM_TAG
}

static int heap_blist_inserttail(heap_blist_pt list, heap_block_pt block) {
	#define LOGM_TAG	"heap_blist_inserttail"
	block->next = NULL;

	if (0 == list->count) {
		list->head = block;
	}
	else {
		list->tail->next = block;
	}

	list->tail = block;

	list->count++;
	if (list->count_max < list->count) {
		list->count_max = list->count;
	}

	return 0;
	#undef LOGM_TAG
}

static int heap_blist_insertsizeorder(heap_blist_pt list, heap_block_pt block) {
	#define LOGM_TAG	"heap_blist_insertsizeorder"
	heap_block_pt pb;
	heap_block_pt cb;

	pb = NULL;
	cb = list->head;
	for (;;) {
		if (NULL == cb) {
			break;
		}
		check_boundary_and_abort(cb);
		if (cb->size > block->size) {
			break;
		}
		pb = cb;
		cb = cb->next;
	}

	heap_blist_insertnext(list, pb, block);

	return 0;
	#undef LOGM_TAG
}

static int heap_init(_heap_pt heap, unsigned int addr, unsigned int size) {
	#define LOGM_TAG	"heap_init"
	if (NULL == heap) {
		return -1;
	}

	heap->type				= OBJTYPE__UBICLIB_HEAP;
	heap->valid				= 0;
	heap->reserved			= 0;
	heap->reserved2			= 0;

	heap->addr				= addr;
	heap->size				= size;

	heap->allocsize			= 0;
	heap->allocsize_max		= 0;
	heap->alloccount		= 0;
	heap->alloccount_max	= 0;

	heap->nend				= addr;
	heap_blist_init(&heap->nfblist);
	heap_blist_init(&heap->nablist);

	heap->nallocsize		= 0;
	heap->nallocsize_max	= 0;

	heap->rend				= addr + size;
	heap_blist_init(&heap->rfblist);
	heap_blist_init(&heap->rablist);

	heap->rallocsize		= 0;
	heap->rallocsize_max	= 0;

	heap->mutex				= NULL;

	heap->valid				= 1;

	return 0;
	#undef LOGM_TAG
}

static int heap_freeblock_bestfits(_heap_pt heap, void * ptr) {
	#define LOGM_TAG	"heap_freeblock_bestfits"
	int r;
	int i;
	heap_block_pt pb = NULL;
	heap_block_pt ab = NULL;
	heap_blist_pt ablist = NULL;

	unsigned int asize;

	heap_assert(heap);

	if (NULL == ptr) {
		logme("ptr is NULL");
		return -3;
	}

	if (NULL != heap->mutex) {
		r = mutex_lock(heap->mutex);
		if (0 != r) {
			logme("fail at mutex_lock");
			return -1;
		}
	}

	for (i=0; i<2; i++) {
		switch (i) {
		case 0: // regular
			ablist = &heap->nablist;
			break;
		case 1: // reverse
			ablist = &heap->rablist;
			break;
		}

		pb = NULL;
		ab = ablist->head;
		for (;;) {
			if (NULL == ab) {
				break;
			}
			check_boundary_and_abort(ab);
			if (ptr == (void *) ((unsigned int)ab + HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE)) {
				break;
			}
			pb = ab;
			ab = ab->next;
		}

		if (NULL != ab) {
			asize = ab->size;

			heap_blist_removenext(ablist, pb);
			heap_mergeblock_bestfits(heap, ab, i);

			if (0 == i) {
				heap->nallocsize -= asize;
			}
			else {
				heap->rallocsize -= asize;
			}

			if (NULL != heap->mutex) {
				r = mutex_unlock(heap->mutex);
				if (0 != r) {
					logme("fail at mutex_unlock");
					return -1;
				}
			}

			return 0;
		}
	}

	if (NULL != heap->mutex) {
		r = mutex_unlock(heap->mutex);
		if (0 != r) {
			logme("fail at mutex_unlock");
			return -1;
		}
	}

	logme("ptr is wrong");
	return -3;
	#undef LOGM_TAG
}

static int heap_mergeblock_bestfits(_heap_pt heap, heap_block_pt block, int direction) {
	#define LOGM_TAG	"heap_mergeblock_bestfits"
	heap_block_pt pb;
	heap_block_pt cb;
	heap_blist_pt fblist;
	unsigned int tmp1;
	unsigned int tmp2;

	switch (direction) {
	case 0: // regular
		fblist = &heap->nfblist;
		break;
	case 1: // reverse
		fblist = &heap->rfblist;
		break;
	}

	pb = NULL;
	cb = fblist->head;
	for (;;) {
		if (NULL == cb) {
			break;
		}
		check_boundary_and_abort(cb);
		if (cb < block) {
			tmp1 = (unsigned int) cb + HEAP_BLOCK_HEADER_SIZE + cb->size + BOUNDARY_SIZE*2;
			tmp2 = (unsigned int) block;
			if (tmp1 == tmp2) {
				heap_blist_removenext(fblist, pb);
				cb->size += (HEAP_BLOCK_HEADER_SIZE + block->size + BOUNDARY_SIZE*2);
				block = cb;

				pb = NULL;
				cb = fblist->head;
				continue;
			}
		}
		else {
			tmp1 = (unsigned int) block + HEAP_BLOCK_HEADER_SIZE + block->size + BOUNDARY_SIZE*2;
			tmp2 = (unsigned int) cb;
			if (tmp1 == tmp2) {
				heap_blist_removenext(fblist, pb);
				block->size += (HEAP_BLOCK_HEADER_SIZE + cb->size + BOUNDARY_SIZE*2);

				pb = NULL;
				cb = fblist->head;
				continue;
			}
		}

		pb = cb;
		cb = cb->next;
	}

	switch (direction) {
	case 0: // regular
		if (((unsigned int) block + HEAP_BLOCK_HEADER_SIZE + block->size + BOUNDARY_SIZE*2) == heap->nend) {
			heap->nend -= (HEAP_BLOCK_HEADER_SIZE + block->size + BOUNDARY_SIZE*2);
			return 0;
		}
		break;
	case 1: // reverse
		if (((unsigned int) block) == heap->rend) {
			heap->rend += (HEAP_BLOCK_HEADER_SIZE + block->size + BOUNDARY_SIZE*2);
			return 0;
		}
		break;
	}

	set_boundary(block);
	heap_blist_insertsizeorder(fblist, block);

	return 0;
	#undef LOGM_TAG
}

static void * heap_allocblock_bestfits(_heap_pt heap, unsigned int size, int direction) {
	#define LOGM_TAG	"heap_allocblock_bestfits"
	int r;
	heap_block_pt pb;
	heap_block_pt fb;
	heap_block_pt nb;
	heap_blist_pt fblist;
	heap_blist_pt ablist;
	unsigned int asize;
	unsigned int aaddr;

	heap_assert_return_null(heap);

	if (0 >= size) {
		logme("size is wrong");
		return NULL;
	}

	if ((heap->size - (HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE * 2)) < size) {
		logmw("memory is not enough");
		return NULL;
	}

	if (NULL != heap->mutex) {
		r = mutex_lock(heap->mutex);
		if (0 != r) {
			logme("fail at mutex_lock");
			return NULL;
		}
	}

	asize  = MEM_ALIGN(size);

	switch (direction) {
	case 0: // regular
		fblist = &heap->nfblist;
		ablist = &heap->nablist;
		break;
	case 1: // reverse
		fblist = &heap->rfblist;
		ablist = &heap->rablist;
		break;
	}

	pb = NULL;
	fb = fblist->head;
	for (;;) {
		if (NULL == fb) {
			break;
		}
		check_boundary_and_abort(fb);
		if (asize <= fb->size) {
			break;
		}
		pb = fb;
		fb = fb->next;
	}

	if (NULL != fb) {
		heap_blist_removenext(fblist, pb);

		if ((HEAP_BLOCK_HEADER_SIZE + asize + BOUNDARY_SIZE*2) < fb->size) {
			switch (direction) {
			case 0: // regular
				nb = (heap_block_pt) (((unsigned int) fb) + HEAP_BLOCK_HEADER_SIZE + asize + BOUNDARY_SIZE*2);
				nb->size = fb->size - HEAP_BLOCK_HEADER_SIZE - asize - BOUNDARY_SIZE*2;
				fb->size = asize;
				break;
			case 1: // reverse
				nb = fb;
				fb = (heap_block_pt) (((unsigned int) fb) + fb->size - asize);
				fb->size = asize;
				nb->size = nb->size - HEAP_BLOCK_HEADER_SIZE - asize - BOUNDARY_SIZE*2;
				break;
			}
			set_boundary(nb);
			heap_blist_insertsizeorder(fblist, nb);
		}

		set_boundary(fb);
		heap_blist_inserttail(ablist, fb);
		aaddr = ((unsigned int) fb) + HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE;

		if (0 == direction) {
			heap->nallocsize += fb->size;
			if (heap->nallocsize_max < heap->nallocsize) {
				heap->nallocsize_max = heap->nallocsize;
			}
		}
		else {
			heap->rallocsize += fb->size;
			if (heap->rallocsize_max < heap->rallocsize) {
				heap->rallocsize_max = heap->rallocsize;
			}
		}
		heap->allocsize = heap->nallocsize + heap->rallocsize;
		if (heap->allocsize_max < heap->allocsize) {
			heap->allocsize_max = heap->allocsize;
		}
		heap->alloccount = heap->nablist.count + heap->rablist.count;
		if (heap->alloccount_max < heap->alloccount) {
			heap->alloccount_max = heap->alloccount;
		}
	}
	else {
		if ( (heap->nend + HEAP_BLOCK_HEADER_SIZE + asize  + BOUNDARY_SIZE*2) <= heap->rend ) {
			switch (direction) {
			case 0: // regular
				fb = (heap_block_pt) (heap->nend);
				fb->size = asize;
				heap->nend += (HEAP_BLOCK_HEADER_SIZE + asize + BOUNDARY_SIZE*2);
				break;
			case 1: // reverse
				heap->rend -= (HEAP_BLOCK_HEADER_SIZE + asize + BOUNDARY_SIZE*2);
				fb = (heap_block_pt) (heap->rend);
				fb->size = asize;
				break;
			}

			set_boundary(fb);
			heap_blist_inserttail(ablist, fb);
			aaddr = ((unsigned int) fb) + HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE;

			if (0 == direction) {
				heap->nallocsize += fb->size;
				if (heap->nallocsize_max < heap->nallocsize) {
					heap->nallocsize_max = heap->nallocsize;
				}
			}
			else {
				heap->rallocsize += fb->size;
				if (heap->rallocsize_max < heap->rallocsize) {
					heap->rallocsize_max = heap->rallocsize;
				}
			}
			heap->allocsize = heap->nallocsize + heap->rallocsize;
			if (heap->allocsize_max < heap->allocsize) {
				heap->allocsize_max = heap->allocsize;
			}
			heap->alloccount = heap->nablist.count + heap->rablist.count;
			if (heap->alloccount_max < heap->alloccount) {
				heap->alloccount_max = heap->alloccount;
			}
		}
		else {
			logmw("memory is not enough");
			aaddr = 0x0;
		}
	}

	if (NULL != heap->mutex) {
		r = mutex_unlock(heap->mutex);
		if (0 != r) {
			logme("fail at mutex_unlock");
		}
	}

	return (void *) aaddr;
	#undef LOGM_TAG
}

#else

#include "itf_ubinos/itf/ubiclib_heap.h"

int ubiclib_heap_comp_init(unsigned int addr, unsigned int size) {
	return HEAP_ERR__UNSUPPORTED;
}

int ubiclib_heap_comp_init_reent(void) {
	return HEAP_ERR__UNSUPPORTED;
}

int heap_printheapinfo(heap_pt _heap) {
	return HEAP_ERR__UNSUPPORTED;
}

#endif /* (1 != EXCLUDE_HEAP) */
