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

#if (1 != EXCLUDE_PRINTF)

#include <stdarg.h>
#include <stdio.h>

#include "itf_ubinos/itf/bsp_dtty.h"

#include "_ubiclib_vprintfn.h"

static int vfprintfn_putchar(int out, char * dst, int max, char c);
static int vfprintfn_putcharn(int out, char * dst, int max, char c, int n);
static int vfprintfn_putunsignedinteger(int out, char * dst, int max, unsigned int value, int width, char filler);
static int vfprintfn_putinteger(int out, char * dst, int max, signed int value, int width, char filler);
static int vfprintfn_putstring(int out, char * dst, int max, char * string);
static int vfprintfn_puthexadecimal(int out, char * dst, int max, unsigned int value, int width, char filler, char uppercase);
static int vfprintfn_putoctal(int out, char * dst, int max, unsigned int value, int width, char filler);
static char vfprintfn_isdigit(char c);
static int vfprintfn_parsedigits(const char * string, int * parsedsize);
#if defined(EXCLUDE_FLOAT) && (1 != EXCLUDE_FLOAT)
static int vfprintfn_putdouble(int out, char * dst, int max, double value, int width, char filler);
#endif /* defined(EXCLUDE_FLOAT) && (1 != EXCLUDE_FLOAT) */


static int vfprintfn_putchar(int out, char * dst, int max, char c) {
	if (SYS_OUT__MEM == out && NULL == dst) {
		return 0;
	}

	if (0 >= max) {
		return 0;
	}

	switch (out) {
	case SYS_OUT__DTTY:
		dtty_putc((int) c);
		break;
	case SYS_OUT__MEM:
		*dst = c;
		break;
	default:
		return 0;
	}

	return 1;
}

static int vfprintfn_putcharn(int out, char * dst, int max, char c, int n) {
	int i;

	if (SYS_OUT__MEM == out && NULL == dst) {
		return 0;
	}

	if (0 >= max) {
		return 0;
	}

	if (n > max) {
		n = max;
	}

	switch (out) {
	case SYS_OUT__DTTY:
		for (i = 0; i < n; i++) {
			dtty_putc((int) c);
		}
		break;
	case SYS_OUT__MEM:
		for (i = 0; i < n; i++) {
			dst[i] = c;
		}
		break;
	default:
		return 0;
	}

	return i;
}

static int vfprintfn_putunsignedinteger(int out, char * dst, int max, unsigned int value, int width, char filler) {
	int i;
	int n;
	int total;
	int cnt;
	unsigned int tmp;

	if (SYS_OUT__MEM == out && NULL == dst) {
		return 0;
	}

	if (0 >= max) {
		return 0;
	}

	cnt = 1;
	tmp = value / 10;
	while (0 != tmp) {
		cnt++;
		tmp /= 10;
	}

	if (cnt > width) {
		width = cnt;
	}

	if (max < width) {
		width = max;
	}

	total = 0;

	// Fill remaining space
	if (cnt < width) {
		n = vfprintfn_putcharn(out, dst, max - total, filler, width - cnt);
		total += n;
	}

	// Output value
	while (total < width) {
		tmp = value;
		for (i=1; i<cnt; i++) {
			tmp /= 10;
		}

		tmp = tmp % 10 + '0';
		n = vfprintfn_putchar(out, &dst[total], max - total, (char) tmp);
		total += n;

		cnt--;
	}

	return total;
}


static int vfprintfn_putinteger(int out, char * dst, int max, int value, int width, char filler) {
	int n;
	int total;

	if (SYS_OUT__MEM == out && NULL == dst) {
		return 0;
	}

	if (0 >= max) {
		return 0;
	}

	total = 0;

	// Output sign if necessary
	if (0 > value) {
		n = vfprintfn_putchar(out, dst, max, '-');
		total += n;
		value = -value;
	}

	// Output unsigned value
	n = vfprintfn_putunsignedinteger(out, &dst[total], max - total, (unsigned int) value, width, filler);
	total += n;

	return total;
}

static int vfprintfn_putstring(int out, char * dst, int max, char * string) {
	int i;

	if (SYS_OUT__MEM == out && NULL == dst) {
		return 0;
	}

	if (0 >= max) {
		return 0;
	}

	if (NULL == string) {
		return 0;
	}

	switch (out) {
	case SYS_OUT__DTTY:
		i = dtty_puts(string, max);
		break;
	case SYS_OUT__MEM:
		for (i = 0; i < max; i++) {
			if ('\0' == string[i]) {
				break;
			}
			dst[i] = string[i];
		}
		break;
	}

	return i;
}

