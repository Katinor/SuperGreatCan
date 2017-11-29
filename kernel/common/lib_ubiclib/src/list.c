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

#if (1 != EXCLUDE_LIST)

#include <stdlib.h>

#include "itf_ubinos/itf/ubinos.h"
#include "itf_ubinos/itf/ubiclib_list.h"

list_elmt_pt list_elmt_create(void * data) {
	list_elmt_pt elmt;

	elmt = malloc(sizeof(list_elmt_t));
	if (NULL != elmt) {
		elmt->next = NULL;
		elmt->data = data;
	}

	return elmt;
}

int list_elmt_delete(list_elmt_pt elmt) {
	if (NULL == elmt) {
		return -2;
	}

	free(elmt);

	return 0;
}

int list_create(list_pt * list_p) {
	list_pt list;

	if (NULL == list_p) {
		logme("list_p is NULL\r\n");
		return -2;
	}

	list = malloc(sizeof(list_t));
	if (NULL == list) {
		logme("malloc(...) fail\r\n");
		return -1;
	}

	list->count	= 0;
	list->head	= NULL;
	list->tail	= NULL;

	*list_p = list;

	return 0;
}

int list_delete(list_pt * list_p) {
	list_pt list;

	if (NULL == list_p) {
		logme("list_p is NULL\r\n");
		return -2;
	}

	if (NULL == *list_p) {
		logme("*list_p is NULL\r\n");
		return -2;
	}

	list = *list_p;

	if (0 != list->count) {
		logme("**list_p is not empty\r\n");
		return -2;
	}

	list->count	= 0;
	list->head	= NULL;
	list->tail	= NULL;

	free(list);

	*list_p = NULL;

	return 0;
}

int list_insertnext(list_pt list, list_elmt_pt ref, list_elmt_pt elmt) {
	if (NULL == list) {
		logme("list is NULL\r\n");
		return -2;
	}

	if (NULL == elmt) {
		logme("elmt is NULL\r\n");
		return -4;
	}

	if (NULL == ref) {
		elmt->next = list->head;
		list->head = elmt;
	}
	else {
		elmt->next = ref->next;
		ref->next = elmt;
	}

	if (NULL == elmt->next) {
		list->tail = elmt;
	}

	list->count++;

	return 0;
}

int list_inserttail(list_pt list, list_elmt_pt elmt) {
	if (NULL == list) {
		logme("list is NULL\r\n");
		return -2;
	}

	if (NULL == elmt) {
		logme("elmt is NULL\r\n");
		return -4;
	}

	elmt->next = NULL;

	if (NULL == list->tail) {
		list->head = elmt;
	}
	else {
		list->tail->next = elmt;
	}

	list->tail = elmt;

	list->count++;

	return 0;

}

int list_removenext(list_pt list, list_elmt_pt ref, list_elmt_pt * elmt_p) {
	list_elmt_pt elmt;

	if (NULL == list) {
		logme("list is NULL\r\n");
		return -2;
	}

	if (NULL == list->head) {
		logme("list is empty\r\n");
		return -1;
	}

	if (NULL != ref && NULL == ref->next) {
		logme("next is NULL\r\n");
		return -1;
	}

	///
	if (NULL == ref) {
		elmt = list->head;
		list->head = elmt->next;

		if (NULL == elmt->next) {
			list->tail = NULL;
		}
	}
	else {
		elmt = ref->next;
		ref->next = elmt->next;

		if (NULL == ref->next) {
			list->tail = ref;
		}
	}

	list->count--;

	elmt->next = NULL;
	///

	if (NULL != elmt_p) {
		*elmt_p = elmt;
	}

	return 0;
}

int list_find(list_pt list, list_elmt_pt * elmt_p, void * data) {
	list_elmt_pt elmt;

	if (NULL == list) {
		logme("list is NULL\r\n");
		return -2;
	}
	if (NULL == elmt_p) {
		logme("elmt_p is NULL\r\n");
		return -3;
	}

	elmt = list->head;
	for (;;) {
		if (NULL == elmt || data == elmt->data) {
			break;
		}
		elmt = elmt->next;
	}

	*elmt_p = elmt;

	if (NULL == elmt) {
		return LIST_ERR__NOTEXIST;
	}

	return 0;
}

int list_findandremove(list_pt list, list_elmt_pt * elmt_p, void * data) {
	list_elmt_pt ref;
	list_elmt_pt elmt;

	if (NULL == list) {
		logme("list is NULL\r\n");
		return -2;
	}

	ref = NULL;
	elmt = list->head;
	for (;;) {
		if (NULL == elmt || data == elmt->data) {
			break;
		}
		ref = elmt;
		elmt = elmt->next;
	}

	if (NULL != elmt_p) {
		*elmt_p = elmt;
	}

	if (NULL == elmt) {
		return LIST_ERR__NOTEXIST;
	}

	///
	if (NULL == ref) {
		elmt = list->head;
		list->head = elmt->next;

		if (NULL == elmt->next) {
			list->tail = NULL;
		}
	}
	else {
		elmt = ref->next;
		ref->next = elmt->next;

		if (NULL == ref->next) {
			list->tail = ref;
		}
	}

	list->count--;

	elmt->next = NULL;
	///

	return 0;
}

#endif /* (1 != EXCLUDE_LIST) */
