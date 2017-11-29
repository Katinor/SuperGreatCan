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

#ifndef ITF_UBIK_STIMER_H_
#define ITF_UBIK_STIMER_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubik_stimer.h
 *
 * @brief ubik (Ubinos Kernel) ������Ʈ �������� Ÿ�̸� �������̽�
 */

/**
 * @example	stimertest00.c
 * @example	stimertest01.c
 * @example	stimertest02.c
 * @example	stimertest03.c
 */

/**
 * \page stimer_example �������� Ÿ�̸� ����
 *
 *         \section stimertest00_sec ������ �������� Ÿ�̸� ��� ����
 *             Simple semaphore timer example
 *
 *             - <a href="stimertest00_8c-example.html">stimertest00.c</a>
 *
 *             �� ������ ������ �ֱ�� ������ �޽����� �ݺ������� ����Ѵ�.<br>
 *
 * <br>
 *
 *         \section stimertest01_sec �������� Ÿ�̸��� �ֱ��� �˸� ��� ����
 *             Test on periodic notification function of semaphore timer
 *
 *             - <a href="stimertest01_8c-example.html">stimertest01.c</a>
 *
 *             �� ������ �������� Ÿ�̸��� �ֱ��� �˸� ����� �����Ѵ�.
 *
 *             ���� 1���� �½�ũ 2�� �߰� �켱������ �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 1���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 2���� �½�ũ 1�� ���� �켱������ �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 2���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 3���� �½�ũ 1�� �������� Ÿ�̸Ӹ� �����ϰ� ������ ����, �˸� ��ȣ�� ��ٸ���.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 3���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 5�� �Ǹ� �������� Ÿ�̸Ӱ� �˸� ��ȣ�� ������.<br>
 *             �׷��� �½�ũ 1�� �� �˸� ��ȣ�� �ް� ��� �־��� ���� ó���Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 5���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ����  6�̵Ǹ�(�־��� �� ó���� ��ġ��) �½�ũ 1��  �ٽ� �˸� ��ȣ�� ��ٸ���.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 6���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 7�� �Ǹ� �������� Ÿ�̸Ӱ� �ٽ� �˸� ��ȣ�� ������.<br>
 *             �׷��� �½�ũ 1�� �� �˸� ��ȣ�� �ް� ��� �ٽ� �־��� ���� ó���Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 7���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ����  8���� �½�ũ 1�� �������� Ÿ�̸Ӹ� �����.<br>
 *             �׸��� ���� 10������ ���� �ð����� �ΰ� �˸� ��ȣ�� ��ٸ���.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 8���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 10�� �Ǹ� �½�ũ 1�� ���� �ð��� ���ؼ� �˸� ��ȣ�� ���� ���ߴ��� �����.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 10���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 11���� �½�ũ 1�� ����ȴ�.<br>
 *
 *             ���� 12���� �½�ũ 2�� ����ȴ�.<br>
 *
 * <br>
 *
 *             \image html ubik_test_stimertest01.gif
 *
 * <br>
 *
 *         \section stimertest02_sec �������� Ÿ�̸��� ��ȸ�� �˸� ��� ����
 *             Test on periodic notification function of semaphore timer
 *
 *             - <a href="stimertest02_8c-example.html">stimertest02.c</a>
 *
 *             �� ������ �������� Ÿ�̸��� ��ȸ�� �˸� ����� �����Ѵ�.
 *
 *             ���� 1���� �½�ũ 2�� �߰� �켱������ �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 1���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 2���� �½�ũ 1�� ���� �켱������ �����Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 2���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 3���� �½�ũ 1�� ��ȸ�� ���� ����(STIMER_OPT__ONESHOT)�� �����ϰ�
 *             �������� Ÿ�̸Ӹ� �����Ѵ�.<br>
 *             �׸��� �������� Ÿ�̸Ӹ� ������ ���� �˸� ��ȣ�� ��ٸ���.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 3���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 5�� �Ǹ� �������� Ÿ�̸Ӱ� �˸� ��ȣ�� ������.<br>
 *             �׷��� �½�ũ 1�� �� �˸� ��ȣ�� �ް� ��� �־��� ���� ó���Ѵ�.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 5���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ����  6�̵Ǹ�(�־��� �� ó���� ��ġ��) �½�ũ 1��  ���� 8������ ���� �ð����� �ΰ� �ٽ� �˸� ��ȣ�� ��ٸ���.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 6���� �½�ũ 2�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 8�� �Ǹ� �½�ũ 1�� ���� �ð��� ���ؼ� �˸� ��ȣ�� ���� ���ߴ��� �����.<br>
 *             ������� ���������� ����Ǿ��ٸ� Ȯ�� ���� 8���� �½�ũ 1�� ���� ���̾�� �Ѵ�.<br>
 *
 *             ���� 9���� �½�ũ 1�� ����ȴ�.<br>
 *
 *             ���� 10���� �½�ũ 2�� ����ȴ�.<br>
 *
 * <br>
 *
 *             \image html ubik_test_stimertest02.gif
 *
 * <br>
 *
 *         \section stimertest03_sec �������� Ÿ�̸ӿ� ���� �޸� ���� ����
 *             Test for memory leak by stimer
 *
 *             - <a href="stimertest03_8c-example.html">stimertest03.c</a>
 *
 *             �� ������ �������� Ÿ�̸ӿ� ���� �޸� ������ �����Ѵ�.<br>
 *
 *             ���� ��heap�� �Ҵ�� �޸� �ѷ��� �޸� ��� ���� ������ �����Ѵ�.<br>
 *             �׸��� �������� Ÿ�̸� ������ ���Ÿ� ���� �� �ݺ��Ѵ�.<br>
 *             �� ���� ��heap�� �Ҵ�� �޸� �ѷ��� �޸� �� ���� �ٽ� ������ �����ߴ� ���� ���Ѵ�.<br>
 *             �޸� ������ �߻����� �ʾҴٸ� �����ߴ� ���� �ٽ� ������ ���� ���ƾ� �Ѵ�.<br>
 *
 * <br>
 *
 * Content is available under Attribution-Noncommercial-No Derivative Works 3.0 Unported.
 */

