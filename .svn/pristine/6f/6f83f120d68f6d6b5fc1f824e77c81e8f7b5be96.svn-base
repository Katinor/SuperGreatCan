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

#if (1 != EXCLUDE_CIRBUF)

#include <stdlib.h>
#include <string.h>

#include "itf_ubinos/itf/ubinos.h"
#include "itf_ubinos/itf/ubiclib_cirbuf.h"

int cirbuf_create(cirbuf_pt * cirbuf_p, unsigned int maxsize) {
	return cirbuf_create_ext(cirbuf_p, maxsize, 0);
}

int cirbuf_create_ext(cirbuf_pt * cirbuf_p, unsigned int maxsize, unsigned int option) {
	#define	__FUNCNAME__	"cirbuf_create_ext"
	cirbuf_pt cirbuf;

	if (NULL == cirbuf_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		return -2;
	}

	if (0 >= maxsize) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		return -3;
	}

	cirbuf = malloc(sizeof(cirbuf_t) + maxsize);
	if (NULL == cirbuf) {
		logme(""__FUNCNAME__": fail at malloc()\r\n");
		return -1;
	}

	cirbuf_init(cirbuf, maxsize);

	if (0 == (option & CIRBUF_OPT__NOOVERWRITE)) {
		cirbuf->overwrite = 1;
	}

	if (0 != (option & CIRBUF_OPT__MTPROTECTION)) {
		cirbuf->mtprotection = 1;
	}

	*cirbuf_p = cirbuf;

	return 0;
	#undef __FUNCNAME__
}

int cirbuf_delete(cirbuf_pt * cirbuf_p) {
	#define	__FUNCNAME__	"cirbuf_delete"
	cirbuf_pt cirbuf;

	if (NULL == cirbuf_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		return -2;
	}

	if (NULL == *cirbuf_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		return -2;
	}

	cirbuf = *cirbuf_p;

	free(cirbuf);

	*cirbuf_p = NULL;

	return 0;
	#undef __FUNCNAME__
}


int cirbuf_write(cirbuf_pt cirbuf, unsigned char * buf, unsigned int size, unsigned int * written_p) {
	#define	__FUNCNAME__	"cirbuf_write"
	unsigned char *	cirbuf__head;
	unsigned char *	cirbuf__tail;
	unsigned int	cirbuf__size;
	unsigned int 	offset;
	unsigned int 	size1;
	unsigned int 	size2;
	unsigned int 	sizeold;

	if (NULL == cirbuf) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		return -2;
	}

	if (NULL == buf) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		return -3;
	}

	if (0 == size) {
		if (NULL != written_p) {
			*written_p = size;
		}
		return 0;
	}

	if (cirbuf->maxsize < size)
	{
		logme(""__FUNCNAME__": parameter 3 is wrong\r\n");
		return -4;
	}

	if (0 != cirbuf->mtprotection) {
		ubik_entercrit();
		cirbuf__head = cirbuf->head;
		cirbuf__tail = cirbuf->tail;
		cirbuf__size = cirbuf->size;
		ubik_exitcrit();
	}
	else {
		cirbuf__head = cirbuf->head;
		cirbuf__tail = cirbuf->tail;
		cirbuf__size = cirbuf->size;
	}

	size1 = cirbuf->maxsize - cirbuf__size;
	if (size1 < size) {
		if (UINT16_MAX > cirbuf->overflowcount) {
			cirbuf->overflowcount++;
		}
		if (0 == cirbuf->overwrite) {
			size = size1;
		}
	}

	if (0 == size) {
		if (NULL != written_p) {
			*written_p = size;
		}
		return 0;
	}

	offset = cirbuf__tail - cirbuf->buf;

	size1 = cirbuf->maxsize - offset;
	if (size1 >= size) {
		size1 = size;
		size2 = 0;
	}
	else {
		size2 = size - size1;
	}

	if (size1 > 0) {
		memcpy(cirbuf__tail, buf, size1);
	}

	if (size2 > 0) {
		memcpy(cirbuf->buf, buf + size1, size2);
	}

	cirbuf__tail = (offset + size) % cirbuf->maxsize + cirbuf->buf;
	sizeold = cirbuf__size;
	cirbuf__size += size;
	if (cirbuf->maxsize < cirbuf__size || sizeold > cirbuf__size) {
		cirbuf__head = cirbuf__tail;
		cirbuf__size = cirbuf->maxsize;
		if (UINT16_MAX > cirbuf->overflowcount) {
			cirbuf->overflowcount++;
		}
	}

	if (NULL != written_p) {
		*written_p = size;
	}

	if (0 != cirbuf->mtprotection) {
		ubik_entercrit();
		cirbuf->head = cirbuf__head;
		cirbuf->tail = cirbuf__tail;
		cirbuf->size = cirbuf__size;
		ubik_exitcrit();
	}
	else {
		cirbuf->head = cirbuf__head;
		cirbuf->tail = cirbuf__tail;
		cirbuf->size = cirbuf__size;
	}

	return 0;
	#undef __FUNCNAME__
}

