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

#if (1 != EXCLUDE_MEM)

#include <string.h>

#include "itf_ubinos/itf/type.h"
#include "itf_ubinos/itf/ubiclib.h"

#if (0 == UBI_INCLUDE_STDLIB)

void * memcpy(void * dst, const void * src, size_t num) {
	size_t i;

	for (i=0; i<num; i++)
	{
		((char *)dst)[i] = ((char *)src)[i];
	}

	return dst;
}

void * memset(void * dst, int value, size_t num) {
	size_t i;

	for (i=0; i<num; i++)
	{
		((char *)dst)[i] = (char) value;
	}

	return dst;
}

int memcmp(const void * ptr1, const void * ptr2, size_t num) {
	size_t i;

	if (NULL == ptr1) {
		return -2;
	}

	if (NULL == ptr2) {
		return -3;
	}

	for (i=0; i<num; i++) {
		if		(((unsigned char *)ptr1)[i] > ((unsigned char *)ptr2)[i]) {
			return 1;
		}
		else if	(((unsigned char *)ptr1)[i] < ((unsigned char *)ptr2)[i]) {
			return -1;
		}
	}

	return 0;
}

void * memmove(void * dst, const void * src, size_t num ) {
	size_t i;

	if (0 < num) {
		if (dst < src) {
			for (i=0; i<num; i++)
			{
				((char *)dst)[i] = ((char *)src)[i];
			}
		}
		else if (dst > src) {
			for (i=num-1; i>=0; i--)
			{
				((char *)dst)[i] = ((char *)src)[i];
			}
		}
	}

	return dst;
}

#endif /* (0 == UBI_INCLUDE_STDLIB) */

int memset0(void * dst, size_t num) {
	unsigned int * ptr = 0;
	unsigned int * end = 0;

	if (NULL == dst) {
		return -2;
	}

	ptr = dst;
	end = (unsigned int *) ((unsigned int) dst + num);

	while (ptr < end && 0 != ((unsigned int) ptr & (INT_SIZE - 1))) {
		*((unsigned char *) ptr) = 0;
		ptr = (unsigned int *) ((unsigned int) ptr + 1);
	}

	while (INT_SIZE <= ( (unsigned int) end - (unsigned int) ptr )) {
		*ptr++ = 0;
	}

	while (ptr < end) {
		*((unsigned char *) ptr) = 0;
		ptr = (unsigned int *) ((unsigned int) ptr + 1);
	}

	return 0;
}

#endif /* (1 != EXCLUDE_MEM) */