#include "type.h"

#include "../itf/ubik_sem.h"
#include "../itf/ubik_signal.h"

// TODO: (Ubinos) ����
/** (���ŵ� ����) �������� Ÿ�̸� �ð� ���� ���� : �ֱ������� �ݺ��� V ����� ������ (�ֱ��� �ݺ� ���� ����) */
#define	STIMER_TIMEOPT__PERIODIC		0x00

// TODO: (Ubinos) ����
/** (���ŵ� ����) �������� Ÿ�̸� �ð� ���� ����: �ѹ��� V ����� ������ (��ȸ�� ���� ����) */
#define	STIMER_TIMEOPT__ONESHOT			0x80

/** �������� Ÿ�̸� ���� ����: �ѹ��� V ���(sem_give �Լ�)�� �����ϰ� ���� (��ȸ�� ���� ����) */
#define	STIMER_OPT__ONESHOT				0x80

/** �������� Ÿ�̸� ���� ����: �ñ׳��� ��ε�ĳ��Ʈ(signal_broadcast �Լ��� ȣ��) �� (��ε�ĳ��Ʈ ���� ����)<br>
									�� ���� ������ �������� Ÿ�̸Ӱ� �ñ׳��� �������� ����(stimer_set_signal�Լ��� ����)�Ǿ��� ��쿡�� ��ȿ�� */
#define	STIMER_OPT__BROADCAST			0x40

typedef	struct __stimer_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _stimer_tip_t;

/** �������� Ÿ�̸� ������ �� ���� */
typedef	_stimer_tip_t *	stimer_pt;

