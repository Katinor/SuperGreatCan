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

#ifndef ITF_UBIK_MSGQ_H_
#define ITF_UBIK_MSGQ_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubik_msgq.h
 *
 * @brief ubik (Ubinos Kernel) 컴포넌트 메시지큐 인터페이스
 */

/**
 * @example	msgqtest00.c
 * @example	msgqtest01.c
 * @example	msgqtest02.c
 * @example	msgqtest03.c
 * @example	msgqtest04.c
 */

/**
 * \page msgq_example 메시지큐 예제
 *
 *         \section msgqtest00_sec 간단한 메시지큐 사용 예제
 *             Simple message queue example
 *
 *             - <a href="msgqtest00_8c-example.html">msgqtest00.c</a>
 *
 *             이 예제는 대표적인 태스크간 동기화 문제인 생산자 소비자 문제를 메시지큐를 사용해 해결한다.
 *             생산자(태스크 1)는 메시지큐를 사용해 메시지를 소비자들(태스크 2와 3)에게 전달한다.
 *             그리고 이와 동시에 소비자들(태스크 2와 3)은 역시 메시지큐를 사용해 메시지 소비 시점을 생산자(태스크 1)의 메시지 생산 시점에 동기시킨다.<br>
 *
 * <br>
 *
 *         \section msgqtest01_sec 메시지큐 기본 기능 시험
 *             Test on basic functions of message queue
 *
 *             - <a href="msgqtest01_8c-example.html">msgqtest01.c</a>
 *
 *             이 예제는 아래에 나열된 메시지큐의 기본 기능들을 시험한다.
 * <pre>
 *     메시지큐로 메시지를 보내는 기능 (msgq_send 함수)
 *     메시지큐로부터 메시지를 받는 기능 (msgq_receive 함수)
 * </pre>
 *
 *             시점 1에서 태스크 1을 높은 우선순위로, 태스크 2를 중간 우선순위로 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 2에서 태스크 1이 메시지 받기를 시도한다.<br>
 *             그러면 메시지큐가 비어있으므로 태스크 1은 잠든다(대기blocked 상태가 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 3에서 태스크 2가 메시지를 보낸다.<br>
 *             그러면 태스크 1이 메시지를 받고 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 4에서 태스크 1이 시점 7까지 휴면sleep을 시도한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 4에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 5에서 태스크 2가 메시지를 보낸다.<br>
 *             메시지를 기다리는 태스크가 없으므로 메시지는 메시지큐에 쌓인다. (메시지큐에 들어 있는 메시지 수는 1이 된다.)<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 5에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 6에서 태스크 2가 메시지를 다시 한번 보낸다.<br>
 *             메시지를 기다리는 태스크가 없으므로 메시지는 메시지큐에 쌓인다. (메시지큐에 들어 있는 메시지 수는 2가 된다.)<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 6에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 7이 되면 태스크 1이 휴면sleep 시간이 다해서 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 7에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 8에서 태스크 1이 메시지 받기를 시도한다.<br>
 *             메시지큐에 메시지가 쌓여 있으므로 태스크 1이 쌓여있는 메시지 하나를 받는다. (메시지큐에 들어 있는 메시지 수는 1이 된다.)<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 8에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 9에서 태스크 1이 메시지 받기를 다시 한번 시도한다.<br>
 *             메시지큐에 메시지가 쌓여 있으므로 태스크 1이 쌓여있는 메시지 하나를 받는다. (메시지큐에 들어 있는 메시지 수는 0이 된다.)<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 9에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 10에서 태스크 1이 시점 11까지를 제한 시간으로 두고 메시지 받기를 시도한다.<br>
 *             그러면 메시지큐가 비어있으므로 태스크 1은 잠든다(대기blocked 상태가 된다).<br>
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
 *             \image html ubik_test_msgqtest01.gif
 *
 *         \section msgqtest04_sec 메시지큐에 의한 메모리 누수 시험
 *             Test for memory leak by message queue
 *
 *             - <a href="msgqtest04_8c-example.html">msgqtest04.c</a>
 *
 *             이 예제는 메시지큐에 의한 메모리 누수를 시험한다.<br>
 *             시험 과정은 다음과 같다.<br>
 *
 *             먼저 힙heap의 할당된 메모리 총량과 메모리 블록 수를 가져와 저장한다.<br>
 *             그리고 메시지큐 생성과 제거를 여러 번 반복한다.<br>
 *             그 다음 힙heap의 할당된 메모리 총량과 메모리 블럭 수를 다시 가져와 저장했던 값과 비교한다.<br>
 *             메모리 누수가 발생하지 않았다면 저장했던 값과 다시 가져온 값이 같아야 한다.<br>
 *
 * <br>
 *
 * Content is available under Attribution-Noncommercial-No Derivative Works 3.0 Unported.
 */

