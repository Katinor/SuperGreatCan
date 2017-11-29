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

#ifndef ITF_UBIK_SEM_H_
#define ITF_UBIK_SEM_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubik_sem.h
 *
 * @brief ubik (Ubinos Kernel) 컴포넌트 세마포어 인터페이스
 */

/**
 * @example	semtest00.c
 * @example	semtest01.c
 * @example	semtest02.c
 * @example	semtest03.c
 * @example	semtest04.c
 */

/**
 * \page sem_example 세마포어 예제
 *
 *         \section semtest00_sec 간단한 세마포어 사용 예제
 *             Simple semaphore example
 *
 *             - <a href="semtest00_8c-example.html">semtest00.c</a>
 *
 *             이 예제는 대표적인 태스크간 동기화 문제인 생산자 소비자 문제를 세마포어를 사용해 해결한다.
 *             생산자(태스크 1)는 자원이 생산되었음을 알리는 신호를 세마포어를 통해 소비자들(태스크 2와 3)에게 전달하고,
 *             소비자들(태스크 2와 3)은 그 신호에 동기되어 자원을 소비한다.
 *             다시 말해 소비자들이 자원 소비 시점을 생산자의 자원 생산 시점에 맞춘다.<br>
 *
 * <br>
 *
 *         \section semtest01_sec 세마포어 기본 기능 시험
 *             Test on basic functions of semaphore
 *
 *             - <a href="semtest01_8c-example.html">semtest01.c</a>
 *
 *             이 예제는 아래에 나열된 세마포어의 기본 기능들을 시험한다.
 * <pre>
 *     세마포어 P 명령(sem_take 함수)
 *     세마포어 V 명령(sem_give 함수)
 * </pre>
 *
 *             시점 1에서 태스크 1을 높은 우선순위로, 태스크 2를 중간 우선순위로 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 2에서 태스크 1이 P 명령(sem_take 함수)을 수행한다.<br>
 *             그러면 세마포어의 값이 0이므로 태스크 1이 세마포어의 대기 태스크 큐에 추가되고 잠든다(대기blocked 상태가 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 3에서 태스크 2가 V 명령(sem_give 함수)을 수행한다.<br>
 *             그러면 세마포어의 대기 태스크 큐가 비어있지 않으므로 대기 태스크 큐에 들어있던 태스크 1이 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 4에서 태스크 1이 시점 7까지를 제한 시간으로 두고 휴면sleep을 시도한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 4에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 5에서 태스크 2가 V 명령(sem_give 함수)을 수행한다.<br>
 *             그러면 세마포어의 대기 태스크 큐가 비어있으므로 세마포어의 값이 1 증가한다(1이 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 5에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 6에서 태스크 2가 V 명령(sem_give 함수)을 다시 한번 수행한다.<br>
 *             그러면 세마포어의 대기 태스크 큐가 비어있으므로 세마포어의 값이 다시  1 증가한다(2가 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 6에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 7이 되면 태스크 1이 휴면sleep 시간이 다해서 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 7에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 8에서 태스크 1이 P 명령(sem_take 함수)을 수행한다.<br>
 *             그러면 세마포어의 값이 0이 아니므로(2이므로) 1 감소한다(1이 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 8에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 9에서 태스크 1이 P 명령(sem_take 함수)을 다시 한번 수행한다.<br>
 *             그러면 세마포어의 값이 0이 아니므로(1이므로) 1 감소한다(0이 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 9에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 10에서 태스크 1이 시점 11까지를 제한 시간으로 두고 P 명령(sem_take 함수)을 다시 한번 수행한다.<br>
 *             그러면 세마포어의 값이 0이므로 태스크 1이 세마포어의 대기 태스크 큐에 추가되고 잠든다(대기blocked 상태가 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 10에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 11이 되면 태스크 1이 제한 시간이 다해서 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 11에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 12에서 태스크 1이 종료된다.<br>
 *
 *             시점 13에서 태스크 2가 종료된다.<br>
 *
 * <br>
 *
 *             \image html ubik_test_semtest01.gif
 *
 *         \section semtest04_sec 세마포어에 의한 메모리 누수 시험
 *             Test for memory leak by semaphore
 *
 *             - <a href="semtest04_8c-example.html">semtest04.c</a>
 *
 *             이 예제는 세마포어에 의한 메모리 누수를 시험한다.<br>
 *             시험 과정은 다음과 같다.<br>
 *
 *             먼저 힙heap의 할당된 메모리 총량과 메모리 블록 수를 가져와 저장한다.<br>
 *             그리고 세마포어 생성과 제거를 여러 번 반복한다.<br>
 *             그 다음 힙heap의 할당된 메모리 총량과 메모리 블럭 수를 다시 가져와 저장했던 값과 비교한다.<br>
 *             메모리 누수가 발생하지 않았다면 저장했던 값과 다시 가져온 값이 같아야 한다.<br>
 *
 * <br>
 *
 * Content is available under Attribution-Noncommercial-No Derivative Works 3.0 Unported.
 */