/**
 * �������� Ÿ�̸Ӹ� �����ϴ� �Լ�
 *
 * @param	stimer_p	������ �������� Ÿ�̸��� �ּҸ� ������ �������� �ּ�<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int stimer_create(stimer_pt * stimer_p);

/**
 * ��� �������� Ÿ�̸Ӹ� �����ϴ� �Լ�
 *
 * @param	stimer_p	������ ��� �������� Ÿ�̸��� �ּҰ� ����� �������� �ּ�<br>
 * 						<br>
 * 						���ſ� �����ϸ� *stimer_p�� NULL�� ��<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int stimer_delete(stimer_pt * stimer_p);

/**
 * ��� �������� Ÿ�̸Ӹ� �����ϴ� �Լ�
 *
 * @param	stimer		��� �������� Ÿ�̸� ������<br>
 * 						<br>
 *
 * @param	tick		�ֱ� (�ý��� ƽtick ��)<br>
 * 						<br>
 *
 * @param	sem			�ֱ⸶�� V ���(sem_give �Լ�)�� ������ ��� ��������<br>
 * 						<br>
 *
 * @param	option		���� ���� (STIMER_OPT__...)<br>
 * 						0: ��� �͵� �������� ����<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int stimer_set(stimer_pt stimer, unsigned int tick, sem_pt sem, unsigned int option);

/**
 * ��� �������� Ÿ�̸Ӹ� �����ϴ� �Լ� (õ���� ���� ����)
 *
 * @param	stimer		��� �������� Ÿ�̸� ������<br>
 * 						<br>
 *
 * @param	timems		�ֱ� (õ���� ����)<br>
 * 						<br>
 *
 * @param	sem			�ֱ⸶�� V ���(sem_give �Լ�)�� ������ ��� ��������<br>
 * 						<br>
 *
 * @param	option		���� ���� (STIMER_OPT__...)<br>
 * 						0: ��� �͵� �������� ����<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int stimer_setms(stimer_pt stimer, unsigned int timems, sem_pt sem, unsigned int option);

/**
 * ��� �������� Ÿ�̸Ӹ� �ñ׳��� �������� �����ϴ� �Լ�
 *
 * @param	stimer		��� �������� Ÿ�̸� ������<br>
 * 						<br>
 *
 * @param	tick		�ֱ� (�ý��� ƽtick ��)<br>
 * 						<br>
 *
 * @param	signal		�ֱ⸶�� �ñ׳��� ���� ���<br>
 * 						<br>
 *
 * @param	sigtype		�ñ׳� ���� (0 �̻�)
 *
 * @param	option		���� ���� (STIMER_OPT__...)<br>
 * 						0: ��� �͵� �������� ����<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int stimer_set_signal(stimer_pt stimer, unsigned int tick, signal_pt signal, int sigtype, unsigned int option);

/**
 * ��� �������� Ÿ�̸Ӹ� �ñ׳��� �������� �����ϴ� �Լ� (õ���� ���� ����)
 *
 * @param	stimer		��� �������� Ÿ�̸� ������<br>
 * 						<br>
 *
 * @param	timems		�ֱ� (õ���� ����)<br>
 * 						<br>
 *
 * @param	signal		�ֱ⸶�� �ñ׳��� ���� ���<br>
 * 						<br>
 *
 * @param	sigtype		�ñ׳� ���� (0 �̻�)
 *
 * @param	option		���� ���� (STIMER_OPT__...)<br>
 * 						0: ��� �͵� �������� ����<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int stimer_setms_signal(stimer_pt stimer, unsigned int timems, signal_pt signal, int sigtype, unsigned int option);

/**
 * ��� �������� Ÿ�̸Ӹ� �����ϴ� �Լ�
 *
 * @param	stimer		��� �������� Ÿ�̸� ������<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int stimer_start(stimer_pt stimer);

/**
 * ��� �������� Ÿ�̸Ӹ� ���ߴ� �Լ�
 *
 * @param	stimer		��� �������� Ÿ�̸� ������<br>
 * 						<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int stimer_stop(stimer_pt stimer);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_STIMER_H_ */
