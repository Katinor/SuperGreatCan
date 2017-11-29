/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com
  
  This file is part of the itf_ubinos component of the Ubinos.
  
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

#ifndef ITF_UBICLIB_H_
#define ITF_UBICLIB_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubiclib.h
 *
 * @brief ubiclib (Ubinos C Library) ������Ʈ �������̽�
 *
 * ubiclib ������Ʈ �������̽��� �����մϴ�.
 */

#include "type.h"

/**
 * ubiclib ������Ʈ�� �ʱ�ȭ�ϴ� �Լ�
 *
 * �ʱ�ȭ �۾��� �� ������(reenterance)�� �����ϱ� ���� �κ��� �������� �ʽ��ϴ�.<br>
 * ������(reenterance)�� �����Ϸ��� ubiclib_comp_init_reent �Լ��� �߰��� ȣ���� �־�� �մϴ�.<br>
 * �� �Լ��� �ý��� ���۽�  bsp ������Ʈ�� main �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int ubiclib_comp_init(void);

/**
 * ubiclib ������Ʈ�� �ʱ�ȭ �۾��� �� ������(reenterance)�� �����ϱ� ���� �κ��� �����ϴ� �Լ�
 *
 * �� �Լ��� �ý��� ���۽�  bsp ������Ʈ�� main �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int ubiclib_comp_init_reent(void);

/**
 * �������� 10���� �ƽ�Ű �ڵ� ���ڿ��� ��ȯ�ϴ� �Լ�
 *
 * @param	value	��ȯ�� ������
 *
 * @param	buf		��ȯ�� ���ڿ��� ������ ����
 *
 * @param	max		��ȯ�� ���ڿ��� ������ ������ �ִ� ũ��
 *
 * @return	��ȯ�� ���ڿ� ũ��<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int itoa(int value, char * buf, int max);

/**
 * �������� 16���� �ƽ�Ű �ڵ� ���ڿ��� ��ȯ�ϴ� �Լ�
 *
 * @param	value	��ȯ�� ������
 *
 * @param	buf		��ȯ�� ���ڿ��� ������ ����
 *
 * @param	max		��ȯ�� ���ڿ��� ������ ������ �ִ� ũ��
 *
 * @return	��ȯ�� ���ڿ� ũ��<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int itoah(int value, char * buf, int max);

/**
 * �������� ����Ʈ ������ big endian���� ��ȯ�ϴ� �Լ�
 *
 * @param	value	��ȯ�� ��
 *
 * @return	��ȯ�� ��
 */
int htobi(int value);

/**
 * ����ε� �������� 10���� �ƽ�Ű �ڵ� ���ڿ��� ��ȯ�ϴ� �Լ�
 *
 * @param	value	��ȯ�� ����ε� ������
 *
 * @param	buf		��ȯ�� ���ڿ��� ������ ����
 *
 * @param	max		��ȯ�� ���ڿ��� ������ ������ �ִ� ũ��
 *
 * @return	��ȯ�� ���ڿ� ũ��<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int uitoa(unsigned int value, char * buf, int max);

/**
 * ����ε� �������� 10���� �ƽ�Ű �ڵ� ���ڿ��� ��ȯ�ϴ� �Լ� (���� �����÷ο� �˻� ����)
 *
 * @param	value	��ȯ�� ����ε� ������
 *
 * @param	buf		��ȯ�� ���ڿ��� ������ ����
 *
 * @param	max		��ȯ�� ���ڿ��� ������ ������ �ִ� ũ��
 *
 * @return	��ȯ�� ���ڿ� ũ��<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int uitoa_nosocheck(unsigned int value, char * buf, int max);

/**
 * ����ε� �������� 16���� �ƽ�Ű �ڵ� ���ڿ��� ��ȯ�ϴ� �Լ�
 *
 * @param	value	��ȯ�� ����ε� ������
 *
 * @param	buf		��ȯ�� ���ڿ��� ������ ����
 *
 * @param	max		��ȯ�� ���ڿ��� ������ ������ �ִ� ũ��
 *
 * @return	��ȯ�� ���ڿ� ũ��<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int uitoah(unsigned int value, char * buf, int max);

/**
 * ����ε� �������� 16���� �ƽ�Ű �ڵ� ���ڿ��� ��ȯ�ϴ� �Լ� (���� �����÷ο� �˻� ����)
 *
 * @param	value	��ȯ�� ����ε� ������
 *
 * @param	buf		��ȯ�� ���ڿ��� ������ ����
 *
 * @param	max		��ȯ�� ���ڿ��� ������ ������ �ִ� ũ��
 *
 * @return	��ȯ�� ���ڿ� ũ��<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int uitoah_nosocheck(unsigned int value, char * buf, int max);

/**
 * ����ε� �������� ����Ʈ ������ big endian���� ��ȯ�ϴ� �Լ�
 *
 * @param	value	��ȯ�� ��
 *
 * @return	��ȯ�� ��
 */
