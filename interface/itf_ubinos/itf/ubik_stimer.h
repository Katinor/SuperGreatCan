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
 * @brief ubik (Ubinos Kernel) 컴포넌트 세마포어 타이머 인터페이스
 */

/**
 * @example	stimertest00.c
 * @example	stimertest01.c
 * @example	stimertest02.c
 * @example	stimertest03.c
 */

/**
 * \page stimer_example 세마포어 타이머 예제
 *
 *         \section stimertest00_sec 간단한 세마포어 타이머 사용 예제
 *             Simple semaphore timer example
 *
 *             - <a href="stimertest00_8c-example.html">stimertest00.c</a>
 *
 *             이 예제는 일정한 주기로 간단한 메시지를 반복적으로 출력한다.<br>
 *
 * <br>
 *
 *         \section stimertest01_sec 세마포어 타이머의 주기적 알림 기능 시험
 *             Test on periodic notification function of semaphore timer
 *
 *             - <a href="stimertest01_8c-example.html">stimertest01.c</a>
 *
 *             이 예제는 세마포어 타이머의 주기적 알림 기능을 시험한다.
 *
 *             시점 1에서 태스크 2를 중간 우선순위로 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 2에서 태스크 1을 높은 우선순위로 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 3에서 태스크 1이 세마포어 타이머를 설정하고 시작한 다음, 알림 신호를 기다린다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 5가 되면 세마포어 타이머가 알림 신호를 보낸다.<br>
 *             그러면 태스크 1이 그 알림 신호를 받고 깨어나 주어진 일을 처리한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 5에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점  6이되면(주어진 일 처리를 마치면) 태스크 1이  다시 알림 신호를 기다린다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 6에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 7이 되면 세마포어 타이머가 다시 알림 신호를 보낸다.<br>
 *             그러면 태스크 1이 그 알림 신호를 받고 깨어나 다시 주어진 일을 처리한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 7에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점  8에서 태스크 1이 세마포어 타이머를 멈춘다.<br>
 *             그리고 시점 10까지를 제한 시간으로 두고 알림 신호를 기다린다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 8에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 10이 되면 태스크 1이 제한 시간이 다해서 알림 신호를 받지 못했더라도 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 10에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 11에서 태스크 1이 종료된다.<br>
 *
 *             시점 12에서 태스크 2가 종료된다.<br>
 *
 * <br>
 *
 *             \image html ubik_test_stimertest01.gif
 *
 * <br>
 *
 *         \section stimertest02_sec 세마포어 타이머의 일회성 알림 기능 시험
 *             Test on periodic notification function of semaphore timer
 *
 *             - <a href="stimertest02_8c-example.html">stimertest02.c</a>
 *
 *             이 예제는 세마포어 타이머의 일회성 알림 기능을 시험한다.
 *
 *             시점 1에서 태스크 2를 중간 우선순위로 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 2에서 태스크 1을 높은 우선순위로 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 3에서 태스크 1이 일회성 선택 사항(STIMER_OPT__ONESHOT)을 선택하고
 *             세마포어 타이머를 설정한다.<br>
 *             그리고 세마포어 타이머를 시작한 다음 알림 신호를 기다린다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 5가 되면 세마포어 타이머가 알림 신호를 보낸다.<br>
 *             그러면 태스크 1이 그 알림 신호를 받고 깨어나 주어진 일을 처리한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 5에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점  6이되면(주어진 일 처리를 마치면) 태스크 1이  시점 8까지를 제한 시간으로 두고 다시 알림 신호를 기다린다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 6에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 8이 되면 태스크 1이 제한 시간이 다해서 알림 신호를 받지 못했더라도 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 8에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 9에서 태스크 1이 종료된다.<br>
 *
 *             시점 10에서 태스크 2가 종료된다.<br>
 *
 * <br>
 *
 *             \image html ubik_test_stimertest02.gif
 *
 * <br>
 *
 *         \section stimertest03_sec 세마포어 타이머에 의한 메모리 누수 시험
 *             Test for memory leak by stimer
 *
 *             - <a href="stimertest03_8c-example.html">stimertest03.c</a>
 *
 *             이 예제는 세마포어 타이머에 의한 메모리 누수를 시험한다.<br>
 *
 *             먼저 힙heap의 할당된 메모리 총량과 메모리 블록 수를 가져와 저장한다.<br>
 *             그리고 세마포어 타이머 생성과 제거를 여러 번 반복한다.<br>
 *             그 다음 힙heap의 할당된 메모리 총량과 메모리 블럭 수를 다시 가져와 저장했던 값과 비교한다.<br>
 *             메모리 누수가 발생하지 않았다면 저장했던 값과 다시 가져온 값이 같아야 한다.<br>
 *
 * <br>
 *
 * Content is available under Attribution-Noncommercial-No Derivative Works 3.0 Unported.
 */

#include "type.h"

#include "../itf/ubik_sem.h"
#include "../itf/ubik_signal.h"

