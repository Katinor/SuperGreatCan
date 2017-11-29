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

#ifndef ITF_UBIK_SIGNAL_H_
#define ITF_UBIK_SIGNAL_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubik_signal.h
 *
 * @brief ubik (Ubinos Kernel) ������Ʈ �ñ׳� �������̽�
 */

/**
 * @example	signaltest01.c
 * @example	signaltest02.c
 * @example	signaltest03.c
 * @example	signaltest04.c
 */

#include "type.h"

#include "../itf/ubik_task.h"

typedef	struct __signal_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _signal_tip_t;

/** �ñ׳� ������ �� ���� */
typedef	_signal_tip_t *	signal_pt;

/**
 * �ñ׳θ� �����ϴ� �Լ�
 *
 * @param	signal_p	������ �ñ׳��� �ּҸ� ������ �������� �ּ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int signal_create(signal_pt * signal_p);

/**
 * �ñ׳θ� �����ϴ� �Լ� (Ȯ����)
 *
 * @param	signal_p	������ �ñ׳��� �ּҸ� ������ �������� �ּ�
 *
 * @param	option		�ɼ� (SIGNAL_OPT__...)<br>
 * 						0: �⺻ ����<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int signal_create_ext(signal_pt * signal_p, unsigned int option);

/**
 * �ñ׳θ� �����ϴ� �Լ�
 *
 * @param	signal_p		������ �ñ׳��� �ּҰ� ����� �������� �ּ�<br>
 * 							���ſ� �����ϸ� *signal_p�� NULL�� ��
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int signal_delete(signal_pt * signal_p);

/**
 * �ñ׳��� ��ٸ��� �Լ�
 *
 * @param	signal		��� �ñ׳� ������
 *
 * @return	���� �ñ׳� ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 * 			UBIK_ERR__TERMINATED	: ��ٸ��� ��ü�� ���ŵǾ���<br>
 */
int signal_wait(signal_pt signal);

/**
 * ���� �ð����� �ñ׳��� ��ٸ��� �Լ�
 *
 * @param	signal		��� �ñ׳� ������
 *
 * @param	tick		���� �ð� (�ý��� ƽ ��)
 *
 * @return	���� �ñ׳� ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__TIMEOUT		: ���� �ð� �ʰ�<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 * 			UBIK_ERR__TERMINATED	: ��ٸ��� ��ü�� ���ŵǾ���<br>
 */
int signal_wait_timed(signal_pt signal, unsigned int tick);

/**
 * ���� �ð����� �ñ׳��� ��ٸ��� �Լ� (õ���� ���� ����)
 *
 * @param	signal		��� �ñ׳� ������
 *
 * @param	timems		���� �ð� (õ���� ����)
 *
 * @return	���� �ñ׳� ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			UBIK_ERR__TIMEOUT		: ���� �ð� �ʰ�<br>
 * 			UBIK_ERR__DEADLOCK		: ����� �߻� ���ɼ��� ����<br>
 * 			UBIK_ERR__TERMINATED	: ��ٸ��� ��ü�� ���ŵǾ���<br>
 */
int signal_wait_timedms(signal_pt signal, unsigned int timems);

/**
 * ��ٸ��� �ִ� �½�ũ�� �� �ϳ����� �ñ׳��� ������ �Լ�
 *
 * @param	signal		��� �ñ׳� ������
 *
 * @param	sigtype		�ñ׳� ���� (0 �̻�)
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int signal_send(signal_pt signal, int sigtype);

/**
 * ��ٸ��� �ִ� �½�ũ�� ��ο��� �ñ׳��� ������ �Լ�
 *
 * @param	signal		��� �ñ׳� ������
 *
 * @param	sigtype		�ñ׳� ���� (0 �̻�)
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int signal_broadcast(signal_pt signal, int sigtype);

/**
 * �ñ׳��� ������(�ñ׳��� ��ٸ��� �ֻ��� �켱���� �½�ũ�ǿ켱������ ��ӹ���) �½�ũ�� �����ϴ� �Լ�
 *
 * @param	signal		��� �ñ׳� ������
 *
 * @param	task		��� �½�ũ ������<br>
 * 						NULL: ���� ����<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int signal_setsender(signal_pt signal, task_pt task);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_SIGNAL_H_ */