unsigned int htobui(unsigned int value);

/**
 * 16���� �ƽ�Ű �ڵ� ���ڿ��� ���������� ��ȯ�ϴ� �Լ�
 *
 * @param	buf		��ȯ�� ���ڿ��� ����� ����
 *
 * @return	��ȯ�� ��
 */
int ahtoi(const char * buf);

/**
 * 16���� �ƽ�Ű �ڵ� ���ڿ��� �� ���������� ��ȯ�ϴ� �Լ�
 *
 * @param	buf		��ȯ�� ���ڿ��� ����� ����
 *
 * @return	��ȯ�� ��
 */
long ahtol(const char * buf);

/**
 * 10���� �ƽ�Ű �ڵ� ���ڿ��� ����ε� ���������� ��ȯ�ϴ� �Լ�
 *
 * @param	buf		��ȯ�� ���ڿ��� ����� ����
 *
 * @return	��ȯ�� ��
 */
unsigned int atoui(const char * buf);

/**
 * 10���� �ƽ�Ű �ڵ� ���ڿ��� ����ε� �� ���������� ��ȯ�ϴ� �Լ�
 *
 * @param	buf		��ȯ�� ���ڿ��� ����� ����
 *
 * @return	��ȯ�� ��
 */
unsigned long atoul(const char * buf);

/**
 * 16���� �ƽ�Ű �ڵ� ���ڿ��� ����ε� ���������� ��ȯ�ϴ� �Լ�
 *
 * @param	buf		��ȯ�� ���ڿ��� ����� ����
 *
 * @return	��ȯ�� ��
 */
unsigned int ahtoui(const char * buf);

/**
 * 16���� �ƽ�Ű �ڵ� ���ڿ��� ����ε� �� ���������� ��ȯ�ϴ� �Լ�
 *
 * @param	buf		��ȯ�� ���ڿ��� ����� ����
 *
 * @return	��ȯ�� ��
 */
unsigned long ahtoul(const char * buf);

#undef toupper
#undef tolower

/**
 * �ƽ�Ű �ڵ� �ҹ��ڸ� �빮�ڷ� ��ȯ�ϴ� �Լ�
 *
 * @param	c		��ȯ�� ����
 *
 * @return	��ȯ�� ����
 */
int toupper ( int c );

/**
 * �ƽ�Ű �ڵ� �빮�ڸ� �ҹ��ڷ� ��ȯ�ϴ� �Լ�
 *
 * @param	c		��ȯ�� ����
 *
 * @return	��ȯ�� ����
 */
int tolower ( int c );

/**
 * �޸𸮸� 0���� �ʱ�ȭ�ϴ� �Լ�
 *
 * @param	dst		�ʱ�ȭ�� �޸� �ּ�
 *
 * @param	num		�ʱ�ȭ�� �޸� ũ��
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int memset0(void * dst, size_t num);

/**
 * ��ȣ ���� ���� ������ ������ �����ϴ� �Լ�
 *
 * @param	x		����
 *
 * @param	y		�¼�
 *
 * @return	y power of x<br>
 */
unsigned int uipow(unsigned int x, unsigned int y);

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBICLIB_H_ */