static int vfprintfn_puthexadecimal(int out, char * dst, int max, unsigned int value, int width, char filler, char uppercase) {
	int i;
	int n;
	int total;
	int cnt;
	unsigned int tmp;

	if (SYS_OUT__MEM == out && NULL == dst) {
		return 0;
	}

	if (0 >= max) {
		return 0;
	}

	cnt = 1;
	tmp = value / 16;
	while (0 != tmp) {
		cnt++;
		tmp /= 16;
	}

	if (cnt > width) {
		width = cnt;
	}

	if (max < width) {
		width = max;
	}

	total = 0;

	// Fill remaining space
	if (cnt < width) {
		n = vfprintfn_putcharn(out, dst, max - total, filler, width - cnt);
		total += n;
	}

	// Output value
	while (total < width) {
		tmp = value;
		for (i=1; i<cnt; i++) {
			tmp /= 16;
		}

		if (10 > (tmp % 16)) {
			// Number
			tmp = tmp % 16 + '0';
		}
		else if (uppercase) {
			// Uppercase letter
			tmp = tmp % 16 - 10 + 'A';
		}
		else {
			// Lowercase letter
			tmp = tmp % 16 - 10 + 'a';
		}
		n = vfprintfn_putchar(out, &dst[total], max - total, (char) tmp);
		total += n;

		cnt--;
	}

	return total;
}

static int vfprintfn_putoctal(int out, char * dst, int max, unsigned int value, int width, char filler) {
	int i;
	int n;
	int total;
	int cnt;
	unsigned int tmp;

	if (SYS_OUT__MEM == out && NULL == dst) {
		return 0;
	}

	if (0 >= max) {
		return 0;
	}

	cnt = 1;
	tmp = value / 8;
	while (0 != tmp) {
		cnt++;
		tmp /= 8;
	}

	if (cnt > width) {
		width = cnt;
	}

	if (max < width) {
		width = max;
	}

	total = 0;

	// Fill remaining space
	if (cnt < width) {
		n = vfprintfn_putcharn(out, dst, max - total, filler, width - cnt);
		total += n;
	}

	// Output value
	while (total < width) {
		tmp = value;
		for (i=1; i<cnt; i++) {
			tmp /= 8;
		}

		tmp = tmp % 8 + '0';
		n = vfprintfn_putchar(out, &dst[total], max - total, (char) tmp);
		total += n;

		cnt--;
	}

	return total;
}

static char vfprintfn_isdigit(char c) {
	if ('0' <= c && '9' >= c) {
		return 1;
	} else {
		return 0;
	}
}

static int vfprintfn_parsedigits(const char * string, int * parsedsize) {
	int i = 0;
	int value = 0;

	if (NULL == string) {
		return 0;
	}

	for (i=0; i<STRING_SIZE_MAX; i++) {
		if (!vfprintfn_isdigit(string[i]))
			break;

		value = (10 * value) + (string[i] - '0');
	}

	if (NULL != parsedsize) {
		*parsedsize = i;
	}

	return value;
}

#if defined(EXCLUDE_FLOAT) && (1 != EXCLUDE_FLOAT)
static int vfprintfn_putdouble(int out, char * dst, int max, double value, int width, char filler) {
	int i;
	int n;
	int total;
	int cnt;
	unsigned int tmp;

	if (SYS_OUT__MEM == out && NULL == dst) {
		return 0;
	}

	if (0 >= max) {
		return 0;
	}

	cnt = 1;
	tmp = value / 10;
	while (0 != tmp) {
		cnt++;
		tmp /= 10;
	}

//	if (cnt > width) {
//		width = cnt;
//	}

//	if (max < width) {
//		width = max;
//	}

	if (6 > width || 0 < width) {
		width = 6;
	}
	total = 0;

	// Output sign if necessary
	if (0 > value) {
		n = vfprintfn_putchar(out, dst, max, '-');
		total += n;
		value = -value;
	}

	// Fill remaining space
//	if (cnt < width) {
//		n = vfprintfn_putcharn(out, dst, max - total, filler, width - cnt);
//		total += n;
//	}

	// Output value
	unsigned int temp_cnt = cnt;
	while (total < temp_cnt/*width*/) {
		tmp = value;
		for (i=1; i<cnt; i++) {
			tmp /= 10;
		}
//
		if(tmp < 0)
			tmp = 0;
		tmp = tmp % 10 + '0';
		n = vfprintfn_putchar(out, &dst[total], max - total, (char) tmp);
		total += n;

		cnt--;
	}

	// Additional code by YJPark : Output Decimal Point
	tmp = value;

	n = vfprintfn_putchar(out, dst, max, '.');
	total += n;

	for(i=0; i<width; i++)
	{
		tmp = (value-tmp)*10;


		tmp = tmp % 10 + '0';
		n = vfprintfn_putchar(out, &dst[total], max - total, (char) tmp);
		total += n;
		value = value*10;
		tmp = value;
	}

	return total;
}
#endif /* defined(EXCLUDE_FLOAT) && (1 != EXCLUDE_FLOAT) */

