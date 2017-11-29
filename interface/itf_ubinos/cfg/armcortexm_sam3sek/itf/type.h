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
 * @brief �⺻ �ڷ� �� ����
 */

/** ����Ʈ �� ���� (8��Ʈ) */
typedef	unsigned char			byte_t;

/** ���� �� ���� (16��Ʈ) */
typedef	unsigned short			word_t;

/** ���� ���� �� ���� (32��Ʈ) */
typedef unsigned int			dword_t;

#ifndef NULL
	/** NULL �� */
	#define NULL				0
#endif

#ifndef EOF
	/** EOF (End of File) �� */
	#define	EOF					(-1)
#endif

#ifndef __SIZE_TYPE__
	#define __SIZE_TYPE__ 			long unsigned int
#endif

/** size_t �� ���� */
typedef __SIZE_TYPE__			size_t;

/** size_t ���� unsigned integer ������ ��ȯ�ϴ� ��ũ�� */
#define SIZETOUINT(a)			(a)

/** unsigned integer ���� size_t ������ ��ȯ�ϴ� ��ũ�� */
#define UINTTOSIZE(a)			(a)

#ifndef INT_MIN
	/** ���� �� �ּ� �� */
	#define INT_MIN				0x80000000L // -2147483648
#endif

#ifndef INT_MAX
	/** ������ �ִ� �� */
	#define INT_MAX				0x7FFFFFFFL //  2147483647
#endif

#ifndef UINT_MAX
	/** ����ȣ ������ �ִ� �� */
	#define UINT_MAX			0xFFFFFFFFL //  4294967295
#endif

#ifndef	INT_SIZE
	/** ������ ũ�� */
	#define INT_SIZE			4
#endif

#ifndef INT16_MIN
	/** 16 ��Ʈ ���� �� �ּ� �� */
	#define INT16_MIN			0x8000 // -32768
#endif

#ifndef INT16_MAX
	/** 16 ��Ʈ ������ �ִ� �� */
	#define INT16_MAX			0x7FFF //  32767
#endif

#ifndef UINT16_MAX
	/** 16 ��Ʈ ����ȣ ������ �ִ� �� */
	#define UINT16_MAX			0xFFFF //  65535
#endif

/** �޸� ���� ���� */
#define MEM_ALIGNMENT			INT_SIZE

/** �޸� ���� ����ũ (Memory Align Mask) */
#define MEM_ALIGNMASK			(INT_SIZE - 1)

/** �޸� ���� ��ũ�� */
#define MEM_ALIGN(a)			((((unsigned int) (a)) + MEM_ALIGNMASK) & (~MEM_ALIGNMASK))

#include "objtype.h"

#ifdef	__cplusplus
}
#endif

#endif /* TYPE__GCCARMELF_H_ */
