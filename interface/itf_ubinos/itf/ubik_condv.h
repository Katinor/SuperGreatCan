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

#ifndef ITF_UBIK_CONDV_H_
#define ITF_UBIK_CONDV_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubik_condv.h
 *
 * @brief ubik (Ubinos Kernel) ������Ʈ ���º��� �������̽�
 */

/**
 * @example	condvtest01.c
 * @example	condvtest02.c
 * @example	condvtest03.c
 * @example	condvtest04.c
 */

#include "type.h"

#include "../itf/ubik_task.h"
#include "../itf/ubik_mutex.h"

typedef	struct __condv_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _condv_tip_t;

/** ���º��� ������ �� ���� */
typedef	_condv_tip_t *	condv_pt;

/**
 * ���º����� �����ϴ� �Լ�
 *
 * @param	condv_p		������ ���º����� �ּҸ� ������ �������� �ּ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int condv_create(condv_pt * condv_p);

/**
 * ���º����� �����ϴ� �Լ� (Ȯ����)
 *
 * @param	condv_p		������ ���º����� �ּҸ� ������ �������� �ּ�
 *
 * @param	option		�ɼ� (CONDV_OPT__...)<br>
 * 						0: �⺻ ����<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int condv_create_ext(condv_pt * condv_p, unsigned int option);

/**
 * ���º����� �����ϴ� �Լ�
 *
 * @param	condv_p		������ ���º����� �ּҰ� ����� �������� �ּ�<br>
 * 						���ſ� �����ϸ� *condv_p�� NULL�� ��
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int condv_delete(condv_pt * condv_p);

/**
 * ���º����� ����Ǳ⸦ ��ٸ��� �Լ�
 *
 * @param	condv			��� ���º��� ������
 *
 * @param	mutex			��� ���º����� ��ȣ�ϴ� ���ؽ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 * 			UBIK_ERR__TERMINATED	: ��ٸ��� ��ü�� ���ŵǾ���<br>
 */
int condv_wait(condv_pt condv, mutex_pt mutex);

/**
 * ���� �ð����� ���º����� ����Ǳ⸦ ��ٸ��� �Լ�
 *
 * @param	condv			��� ���º��� ������
 *
 * @param	mutex			��� ���º����� ��ȣ�ϴ� ���ؽ�
 *
 * @param	tick			���� �ð� (�ý��� ƽ ��)
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__TIMEOUT		: ���� �ð� �ʰ�<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 * 			UBIK_ERR__TERMINATED	: ��ٸ��� ��ü�� ���ŵǾ���<br>
 */
int condv_wait_timed(condv_pt condv, mutex_pt mutex, unsigned int tick);

/**
 * ���� �ð����� ���º����� ����Ǳ⸦ ��ٸ��� �Լ� (õ���� ���� ����)
 *
 * @param	condv			��� ���º��� ������
 *
 * @param	mutex			��� ���º����� ��ȣ�ϴ� ���ؽ�
 *
 * @param	timems			���� �ð� (õ���� ����)
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__TIMEOUT		: ���� �ð� �ʰ�<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 * 			UBIK_ERR__TERMINATED	: ��ٸ��� ��ü�� ���ŵǾ���<br>
 */
int condv_wait_timedms(condv_pt condv, mutex_pt mutex, unsigned int timems);

/**
 * ��ٸ��� �ִ� �½�ũ�� �� �ϳ����� ���º����� ����Ǿ����� �˷��ִ� �Լ�
 *
 * @param	condv		��� ���º��� ������
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int condv_signal(condv_pt condv);

/**
 * ��ٸ��� �ִ� �½�ũ���ο��� ���º����� ����Ǿ����� �˷��ִ� �Լ�
 *
 * @param	condv		��� ���º��� ������
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int condv_broadcast(condv_pt condv);

/**
 * ���º����� ����Ǿ����� �˷��ִ� (���º����� ��ٸ��� �ֻ��� �켱���� �½�ũ�ǿ켱������ ��ӹ���) �½�ũ�� �����ϴ� �Լ�
 *
 * @param	condv		��� ���º��� ������
 *
 * @param	task		��� �½�ũ ������<br>
 * 						NULL�̸� ��ϵ� �½�ũ ����<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int condv_setsender(condv_pt condv, task_pt task);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_CONDV_H_ */
