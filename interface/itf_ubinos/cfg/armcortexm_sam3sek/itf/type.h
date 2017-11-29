/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com
  
  This file is part of the tool_ubibuilder component of the Ubinos.
  
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

#ifndef TYPE__GCCARMELF_H_
#define TYPE__GCCARMELF_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file type.h
 *
 * @brief 기본 자료 형 정의
 */

/** 바이트 형 정의 (8비트) */
typedef	unsigned char			byte_t;

/** 워드 형 정의 (16비트) */
typedef	unsigned short			word_t;

/** 더블 워드 형 정의 (32비트) */
typedef unsigned int			dword_t;

#ifndef NULL
	/** NULL 값 */
	#define NULL				0
#endif

#ifndef EOF
	/** EOF (End of File) 값 */
	#define	EOF					(-1)
#endif

#ifndef __SIZE_TYPE__
	#define __SIZE_TYPE__ 			long unsigned int
#endif

/** size_t 형 정의 */
typedef __SIZE_TYPE__			size_t;

/** size_t 형을 unsigned integer 형으로 변환하는 매크로 */
#define SIZETOUINT(a)			(a)

/** unsigned integer 형을 size_t 형으로 변환하는 매크로 */
#define UINTTOSIZE(a)			(a)

#ifndef INT_MIN
	/** 정수 형 최소 값 */
	#define INT_MIN				0x80000000L // -2147483648
#endif

#ifndef INT_MAX
	/** 정수형 최대 값 */
	#define INT_MAX				0x7FFFFFFFL //  2147483647
#endif

#ifndef UINT_MAX
	/** 무부호 정수형 최대 값 */
	#define UINT_MAX			0xFFFFFFFFL //  4294967295
#endif

#ifndef	INT_SIZE
	/** 정수형 크기 */
	#define INT_SIZE			4
#endif

#ifndef INT16_MIN
	/** 16 비트 정수 형 최소 값 */
	#define INT16_MIN			0x8000 // -32768
#endif

#ifndef INT16_MAX
	/** 16 비트 정수형 최대 값 */
	#define INT16_MAX			0x7FFF //  32767
#endif

#ifndef UINT16_MAX
	/** 16 비트 무부호 정수형 최대 값 */
	#define UINT16_MAX			0xFFFF //  65535
#endif

/** 메모리 정렬 단위 */
#define MEM_ALIGNMENT			INT_SIZE

/** 메모리 정렬 마스크 (Memory Align Mask) */
#define MEM_ALIGNMASK			(INT_SIZE - 1)

/** 메모리 정렬 매크로 */
#define MEM_ALIGN(a)			((((unsigned int) (a)) + MEM_ALIGNMASK) & (~MEM_ALIGNMASK))

#include "objtype.h"

#ifdef	__cplusplus
}
#endif

#endif /* TYPE__GCCARMELF_H_ */
