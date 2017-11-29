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

#if (1 != EXCLUDE_ATOI)

#include "itf_ubinos/itf/ubiclib.h"

int atoi(const char * buf) {
    int i;
    unsigned char c;
    int v = 0;
    int s = 1;

    i = 0;
	c = (unsigned char) buf[i];
	if ('-' == c) {
		s = -1;
		i++;
	}
	else if ('+' == c) {
		i++;
	}
	for(; i<STRING_SIZE_MAX; i++) {
		c = (unsigned char) buf[i];
		if ('0' <= c && c <= '9') {
			c = c - '0';
		}
		else {
			break;
		}
		v = v * 10 + c;
	}
	v = v * s;

    return v;
}

int ahtoi(const char * buf) {
    int i;
    unsigned char c;
    int v = 0;
    int s = 1;

    i = 0;
	c = (unsigned char) buf[i];
	if ('-' == c) {
		s = -1;
		i++;
	}
	else if ('+' == c) {
		i++;
	}
	for(; i<STRING_SIZE_MAX; i++) {
		c = (unsigned char) buf[i];
		if ('0' <= c && c <= '9') {
			c = c - '0';
		}
		else if ('a' <= c && c <= 'f') {
			c = c - 'a' + 10;
		}
		else if ('A' <= c && c <= 'F') {
			c = c - 'A' + 10;
		}
		else if ('x' == c || 'X' ==  c) {
			v = 0;
			continue;
		}
		else {
			break;
		}
		v = v * 16 + c;
	}
	v = v * s;

    return v;
}

long atol(const char * buf) {
    int i;
    unsigned char c;
    long v = 0;
    int s = 1;

    i = 0;
	c = (unsigned char) buf[i];
	if ('-' == c) {
		s = -1;
		i++;
	}
	else if ('+' == c) {
		i++;
	}
	for(; i<STRING_SIZE_MAX; i++) {
		c = (unsigned char) buf[i];
		if ('0' <= c && c <= '9') {
			c = c - '0';
		}
		else {
			break;
		}
		v = v * 10 + c;
	}
	v = v * s;

    return v;
}

long ahtol(const char * buf) {
    int i;
    unsigned char c;
    long v = 0;
    int s = 1;

    i = 0;
	c = (unsigned char) buf[i];
	if ('-' == c) {
		s = -1;
		i++;
	}
	else if ('+' == c) {
		i++;
	}
	for(; i<STRING_SIZE_MAX; i++) {
		c = (unsigned char) buf[i];
		if ('0' <= c && c <= '9') {
			c = c - '0';
		}
		else if ('a' <= c && c <= 'f') {
			c = c - 'a' + 10;
		}
		else if ('A' <= c && c <= 'F') {
			c = c - 'A' + 10;
		}
		else if ('x' == c || 'X' ==  c) {
			v = 0;
			continue;
		}
		else {
			break;
		}
		v = v * 16 + c;
	}
	v = v * s;

    return v;
}

#endif /* (1 != EXCLUDE_ATOI) */