#include "type.h"

#include "../itf/ubik_task.h"

/** 세마포어 선택 사항: 세마포어 값이 최대값을 넘어서는 것을 오류로 간주하지 않음 (최대값 초과 오류를 무시하는 선택 사항) */
#define	SEM_OPT__IGNOREOVERFLOW		0x80

typedef	struct __sem_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _sem_tip_t;

/** 세마포어 포인터 형 정의 */
typedef	_sem_tip_t *	sem_pt;

/**
 * 카운팅counting 세마포어를 생성하는 함수
 *
 * 세마포어의 초기값은 0이 되고, 최대값은 UINT_MAX가 됨
 *
 * @param	sem_p		생성한 세마포어의 주소를 저장할 포인터의 주소<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int sem_create(sem_pt * sem_p);

/**
 * 이진binary 세마포어를 생성하는 함수
 *
 * 세마포어의 초기값은 0이 되고, 최대값은 1이 되며, 최대값 초과 오류를 무시하는 선택 사항(SEM_OPT__IGNOREOVERFLOW)이 선택 됨
 *
 * @param	sem_p		생성한 세마포어의 주소를 저장할 포인터의 주소<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int semb_create(sem_pt * sem_p);

/**
 * 세마포어를 생성하는 함수 확장형
 *
 * @param	sem_p		생성한 세마포어의 주소를 저장할 포인터의 주소<br>
 * 						<br>
 *
 * @param	initcount	초기값<br>
 * 						<br>
 *
 * @param	maxcount	최대값<br>
 * 						<br>
 *
 * @param	option		선택 사항 (SEM_OPT__...)<br>
 * 						0: 어느 것도 선택하지 않음<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int sem_create_ext(sem_pt * sem_p, unsigned int initcount, unsigned int maxcount, unsigned int option);

/**
 * 대상 세마포어를 제거하는 함수
 *
 * @param	sem_p		제거할 대상 세마포어의 주소가 저장된 포인터의 주소<br>
 * 						<br>
 * 						제거에 성공하면 *sem_p는 NULL이 됨<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int sem_delete(sem_pt * sem_p);

/**
 * 대상 세마포어에 P 명령을 수행하는 함수
 *
 * @param	sem			대상 세마포어 포인터<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int sem_take(sem_pt sem);

/**
 * 제한 시간을 두고 대상 세마포어에 P 명령을 수행하는 함수
 *
 * @param	sem			대상 세마포어 포인터<br>
 * 						<br>
 *
 * @param	tick		제한 시간 (시스템 틱tick 수)<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__TIMEOUT		: 제한 시간 초과<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int sem_take_timed(sem_pt sem, unsigned int tick);

/**
 * 제한 시간을 두고 대상 세마포어에 P 명령을 수행하는 함수 (천분의 일초 단위)
 *
 * @param	sem			대상 세마포어 포인터<br>
 * 						<br>
 *
 * @param	timems		제한 시간 (천분의 일초)<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__TIMEOUT		: 제한 시간 초과<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int sem_take_timedms(sem_pt sem, unsigned int timems);

/**
 * 대상 세마포어에 V 명령을 수행하는 함수
 *
 * @param	sem			대상 세마포어 포인터<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__OVERFLOWED: 최대값 초과<br>
 */
int sem_give(sem_pt sem);

/**
 * 대상 세마포어의 값을 0으로 만드는 함수
 *
 * @param	sem			대상 세마포어 포인터<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int sem_clear(sem_pt sem);

/**
 * 대상 세마포어에 V 명령을 수행하는(세마포어를 기다리는 최상위 우선순위 태스크의 우선순위를 상속받을) 태스크를 설정하는 함수
 *
 * @param	sem			대상 세마포어 포인터<br>
 * 						<br>
 *
 * @param	task		대상 태스크 포인터<br>
 * 						NULL: 등록 해제<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int sem_setsender(sem_pt sem, task_pt task);

/**
 * 대상 세마포어의 값을 돌려주는 함수
 *
 * @param	sem			대상 세마포어 포인터<br>
 * 						<br>
 *
 * @param	count_p		세마포어의 값을 저장할 변수의 주소<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int sem_getcount(sem_pt sem, unsigned int * count_p);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_SEM_H_ */
