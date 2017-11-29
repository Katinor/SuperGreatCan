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

#if (1 != EXCLUDE_DLIST)

#include <stdlib.h>

#include "itf_ubinos/itf/ubinos.h"
#include "itf_ubinos/itf/ubiclib_dlist.h"

dlist_elmt_pt dlist_elmt_create(void * data, void * data2) {
	dlist_elmt_pt elmt;

	elmt = malloc(sizeof(dlist_elmt_t));
	if (NULL != elmt) {
		elmt->prev	= NULL;
		elmt->next	= NULL;
		elmt->list	= NULL;
		elmt->data	= data;
		elmt->data2	= data2;
	}

	return elmt;
}

int dlist_elmt_delete(dlist_elmt_pt elmt) {
	if (NULL == elmt) {
		return -2;
	}

	free(elmt);

	return 0;
}

int dlist_create(dlist_pt * dlist_p) {
	dlist_pt dlist;

	if (NULL == dlist_p) {
		logme("dlist_p is NULL\r\n");
		return -2;
	}

	dlist = malloc(sizeof(dlist_t));
	if (NULL == dlist) {
		logme("malloc(...) fail\r\n");
		return -1;
	}

	dlist->count	= 0;
	dlist->head		= NULL;
	dlist->tail		= NULL;
	dlist->cur		= NULL;
	dlist->data		= NULL;

	*dlist_p = dlist;

	return 0;
}

int dlist_delete(dlist_pt * dlist_p) {
	dlist_pt dlist;

	if (NULL == dlist_p) {
		logme("dlist_p is NULL\r\n");
		return -2;
	}

	if (NULL == *dlist_p) {
		logme("*dlist_p is NULL\r\n");
		return -2;
	}

	dlist = *dlist_p;

	if (0 != dlist->count) {
		logme("**dlist_p is not empty\r\n");
		return -2;
	}

	dlist->count	= 0;
	dlist->head		= NULL;
	dlist->tail		= NULL;
	dlist->cur		= NULL;
	dlist->data		= NULL;

	free(dlist);

	*dlist_p = NULL;

	return 0;
}

int dlist_insertprev(dlist_pt dlist, dlist_elmt_pt ref, dlist_elmt_pt elmt) {
	if (NULL == dlist) {
		logme("dlist is NULL\r\n");
		return -2;
	}

	if (NULL == elmt) {
		logme("elmt is NULL\r\n");
		return -4;
	}

	elmt->next = ref;

	if (NULL == ref) {
		elmt->prev = dlist->tail;
		dlist->tail = elmt;
	}
	else {
		elmt->prev = ref->prev;
		ref->prev = elmt;
	}

	if (NULL == elmt->prev) {
		dlist->head = elmt;
	}
	else {
		elmt->prev->next = elmt;
	}

	dlist->count++;

	elmt->list = dlist;

	return 0;
}

int dlist_insertnext(dlist_pt dlist, dlist_elmt_pt ref, dlist_elmt_pt elmt) {
	if (NULL == dlist) {
		logme("dlist is NULL\r\n");
		return -2;
	}

	if (NULL == elmt) {
		logme("elmt is NULL\r\n");
		return -4;
	}

	elmt->prev = ref;

	if (NULL == ref) {
		elmt->next = dlist->head;
		dlist->head = elmt;
	}
	else {
		elmt->next = ref->next;
		ref->next = elmt;
	}

	if (NULL == elmt->next) {
		dlist->tail = elmt;
	}
	else {
		elmt->next->prev = elmt;
	}

	dlist->count++;

	elmt->list = dlist;

	return 0;
}

int dlist_remove(dlist_elmt_pt elmt) {
	dlist_pt dlist;

	if (NULL == elmt) {
		logme("elmt is NULL\r\n");
		return -2;
	}

	dlist = (dlist_pt) (elmt->list);

	if (NULL == dlist) {
		logme("dlist is NULL\r\n");
		return -2;
	}

	///
	if (elmt == dlist->cur) {
		dlist->cur = elmt->prev;
	}

	if (NULL == elmt->prev) {
		dlist->head = elmt->next;
	}
	else {
		elmt->prev->next = elmt->next;
	}

	if (NULL == elmt->next) {
		dlist->tail = elmt->prev;
	}
	else {
		elmt->next->prev = elmt->prev;
	}

	dlist->count--;

	elmt->prev = NULL;
	elmt->next = NULL;
	elmt->list = NULL;
	///

	return 0;
}

dlist_elmt_pt dlist_getcurnext(dlist_pt dlist) {
	if (NULL == dlist->cur || NULL == dlist->cur->next) {
		dlist->cur = dlist->head;
	}
	else {
		dlist->cur = dlist->cur->next;
	}

	return dlist->cur;
}

int dlist_find(dlist_pt dlist, dlist_elmt_pt * elmt_p, void * data) {
	dlist_elmt_pt elmt;

	if (NULL == dlist) {
		logme("dlist is NULL\r\n");
		return -2;
	}

	if (NULL == elmt_p) {
		logme("elmt_p is NULL\r\n");
		return -3;
	}

	elmt = dlist->head;
	for (;;) {
		if (NULL == elmt || data == elmt->data) {
			break;
		}
		elmt = elmt->next;
	}

	*elmt_p = elmt;

	if (NULL == elmt) {
		return DLIST_ERR__NOTEXIST;
	}

	return 0;
}

int dlist_findandremove(dlist_pt dlist, dlist_elmt_pt * elmt_p, void * data) {
	dlist_elmt_pt elmt;

	if (NULL == dlist) {
		logme("dlist is NULL\r\n");
		return -2;
	}

	elmt = dlist->head;
	for (;;) {
		if (NULL == elmt || data == elmt->data) {
			break;
		}
		elmt = elmt->next;
	}

	if (NULL != elmt_p) {
		*elmt_p = elmt;
	}

	if (NULL == elmt) {
		return DLIST_ERR__NOTEXIST;
	}

	///
	if (elmt == dlist->cur) {
		dlist->cur = elmt->prev;
	}

	if (NULL == elmt->prev) {
		dlist->head = elmt->next;
	}
	else {
		elmt->prev->next = elmt->next;
	}

	if (NULL == elmt->next) {
		dlist->tail = elmt->prev;
	}
	else {
		elmt->next->prev = elmt->prev;
	}

	dlist->count--;

	elmt->prev = NULL;
	elmt->next = NULL;
	elmt->list = NULL;
	///

	return 0;
}

#endif /* (1 != EXCLUDE_DLIST) */