int vfprintfn(int out, char * dst, int max, const char * format, va_list ap) {
	int i = 0;
	int n = 0;
	int total = 0;

	char parsed = 0;
	int width = -1;
	char filler = ' ';

	if (SYS_OUT__MEM == out && NULL == dst) {
		return 0;
	}

	if (0 >= max) {
		return 0;
	}

	// Loop through format
	for (i=0; i<STRING_SIZE_MAX; i++) {
		if ('\0' == format[i]) {
			break;
		}

		// Check if this is a formatted parameter
		if ('%' == format[i]) {
			// Start token parsing
			parsed = 1;
		}
		else if (parsed)
		{
			// Check if a token is being parsed
			switch (format[i]) {

				// '0'
			case '0':
				filler = '0';
				break;

				// TODO: (Ubinos)  +
				// TODO: (Ubinos)  -
				// TODO: (Ubinos)  #
				// TODO: (Ubinos)  .

				// Width
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				width = vfprintfn_parsedigits(&format[i], &n);
				i += (n-1);
				break;

				// TODO: (Ubinos) *

				// Signed integer
			case 'd':
			case 'i':
				n = vfprintfn_putinteger(out, &dst[total], max - total, va_arg(ap, int), width, filler);
				total += n;
				parsed = 0;
				break;

				// Unsigned integer
			case 'u':
				n = vfprintfn_putunsignedinteger(out, &dst[total], max - total, va_arg(ap, unsigned int), width, filler);
				total += n;
				parsed = 0;
				break;

				// Double in fixed-point notation
				// TODO: (Ubinos) double

				// Hexadecimal value
			case 'x':
				n = vfprintfn_puthexadecimal(out, &dst[total], max - total, va_arg(ap, unsigned int), width, filler, 0);
				total += n;
				parsed = 0;
				break;

			case 'X':
				n = vfprintfn_puthexadecimal(out, &dst[total], max - total, va_arg(ap, unsigned int), width, filler, 1);
				total += n;
				parsed = 0;
				break;

				// Octal value
			case 'o':
				n = vfprintfn_putoctal(out, &dst[total], max - total, va_arg(ap, unsigned int), width, filler);
				total += n;
				parsed = 0;
				break;

				// String
			case 's':
				n = vfprintfn_putstring(out, &dst[total], max - total, va_arg(ap, char *));
				total += n;
				parsed = 0;
				break;

				// Character
			case 'c':
				// 'char' is promoted to 'int' when passing through va_arg
				n = vfprintfn_putchar(out, &dst[total], max - total, va_arg(ap, unsigned int));
				total += n;
				parsed = 0;
				break;

				// %
			case '%':
				i--;
				parsed = 0;
				break;

				//
#if defined(EXCLUDE_FLOAT) && (1 != EXCLUDE_FLOAT)
			case 'f':
				n = vfprintfn_putdouble(out, &dst[total], max - total, va_arg(ap, double), width, filler);
				total += n;
				parsed = 0;
				break;
#endif /* defined(EXCLUDE_FLOAT) && (1 != EXCLUDE_FLOAT) */
			}

			// Reset optional values if token has been consumed
			if (!parsed) {
				width = -1;
				filler = ' ';
			}
		}
		else {
			// Display character
			n = vfprintfn_putchar(out, &dst[total], max - total, format[i]);
			total += n;
		}
	}

	if (max <= total) {
		total = max;
		i = max - 1;
	}
	else {
		i = total;
	}

	if (SYS_OUT__MEM == out) {
		dst[i] = '\0';
	}

	return total;
}

#if (0 == UBI_INCLUDE_STDLIB)

int sprintf(char * str, const char * format, ...) {
	int n = 0;
	va_list ap;

	va_start(ap, format);

	n = vfprintfn(SYS_OUT__MEM, str, STRING_SIZE_MAX, format, ap);

	va_end(ap);

	return n;
}

int snprintf(char * str, size_t num, const char * format, ...) {
	int n = 0;
	va_list ap;

	va_start(ap, format);

	n = vfprintfn(SYS_OUT__MEM, str, num, format, ap);

	va_end(ap);

	return n;
}

int printf(const char * format, ...) {
	int n = 0;
	va_list ap;

	va_start(ap, format);

	n = vfprintfn(SYS_OUT__DTTY, 0x0, STRING_SIZE_MAX, format, ap);

	va_end(ap);

	return n;
}

int puts(const char * str) {
	int n = 0;
	int r = 0;

	n = dtty_puts(str, STRING_SIZE_MAX);
	r = dtty_putc('\n');
	if (0 == r) {
		n++;
	}

	return n;
}

char * gets(char * str) {
	int n;

	n = dtty_gets(str, STRING_SIZE_MAX);
	if (0 >= n) {
		return NULL;
	}
	else {
		return str;
	}
}

#undef putchar

int putchar(int c) {
	int r = 0;

	r = dtty_putc(c);

	if (0 == r) {
		return c;
	}
	else {
		return EOF;
	}
}

#endif /* (0 == UBI_INCLUDE_STDLIB) */

#endif /* (1 != EXCLUDE_PRINTF) */