int cirbuf_read(cirbuf_pt cirbuf, unsigned char * buf, unsigned int size, unsigned int * read_p) {
	#define	__FUNCNAME__	"cirbuf_read"
	unsigned char *	cirbuf__head;
	unsigned char *	cirbuf__tail;
	unsigned int	cirbuf__size;
	unsigned int 	offset;
	unsigned int 	size1;
	unsigned int 	size2;

	if (NULL == cirbuf) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		return -2;
	}

	if (NULL == buf) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		return -3;
	}

	if (0 == size) {
		if (NULL != read_p) {
			*read_p = size;
		}
		return 0;
	}

	if (0 != cirbuf->mtprotection) {
		ubik_entercrit();
		cirbuf__head = cirbuf->head;
		cirbuf__tail = cirbuf->tail;
		cirbuf__size = cirbuf->size;
		ubik_exitcrit();
	}
	else {
		cirbuf__head = cirbuf->head;
		cirbuf__tail = cirbuf->tail;
		cirbuf__size = cirbuf->size;
	}

	if (cirbuf__size < size) {
		logme(""__FUNCNAME__": parameter 3 is wrong\r\n");
		return -4;
	}

	offset = cirbuf__head - cirbuf->buf;

	size1 = cirbuf->maxsize - offset;
	if (size1 >= size) {
		size1 = size;
		size2 = 0;
	}
	else {
		size2 = size - size1;
	}

	if (size1 > 0) {
		memcpy(buf, cirbuf__head, size1);
	}

	if (size2 > 0) {
		memcpy(buf + size1, cirbuf->buf, size2);
	}

	cirbuf__head = (offset + size) % cirbuf->maxsize + cirbuf->buf;
	cirbuf__size -= size;

	if (NULL != read_p) {
		*read_p = size;
	}

	if (0 != cirbuf->mtprotection) {
		ubik_entercrit();
		cirbuf->head = cirbuf__head;
		cirbuf->tail = cirbuf__tail;
		cirbuf->size = cirbuf__size;
		ubik_exitcrit();
	}
	else {
		cirbuf->head = cirbuf__head;
		cirbuf->tail = cirbuf__tail;
		cirbuf->size = cirbuf__size;
	}

	return 0;
	#undef __FUNCNAME__
}

int cirbuf_clear(cirbuf_pt cirbuf) {
	#define	__FUNCNAME__	"cirbuf_clear"

	if (NULL == cirbuf) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		return -2;
	}

	if (0 != cirbuf->mtprotection) {
		ubik_entercrit();
		cirbuf->head			= cirbuf->buf;
		cirbuf->tail			= cirbuf->buf;
		cirbuf->size			= 0;
		cirbuf->overflowcount	= 0;
		ubik_exitcrit();
	}
	else {
		cirbuf->head			= cirbuf->buf;
		cirbuf->tail			= cirbuf->buf;
		cirbuf->size			= 0;
		cirbuf->overflowcount	= 0;
	}

	return 0;
	#undef __FUNCNAME__
}
#endif /* (1 != EXCLUDE_CIRBUF) */
