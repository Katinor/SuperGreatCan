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

#if (1 != EXCLUDE_ITOA)

#include "itf_ubinos/itf/ubiclib.h"

int itoa(int value, char * buf, int max) {
	int n = 0;
	int i = 0;
	int tmp = 0;

	if (NULL == buf) {
		return -3;
	}

	if (2 > max) {
		return -4;
	}

	i=1;
	tmp = value;
	if (0 > tmp) {
		tmp *= -1;
		i++;
	}
	for (;;) {
		tmp /= 10;
		if (0 >= tmp) {
			break;
		}
		i++;
	}
	if (i >= max) {
		buf[0] = '?';
		buf[1] = 0x0;
		return 2;
	}

	n = i;
	tmp = value;
	if (0 > tmp) {
		tmp *= -1;
	}
	buf[i--] = 0x0;
	for (;;) {
		buf[i--] = (tmp % 10) + '0';
		tmp /= 10;
		if (0 >= tmp) {
			break;
		}
	}
	if (-1 != i) {
		buf[i--] = '-';
	}

	return n;
}

int itoah(int value, char * buf, int max) {
	int n = 0;
	int i = 0;
	int tmp = 0;
	int tmp2 = 0;

	if (NULL == buf) {
		return -3;
	}

	if (2 > max) {
		return -4;
	}

	i=1;
	tmp = value;
	if (0 > tmp) {
		tmp *= -1;
		i++;
	}
	for (;;) {
		tmp /= 16;
		if (0 >= tmp) {
			break;
		}
		i++;
	}
	if (i >= max) {
		buf[0] = '?';
		buf[1] = 0x0;
		return 2;
	}

	n = i;
	tmp = value;
	if (0 > tmp) {
		tmp *= -1;
	}
	buf[i--] = 0x0;
	for (;;) {
		tmp2 = (tmp % 16);
		if (10 > tmp2) {
			buf[i--] = tmp2 + '0';
		}
		else {
			buf[i--] = tmp2 - 10 + 'A';
		}
		tmp /= 16;
		if (0 >= tmp) {
			break;
		}
	}
	if (-1 != i) {
		buf[i--] = '-';
	}

	return n;
}

int htobi(int value) {
#if (UBI_LITTLE_ENDIAN == 1)
	int tmp = 0;

	((unsigned char *) &tmp)[0] = ((unsigned char *) &value)[3];
	((unsigned char *) &tmp)[1] = ((unsigned char *) &value)[2];
	((unsigned char *) &tmp)[2] = ((unsigned char *) &value)[1];
	((unsigned char *) &tmp)[3] = ((unsigned char *) &value)[0];

	return tmp;
#else
	return value;
#endif
}

#endif /* (1 != EXCLUDE_ITOA) */
