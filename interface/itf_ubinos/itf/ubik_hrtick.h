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

#ifndef ITF_UBIK_HRTICK_H_
#define ITF_UBIK_HRTICK_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubik_hrtick.h
 *
 * @brief ubik (Ubinos Kernel) ������Ʈ ���ػ� tick �������̽�
 */

#include "type.h"

/**
 * ���ػ� tick �ڷ� ����
 */
typedef struct _hrtick_t {
	unsigned int	high;
	unsigned int	low;
	unsigned int	hrtick;
} hrtick_t;

/**
 * ���ػ� tick ����� Ȱ��ȭ�ϴ� �Լ�
 *
 * @param	htimerno	���ػ� tick�� ���� �ϵ���� Ÿ�̸� ��ȣ<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int ubik_hrtick_enable(int htimerno);

/**
 * ���ػ� tick ����� ��Ȱ��ȭ�ϴ� �Լ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int ubik_hrtick_disable(void);

/**
 * �ʴ� tick ��(high, low)�� �����ִ� �Լ�
 *
 * @return	�ʴ� tick ��(high, low)<br>
 */
unsigned int ubik_hrtick_gettickpersec(void);

/**
 * õ���� ���ʴ� ���ػ� tick ��(htick)�� �����ִ� �Լ�
 *
 * @return	õ���� ���ʴ� ���ػ� tick ��(htick)<br>
 */
unsigned int ubik_hrtick_gethrtickpermsec(void);

/**
 * ������� ������ ���ػ� tick ���� �����ִ� �Լ�
 *
 * @param	tick_p	������� ������ ���ػ� tick ���� ������ ������  �ּ�<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int ubik_hrtick_gettick(hrtick_t * tick_p);

/**
 * ���ػ� tick ���� ����(tick2���� tick1�� �� ��)�� �����ִ� �Լ�
 *
 * �� ��� tick 1�� �� ��� tick 2���� Ŭ ��� �����÷ο찡 �Ͼ ������ �����ϰ� ���̸� �����
 *
 * @param	tick1_p		�� ��� ���ػ� tick 1�� ������<br>
 * 			<br>
 *
 * @param	tick2_p		�� ��� ���ػ� tick 2�� ������<br>
 * 			<br>
 *
 * @param	tickdiff_p 	���ػ� tick ���� ���̸� ������ ������ �ּ�<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int ubik_hrtick_gettickdiff(hrtick_t * tick1_p, hrtick_t * tick2_p, hrtick_t * tickdiff_p);

/**
 * ���ػ� tick ���� �ð� ������ ��ȯ�ϴ� �Լ�
 *
 * @param	tick_p	��� ���ػ� tick ���� ������<br>
 *
 * @param	nsec_p	��ȯ�� �ʾ���� ���� ���� ���� ������ ������ �ּ�<br>
 * 					NULL�̸� ������<br>
 *
 * @param	usec_p	��ȯ�� �鸸���� ���� ���� ���� ������ ������ �ּ�<br>
 * 					NULL�̸� ������<br>
 *
 * @param	msec_p	��ȯ�� õ���� ���� ���� ���� ������ ������ �ּ�<br>
 * 					NULL�̸� ������<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int ubik_hrtick_hrticktotime(hrtick_t * tick_p, unsigned int * nsec_p, unsigned int * usec_p, unsigned int * msec_p);

/**
 * ���ػ� tick ���� �ʾ���� ���� ������ ��ȯ�ϴ� �Լ�
 *
 * @param	tick_p	��� ���ػ� tick ���� ������<br>
 *
 * @param	nsec_p	��ȯ�� �ʾ���� ���� ���� ���� ������ ������ �ּ�<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int ubik_hrtick_hrticktotimens(hrtick_t * tick_p, unsigned int * nsec_p);

/**
 * ���ػ� tick ���� �鸸���� ���� ������ ��ȯ�ϴ� �Լ�
 *
 * @param	tick_p	��� ���ػ� tick ���� ������<br>
 *
 * @param	usec_p	��ȯ�� �鸸���� ���� ���� ���� ������ ������ �ּ�<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int ubik_hrtick_hrticktotimeus(hrtick_t * tick_p, unsigned int * usec_p);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_HRTICK_H_ */