// TODO: (Ubinos) 제거
/** (제거될 예정) 세마포어 타이머 시간 선택 사항 : 주기적으로 반복해 V 명령을 수행함 (주기적 반복 선택 사항) */
#define	STIMER_TIMEOPT__PERIODIC		0x00

// TODO: (Ubinos) 제거
/** (제거될 예정) 세마포어 타이머 시간 선택 사항: 한번만 V 명령을 수행함 (일회성 선택 사항) */
#define	STIMER_TIMEOPT__ONESHOT			0x80

/** 세마포어 타이머 선택 사항: 한번만 V 명령(sem_give 함수)을 수행하고 멈춤 (일회성 선택 사항) */
#define	STIMER_OPT__ONESHOT				0x80

/** 세마포어 타이머 선택 사항: 시그널을 브로드캐스트(signal_broadcast 함수를 호출) 함 (브로드캐스트 선택 사항)<br>
									이 선택 사항은 세마포어 타이머가 시그널을 보내도록 설정(stimer_set_signal함수로 설정)되었을 경우에만 유효함 */
#define	STIMER_OPT__BROADCAST			0x40

typedef	struct __stimer_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _stimer_tip_t;

/** 세마포어 타이머 포인터 형 정의 */
typedef	_stimer_tip_t *	stimer_pt;

/**
 * 세마포어 타이머를 생성하는 함수
 *
 * @param	stimer_p	생성한 세마포어 타이머의 주소를 저장할 포인터의 주소<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int stimer_create(stimer_pt * stimer_p);

/**
 * 대상 세마포어 타이머를 제거하는 함수
 *
 * @param	stimer_p	제거할 대상 세마포어 타이머의 주소가 저장된 포인터의 주소<br>
 * 						<br>
 * 						제거에 성공하면 *stimer_p는 NULL이 됨<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int stimer_delete(stimer_pt * stimer_p);

/**
 * 대상 세마포어 타이머를 설정하는 함수
 *
 * @param	stimer		대상 세마포어 타이머 포인터<br>
 * 						<br>
 *
 * @param	tick		주기 (시스템 틱tick 수)<br>
 * 						<br>
 *
 * @param	sem			주기마다 V 명령(sem_give 함수)을 수행할 대상 세마포어<br>
 * 						<br>
 *
 * @param	option		선택 사항 (STIMER_OPT__...)<br>
 * 						0: 어느 것도 선택하지 않음<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int stimer_set(stimer_pt stimer, unsigned int tick, sem_pt sem, unsigned int option);

/**
 * 대상 세마포어 타이머를 설정하는 함수 (천분의 일초 단위)
 *
 * @param	stimer		대상 세마포어 타이머 포인터<br>
 * 						<br>
 *
 * @param	timems		주기 (천분의 일초)<br>
 * 						<br>
 *
 * @param	sem			주기마다 V 명령(sem_give 함수)을 수행할 대상 세마포어<br>
 * 						<br>
 *
 * @param	option		선택 사항 (STIMER_OPT__...)<br>
 * 						0: 어느 것도 선택하지 않음<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int stimer_setms(stimer_pt stimer, unsigned int timems, sem_pt sem, unsigned int option);

/**
 * 대상 세마포어 타이머를 시그널을 보내도록 설정하는 함수
 *
 * @param	stimer		대상 세마포어 타이머 포인터<br>
 * 						<br>
 *
 * @param	tick		주기 (시스템 틱tick 수)<br>
 * 						<br>
 *
 * @param	signal		주기마다 시그널을 보낼 대상<br>
 * 						<br>
 *
 * @param	sigtype		시그널 종류 (0 이상)
 *
 * @param	option		선택 사항 (STIMER_OPT__...)<br>
 * 						0: 어느 것도 선택하지 않음<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int stimer_set_signal(stimer_pt stimer, unsigned int tick, signal_pt signal, int sigtype, unsigned int option);

/**
 * 대상 세마포어 타이머를 시그널을 보내도록 설정하는 함수 (천분의 일초 단위)
 *
 * @param	stimer		대상 세마포어 타이머 포인터<br>
 * 						<br>
 *
 * @param	timems		주기 (천분의 일초)<br>
 * 						<br>
 *
 * @param	signal		주기마다 시그널을 보낼 대상<br>
 * 						<br>
 *
 * @param	sigtype		시그널 종류 (0 이상)
 *
 * @param	option		선택 사항 (STIMER_OPT__...)<br>
 * 						0: 어느 것도 선택하지 않음<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int stimer_setms_signal(stimer_pt stimer, unsigned int timems, signal_pt signal, int sigtype, unsigned int option);

/**
 * 대상 세마포어 타이머를 시작하는 함수
 *
 * @param	stimer		대상 세마포어 타이머 포인터<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int stimer_start(stimer_pt stimer);

/**
 * 대상 세마포어 타이머를 멈추는 함수
 *
 * @param	stimer		대상 세마포어 타이머 포인터<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int stimer_stop(stimer_pt stimer);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_STIMER_H_ */
