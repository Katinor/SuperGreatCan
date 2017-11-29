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

#ifndef ITF_BSP_INTR_H_
#define ITF_BSP_INTR_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file bsp_intr.h
 *
 * @brief BSP (Board Support Package) ������Ʈ ���ͷ�Ʈ �������̽�
 *
 * BSP ������Ʈ��  �����ϴ� ���ͷ�Ʈ �������̽��� �����մϴ�.
 */

/**
 * @example	intrtest00.c
 */

/**
 * \page intr_example ���ͷ�Ʈ ����
 *
 *         \section intrtest00_sec ������ ���ͷ�Ʈ ó�� ����
 *             Simple interrupt handling example
 *
 *             - <a href="intrtest00_8c-example.html">intrtest00.c</a>
 *
 *             �� ������ sam7x256ek ������ �ϵ���� Ÿ�̸Ӹ� 1�ʸ� �ֱ�� �ݺ������� ���ͷ�Ʈ�� �߻���Ű���� �����ϰ�,
 *             �� ���ͷ�Ʈ�� �޾� ó���Ѵ�.<br>
 *
 * <br>
 *
 * Content is available under Attribution-Noncommercial-No Derivative Works 3.0 Unported.
 */

#include "type.h"

/** ���ͷ�Ʈ ���� ���� ����: 	���� ����edge triggered */
#define	INTR_OPT__EDGE			0x00

/** ���ͷ�Ʈ ���� ���� ����: 	���� ����level sensitive */
#define	INTR_OPT__LEVEL			0x80

/** ���ͷ�Ʈ ���� ���� ����: 	���� ����edge triggered ���� ���װ� �Բ� ���� ��� �װ�Ƽ�� ���� ����negative edge triggered,
							���� ����level sensitive ���� ���װ� �Բ� ���� ��� �ο� ���� ����low level sensitive */
#define	INTR_OPT__LOW			0x00

/** ���ͷ�Ʈ ���� ���� ����: 	���� ����edge triggered ���� ���װ� �Բ� ���� ��� ����Ƽ�� ���� ����positive edge triggered,
							���� ����level sensitive ���� ���װ� �Բ� ���� ��� ���� ���� ����high level sensitive */
#define	INTR_OPT__HIGH			0x40

/** ���ͷ�Ʈ ���� ��ƾ �Լ� ������  �� ����  */
typedef void (* isr_ft) (void);

/**
 * ��� ���ͷ�Ʈ�� ���ͷ�Ʈ ���� ��ƾ�� �����Ű�� �Լ�
 *
 * @param	no			��� ���ͷ�Ʈ ��ȣ<br>
 *						<br>
 *
 * @param	isr			���ͷ�Ʈ ���� ��ƾ �Լ� ������<br>
 *						<br>
 *
 * @param	priority	�켱 ����<br>
 *						<br>
 *
 * @param	option		���� ���� (BSP_INTR_OPT__...)<br>
 * 						0: �װ�Ƽ�� ���� ����negative edge triggered<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int intr_connectisr(int no, isr_ft isr, int priority, unsigned int option);

/**
 * ��� ���ͷ�Ʈ�� Ȱ��ȭ�ϴ� �Լ�
 *
 * @param	no			��� ���ͷ�Ʈ ��ȣ<br>
 *						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int intr_enable(int no);

/**
 * ��� ���ͷ�Ʈ�� ��Ȱ��ȭ�ϴ� �Լ�
 *
 * @param	no			��� ���ͷ�Ʈ ��ȣ<br>
 *						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int intr_disable(int no);

/**
 * ��� ��� ���ͷ�Ʈ�� Ŭ�����ϴ� �Լ�
 *
 * @param	no			��� ���ͷ�Ʈ ��ȣ<br>
 *						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int intr_ClearPending(int no);

/**
 * ���� ó������ ���ͷ�Ʈ ��ȣ�� �����ִ� �Լ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -2: ���ͷ�Ʈ ó������ �ƴ�<br>
 * 			 -3: �� ����� �������� ����<br>
 */
int intr_getcurno();

/**
 * ���ͷ�Ʈ�� ���� �� �ִ� �ְ� �켱������ �����ִ� �Լ�
 *
 * @return	���ͷ�Ʈ�� ���� �� �ִ� �ְ� �켱����
 */
int intr_gethighestpriority(void);

/**
 * ���ͷ�Ʈ�� ���� �� �ִ� ���� �켱������ �����ִ� �Լ�
 *
 * @return	���ͷ�Ʈ�� ���� �� �ִ� ���� �켱����
 */
int intr_getlowestpriority(void);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_BSP_INTR_H_ */
