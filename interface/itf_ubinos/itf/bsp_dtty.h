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

#ifndef ITF_BSP_DTTY_H_
#define ITF_BSP_DTTY_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file bsp_dtty.h
 *
 * @brief BSP (Board Support Package) ������Ʈ ����� �͹̳� �������̽�
 *
 * BSP ������Ʈ��  �����ϴ� ����� �͹̳� �������̽��� �����մϴ�.
 */

#include "type.h"

/**
 * ����� �͹̳��� �ʱ�ȭ�ϴ� �Լ�
 *
 * �� �Լ��� �ý��� ���۽� bsp_comp_init �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @return	 0: ����<br>
 * 			<br>
 * 			-1: ����
 */
int dtty_init(void);

/**
 * ����� �͹̳��� Ȱ��ȭ�ϴ� �Լ�
 *
 * �� �Լ��� dtty_init �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @return	 0: ����<br>
 * 			<br>
 * 			-1 : ����
 * 			-10: �ý����� �� �Լ��� �������� ����
 */
int dtty_enable(void);

/**
 * ����� �͹̳��� ��Ȱ��ȭ�ϴ� �Լ�
 *
 * @return	 0: ����<br>
 * 			<br>
 * 			-1 : ����
 * 			-10: �ý����� �� �Լ��� �������� ����
 */
int dtty_disable(void);

/**
 * ����� �͹̳ο� ���ڸ� ����ϴ� �Լ�
 *
 * @param	ch		����� ����
 *
 * @return	 0: ����<br>
 * 			<br>
 * 			-1: ����
 */
int dtty_putc(int ch);

/**
 * ����� �͹̳ο� ���ڸ� ����ϴ� �Լ� (���� �����÷ο� �˻縦 ���� ����)
 *
 * @param	ch		����� ����
 *
 * @return	 0: ����<br>
 * 			<br>
 * 			-1: ����
 */
int dtty_putc_nosocheck(int ch);

/**
 * ����� �͹̳ο��� ���ڸ� �Է¹޴� �Լ�
 *
 * @param	ch_p	�Է¹��� ���ڸ� ������ ������ �ּ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int dtty_getc(char * ch_p);

/**
 * ����� �͹̳ο� ���ڿ��� ����ϴ� �Լ�
 *
 * @param	str		����� ���ڿ�
 *
 * @param	max		����� ���ڿ��� �ִ� ũ��
 *
 * @return	 ����� ���ڿ��� ũ��
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int dtty_puts(const char * str, int max);

/**
 * ����� �͹̳ο��� ���ڿ��� �Է¹޴� �Լ�
 *
 * '\n', '\r', �Ǵ� '\0' �� �Էµǰų� �ִ� ũ�⺸�� ���� ������ �Է¹޴´�.
 * �Է¹��� '\n', '\r'�� �۹��� ������� �ʴ´�.
 *
 * @param	str		�Է¹��� ���ڿ��� ������ ����
 *
 * @param	max		�Է¹��� ���ڿ��� �ִ� ũ��
 *
 * @return	 �Է¹��� ���ڿ��� ũ��
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int dtty_gets(char * str, int max);

/**
 * ����� �͹̳ο� ���ڿ��� ����ϴ� �Լ� (���� �����÷ο� �˻縦 ���� ����)
 *
 * @param	str		����� ���ڿ�
 *
 * @param	max		����� ���ڿ��� �ִ� ũ��
 *
 * @return	 ����� ���ڿ��� ũ��
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int dtty_puts_nosocheck(const char * str, int max);

/**
 * ����� �͹̳� �Է� ���ۿ� �Է¹��� ���ڰ� �ִ����� �˻��ϴ� �Լ�
 *
 * @return	 1: ����<br>
 * 			 0: ����<br>
 */
int dtty_kbhit(void);

/**
 * ����� �͹̳� ���� ���� �Լ�
 *
 * @param	echo	0: echo off<br>
 * 					1: echo on<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int dtty_setecho(int echo);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_BSP_DTTY_H_ */
