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

#if (1 != EXCLUDE_STRING)

#include <string.h>

#include "itf_ubinos/itf/type.h"

#if (0 == UBI_INCLUDE_STDLIB)

char * strcpy(char * dst, const char * src) {
	unsigned int i = 0;

	for (i=0; i<(STRING_SIZE_MAX-1); i++)
	{
		if (0 == src[i])
			break;

		dst[i] = src[i];
	}

	dst[i] = 0;

	return dst;
}


char * strncpy(char * dst, const char * src, size_t num) {
	unsigned int i = 0;

	for (i=0; i<(num-1); i++)
	{
		if (0 == src[i])
			break;

		dst[i] = src[i];
	}

	dst[i] = 0;

	return dst;
}

char * strcat(char * dst, const char * src) {
	unsigned int i = 0;
	unsigned int j = 0;

	for (j=0; j<STRING_SIZE_MAX; j++)
	{
		if (0 == dst[j])
			break;
	}

	if (STRING_SIZE_MAX == j) {
		return dst;
	}

	for (i=j; i<(STRING_SIZE_MAX-1); i++)
	{
		if (src[i-j] == 0)
			break;

		dst[i] = src[i-j];
	}

	dst[i] = 0;

	return dst;
}

size_t strlen(const char * str) {
	unsigned int i = 0;

	if (0 == str)
		return UINTTOSIZE(0);

	for (i=0; i<STRING_SIZE_MAX; i++)
	{
		if (0 == str[i])
			break;
	}

	return UINTTOSIZE(i);
}

int strncmp(const char * str1, const char * str2, size_t num) {
	unsigned int i = 0;

	if (NULL == str1) {
		return -2;
	}

	if (NULL == str2) {
		return -3;
	}

	for (i=0; i<num; i++) {
		if (str1[i] == 0 && str2[i] == 0) {
			break;
		}

		if 		(((unsigned char *)str1)[i] > ((unsigned char *)str2)[i]) {
			return 1;
		}
		else if	(((unsigned char *)str1)[i] < ((unsigned char *)str2)[i]) {
			return -1;
		}
	}

	return 0;
}

int strcmp(const char * str1, const char * str2) {
	return strncmp(str1, str2, STRING_SIZE_MAX);
}

char * strnchr(const char *str, int ch, size_t num) {
	int i;
    char * p = (char *) str;
    char   c = ch & 0xFF;

	for (i=0; i<num; i++, p++) {
		if (c == *p) {
			return p;
		}
		if (0 == *p) {
		    return NULL;
		}
	}

    return NULL;
}

char * strchr(const char *str, int ch) {
	return strnchr(str, ch, STRING_SIZE_MAX);
}

char * strrnchr(const char *str, int ch, size_t num) {
	int i;
    char * p = (char *) str;
    char   c = ch & 0xFF;
    char *pr = NULL;

	for (i=0; i<num; i++, p++) {
		if (c == *p) {
			pr = p;
		}
		if (0 == *p) {
		    break;
		}
	}

    return pr;
}

char * strrchr(const char *str, int ch) {
	return strrnchr(str, ch, STRING_SIZE_MAX);
}

int toupper ( int c ) {
	if ('a' <= c && c <= 'z') {
		c = c - ('a' - 'A');
	}
	return c;
}

int tolower ( int c ) {
	if ('A' <= c && c <= 'Z') {
		c = c + ('a' - 'A');
	}
	return c;
}

#endif /* (0 == UBI_INCLUDE_STDLIB) */

#endif /* (1 != EXCLUDE_STRING) */