#include "type.h"

#include "../itf/ubik_task.h"

/** 메시지큐 선택 사항: 카운터가 최대값을 넘어서는 것을 오류로 간주하지 않음 (최대값 초과 오류를 무시하는 선택 사항) */
#define	MSGQ_OPT__IGNOREOVERFLOW		0x80

typedef	struct __msgq_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _msgq_tip_t;

/** 메시지큐 포인터 형 정의 */
typedef	_msgq_tip_t *	msgq_pt;

/**
 * 메시지큐를 생성하는 함수
 *
 * @param	msgq_p		생성한 메시지큐의 주소를 저장할 포인터의 주소<br>
 * 						<br>
 *
 * @param	msgsize		메시지 크기<br>
 * 						<br>
 *
 * @param	maxcount	메시지큐에 쌓일 수 있는 최대 메시지 수<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int msgq_create(msgq_pt * msgq_p, unsigned int msgsize, unsigned int maxcount);

/**
 * 메시지큐를 생성하는 함수 확장형
 *
 * @param	msgq_p		생성한 메시지큐의 주소를 저장할 포인터의 주소<br>
 * 						<br>
 *
 * @param	msgsize		메시지 크기<br>
 * 						<br>
 *
 * @param	maxcount	메시지큐에 쌓일 수 있는 최대 메시지 수<br>
 * 						<br>
 *
 * @param	option		선택 사항 (MSGQ_OPT__...)<br>
 * 						0: 어느 것도 선택하지 않음<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int msgq_create_ext(msgq_pt * msgq_p, unsigned int msgsize, unsigned int maxcount, unsigned int option);

/**
 * 대상 메시지큐를 제거하는 함수
 *
 * @param	msgq_p		제거할 대상 메시지큐의 주소가 저장된 포인터의 주소<br>
 * 						<br>
 * 						제거에 성공하면 *msgq_p는 NULL이 됨<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int msgq_delete(msgq_pt * msgq_p);

/**
 * 대상 메시지큐로부터 메시지를 받는 함수
 *
 * @param	msgq		대상 메시지큐 포인터<br>
 * 						<br>
 *
 * @param	msg			받은 메시지를 저장할 버퍼<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int msgq_receive(msgq_pt msgq, unsigned char * msg);

/**
 * 제한 시간을 두고 대상 메시지큐로부터 메시지를 받는 함수
 *
 * @param	msgq		대상 메시지큐 포인터<br>
 * 						<br>
 *
 * @param	msg			받은 메시지를 저장할 버퍼<br>
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
int msgq_receive_timed(msgq_pt msgq, unsigned char * msg, unsigned int tick);

/**
 * 제한 시간을 두고 대상 메시지큐로부터 메시지를 받는 함수 (천분의 일초 단위)
 *
 * @param	msgq		대상 메시지큐 포인터<br>
 * 						<br>
 *
 * @param	msg			받은 메시지를 저장할 버퍼<br>
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
int msgq_receive_timedms(msgq_pt msgq, unsigned char * msg, unsigned int timems);

/**
 * 대상 메시지큐로 메시지를 보내는 함수
 *
 * @param	msgq		대상 메시지큐 포인터<br>
 * 						<br>
 *
 * @param	msg			보낼 메시지가 저장된 버퍼<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__OVERFLOWED: 최대 메시지 수 초과<br>
 */
int msgq_send(msgq_pt msgq, unsigned char * msg);

/**
 * 대상 메시지큐에 쌓여있는 메시지를 모두 제거하는 함수
 *
 * @param	msgq		대상 메시지큐 포인터<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int msgq_clear(msgq_pt msgq);

/**
 * 대상 메시지큐로 메시지를 보내는 (메시지를 기다리는 최상위 우선순위 태스크의우선순위를 상속받을) 태스크를 설정하는 함수
 *
 * @param	msgq		대상 메시지큐 포인터<br>
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
int msgq_setsender(msgq_pt msgq, task_pt task);

/**
 * 대상 메시지큐에 쌓여있는 메시지 수를 돌려주는 함수
 *
 * @param	msgq		대상 메시지큐 포인터<br>
 * 						<br>
 *
 * @param	count_p		메시지큐에 쌓여있는 메시지 수를 저장할 변수의 주소<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int msgq_getcount(msgq_pt msgq, unsigned int * count_p);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_MSGQ_H_ */
