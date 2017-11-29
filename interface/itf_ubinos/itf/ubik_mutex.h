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

#ifndef ITF_UBIK_MUTEX_H_
#define ITF_UBIK_MUTEX_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubik_mutex.h
 *
 * @brief ubik (Ubinos Kernel) 컴포넌트 뮤텍스 인터페이스
 */

/**
 * @example	mutextest00.c
 * @example	mutextest01.c
 * @example	mutextest02.c
 * @example	mutextest03.c
 * @example	mutextest04.c
 */

/**
 * \page mutex_example 뮤텍스 예제
 *
 *         \section mutextest00_sec 간단한 뮤텍스 사용 예제
 *             Simple mutex example
 *
 *             - <a href="mutextest00_8c-example.html">mutextest00.c</a>
 *
 *             이 예제는 하위 예제 두개로 구성된다.
 *             하위 예제들은 각각 데이터 하나를 경쟁적으로 사용하는 태스크를 두개 생성한다.
 *             첫 번째 하위 예제는 상호 배제적mutual exclusive이 아닌 방식으로 데이터를 사용하며,
 *             두 번째 하위 예제는 뮤텍스를 이용해 상호 배제적mutual exclusive 방식으로 데이터를 사용한다.
 *             각 하위 예제의 첫 번째 태스크(태스크 1)는 루프loop 반복 횟수 만큼  데이터에 1을 더하며,
 *             두 번째 태스크(태스크 2)는 루프loop 반복 횟수 만큼 데이터에 1을 뺀다.
 *             데이터의 초기값은 0이며, 두 태스크의 루프loop 반복 횟수는 동일하다.
 *             따라서 데이터의 최종값은 0이어야 한다.
 *             그러나 첫 번째 하위 예제는 경쟁 상태race condition 문제를 발생시킨다. 따라서 데이터의 최종값은 0이 아닌 값이 된다.
 *             두 번째 하위 예제는 뮤텍스를 이용해 상호 배제적mutual exclusive 방식으로 데이터를 사용하기 때문에
 *             경쟁 상태race condition 문제를 발생시키지 않는다. 따라서 데이터의 최종값은 0이 된다.<br>
 *
 * <br>
 *
 *         \section mutextest01_sec 뮤텍스 기본 기능 시험
 *             Test on basic functions of mutex
 *
 *             - <a href="mutextest01_8c-example.html">mutextest01.c</a>
 *
 *             이 예제는 아래에 나열된 뮤텍스의 기본 기능들을 시험한다.
 * <pre>
 *     잠그기 기능
 *     잠금 해제 기능
 * </pre>
 *
 *             시점 1에서 중간 우선순위로 태스크 2를 생성한다.<br>
 *             그리고 생성된 태스크 2는 뮤텍스를 잠근다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 2에서 높은 우선순위로 태스크 1을 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 3에서 태스크 1이 뮤텍스 잠그기를 시도한다.<br>
 *             그러면 뮤텍스가 이미 태스크 2에 의해 잠겨졌으므로 태스크 1은 잠든다(대기blocked 상태가 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 4에서 태스크 2가 뮤텍스 잠그기를 다시 한번 시도한다.<br>
 *             뮤텍스가 이미 잠겨졌지만, 잠근 태스크가 태스크 2 자신이므로 뮤텍스는 이 잠금 시도를 허용한다.<br>
 *             따라서 여기까지 성공적으로 진행되었다면 확인 지점 4에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 5에서 태스크 2가 뮤텍스 잠금 해제를 시도한다.<br>
 *             뮤텍스는 태스크 2에 의해 두번 잠겨졌다.<br>
 *             따라서 이 시도가 성공하더라도 뮤텍스는 여전히 잠겨진 상태에 머무른다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 5에서 태스크2가 수행 중이어야 한다.<br>
 *
 *             시점 6에서 태스크 2가 다시 한번 뮤텍스 잠금 해제를 시도한다.<br>
 *             그러면 뮤텍스는 잠겨진 상태에서 벗어난다.<br>
 *             뮤텍스가 잠금 해제되면 태스크 1이 뮤텍스를 잠그고 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 6에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 7에서 태스크 1이 뮤텍스를 잠금 해제하고, 시점 8까지 휴면sleep을 시도한다.<br>
 *             그러면 태스크 2가 깨어나서 뮤텍스를 잠근다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 7에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 8이 되면 태스크 1이 휴면sleep 시간이 다해서 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 8에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 9에서 태스크 1이 시점 10까지를 제한 시간으로 두고 뮤텍스 잠그기를 시도한다.<br>
 *             그러면 뮤텍스가 이미 태스크 2에 의해 잠겨졌으므로 태스크 1은 잠든다(대기blocked 상태가 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 9에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 10이 되면 태스크 1이 제한 시간이 다해서 뮤텍스를 잠그지 못했더라도 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 10에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 11에서 태스크 1이 종료된다.<br>
 *
 *             시점 12에서 태스크 2가 뮤텍스를 잠금 해제하고 종료된다.<br>
 *
 * <br>
 *
 *             \image html ubik_test_mutextest01.gif
 *
 * <br>
 *
 *         \section mutextest02_sec 뮤텍스에 의한 우선순위 역전priority inversion 상황 시험
 *             Test for priority inversion situation by mutex
 *
 *             - <a href="mutextest02_8c-example.html">mutextest02.c</a>
 *
 *             이 예제는 우선순위 상속priority inheritance 특성이 비활성화된 뮤텍스를 사용해서
 *             뮤텍스에 의한 우선순위 역전priority inversion 상황을 시험한다.<br>
 *             우선순위 상속priority inheritance 특성이 비활성화된 뮤텍스는
 *             뮤텍스 생성 함수 확장형(mutex_create_ext 함수)을
 *             우선순위 상속priority inheritance 특성 비활성화 선택 사항(MUTEX_OPT__NOPRIORITYINHERITANCE)과
 *             함께 호출하면 생성된다.<br>
 *
 *             시점 1에서 낮은 우선순위로 태스크 3을 생성한다.<br>
 *             그리고 생성된 태스크 3은 뮤텍스를 잠근다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 3이 수행 중이어야 한다.<br>
 *
 *             시점 2에서 높은 우선순위로 태스크 1을 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 3에서 태스크 1이 뮤텍스 잠그기를 시도한다.<br>
 *             그러면 뮤텍스가 이미 태스크 3에 의해 잠겨졌으므로 태스크 1은 잠든다(대기blocked 상태가 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 3이 수행 중이어야 한다.<br>
 *
 *             시점 4에서 중간 우선순위로 태스크 2를 생성한다.<br>
 *             생성된 태스크 2의 우선순위가 태스크 3의 우선순위보다 높다.<br>
 *             따라서 여기까지 성공적으로 진행되었다면 확인 지점 4에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 5에서 태스크 2가 종료된다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 5에서 태스크 3이 수행 중이어야 한다.<br>
 *
 *             <b>
 *             그런데 여기까지 성공적으로 진행되었더라도 시점 4와 5 사이의 상황은 이상하다.
 *             태스크 2가 우선순위가 더 높은 태스크 1보다 먼저 수행되었다.
 *             이에 앞서 발생한 태스크 3이 태스크 1보다 먼저 수행되는 상황은
 *             태스크 1이 태스크 3이 가진 자원(뮤텍스)을 필요로 하기 때문에 발생하는 어쩔 수 없는 상황이다.
 *             그러나 태스크 2는 태스크 1과 아무런 관계가 없다.
 *             그럼에도 불구하고 태스크 2가 우선순위가 더 높은 태스크 1보다 먼저 수행되었다.
 *             이 이상한 상황은 우선순위 역전priority inversion 상황이라 불리며, 문제 상황으로 간주 된다.<br>
 *             </b>
 *
 *             시점 6에서 태스크 3이 뮤텍를 잠금 해제한다.<br>
 *             그러면 태스크 1이 뮤텍스를 잠그고  깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 6에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 7에서 태스크 1이 뮤텍스를 잠금 해제하고 종료된다.<br>
 *
 *             시점 8에서 태스크 3이 종료된다.<br>
 *
 * <br>
 *
 *             \image html ubik_test_mutextest02.gif
 *
 * <br>
 *
 *         \section mutextest03_sec 우선순위 역전priority inversion 상황을 방지하는 뮤텍스의 우선순위 상속priority inheritance 특성 시험
 *             Test on priority inheritance feature of mutex preventing priority inversion situation
 *
 *             - <a href="mutextest03_8c-example.html">mutextest03.c</a>
 *
 *             이 예제는 우선순위 역전priority inversion 상황을 방지하는 뮤텍스의 우선순위 상속priority inheritance 특성을 시험한다.<br>
 *
 *             시점 1에서낮은 우선순위로 태스크 3을 생성한다.<br>
 *             그리고 생성된 태스크 3은 뮤텍스를 잠근다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 3이 수행 중이어야 한다.<br>
 *
 *             시점 2에서 높은 우선순위로 태스크 1을 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 3에서 태스크 1이 뮤텍스 잠그기를 시도한다.<br>
 *             그러면 뮤텍스가 이미 태스크 3에 의해 잠겨졌으므로 태스크 1은 잠든다(대기blocked 상태가 된다).<br>
 *             그리고 태스크 3이 태스크 1의 우선순위를 상속 받는다. (태스크 3의 우선순위가 태스크 1의 우선순위와 같아진다.)<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 3이 수행 중이어야 한다.<br>
 *
 *             시점 4에서 중간 우선순위로 태스크 2를 생성한다.<br>
 *             태스크 3이 태스크 1의 우선순위를 상속 받았기 때문에 태스크 3의 우선순위가 태스크 2의 우선순위보다 높다.<br>
 *             따라서 여기까지 성공적으로 진행되었다면 확인 지점 4에서 태스크 3이 수행 중이어야 한다.<br>
 *
 *             <b>
 *             뮤텍스에 의한 우선순위 역전priority inversion 상황 시험 예제에서는 이 시점에서
 *             우선순위 역전priority inversion 상황이 발생했다.
 *             그러나 이 예제에서는 우선순위 상속priority inheritance 특성을 가진 뮤텍스를 사용하기 때문에
 *             그러한 문제 상황이 발생하지 않는다.<br>
 *             </b>
 *
 *             시점 5에서 태스크 3이 뮤텍스를 잠금 해제 한다.<br>
 *             그러면 태스크 1이 뮤텍스를 잠그고 깨어난다.<br>
 *             그리고 태스크 3의 우선순위는 본래 우선순위로 복원 된다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 5에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 6에서 태스크 1이 뮤텍스를 잠금 해제하고 종료된다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 6에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 7에서 태스크 2가 종료된다.<br>
 *
 *             시점 8에서 태스크 3이 종료된다.<br>
 *
 * <br>
 *
 *             \image html ubik_test_mutextest03.gif
 *
 * <br>
 *
 *         \section mutextest04_sec 뮤텍스에 의한 메모리 누수 시험
 *             Test for memory leak by mutex
 *
 *             - <a href="mutextest04_8c-example.html">mutextest04.c</a>
 *
 *             이 예제는 뮤텍스에 의한 메모리 누수를 시험한다.<br>
 *             시험 과정은 다음과 같다.<br>
 *
 *             먼저 힙heap의 할당된 메모리 총량과 메모리 블록 수를 가져와 저장한다.<br>
 *             그리고 뮤텍스 생성과 제거를 여러 번 반복한다.<br>
 *             그 다음 힙heap의 할당된 메모리 총량과 메모리 블럭 수를 다시 가져와 저장했던 값과 비교한다.<br>
 *             메모리 누수가 발생하지 않았다면 저장했던 값과 다시 가져온 값이 같아야 한다.<br>
 *
 * <br>
 *
 * Content is available under Attribution-Noncommercial-No Derivative Works 3.0 Unported.
 */

#include "type.h"

/** 뮤텍스 선택 사항: 뮤텍스의 우선순위 상속priority inheritance 특성을 비활성화 함 (우선순위 상속priority inheritance 특성 비활성화 선택 사항) */
#define	MUTEX_OPT__NOPRIORITYINHERITANCE	0x80

typedef	struct __mutex_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _mutex_tip_t;

/** 뮤텍스 포인터 형 정의 */
typedef	_mutex_tip_t *	mutex_pt;

/**
 * 뮤텍스를 생성하는 함수
 *
 * @param	mutex_p		생성한 뮤텍스의 주소를 저장할 포인터의 주소<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int mutex_create(mutex_pt * mutex_p);

/**
 * 뮤텍스를 생성하는 함수 확장형
 *
 * @param	mutex_p		생성한 뮤텍스의 주소를 저장할 포인터의 주소<br>
 * 						<br>
 *
 * @param	option		선택 사항 (MUTEX_OPT__...)<br>
 * 						0: 어느 것도 선택하지 않음<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int mutex_create_ext(mutex_pt * mutex_p, unsigned int option);

/**
 * 대상 뮤텍스를 제거하는 함수
 *
 * @param	mutex_p		제거할 대상 뮤텍스의 주소가 저장된 포인터의 주소<br>
 * 						<br>
 * 						제거에 성공하면 *mutex_p는 NULL이 됨<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int mutex_delete(mutex_pt * mutex_p);

/**
 * 대상 뮤텍스를 잠그는 함수
 *
 * @param	mutex		대상 뮤텍스 포인터<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int mutex_lock(mutex_pt mutex);

/**
 * 제한 시간을 두고 대상 뮤텍스 잠그기를 시도하는 함수
 *
 * @param	mutex		대상 뮤텍스 포인터<br>
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
int mutex_lock_timed(mutex_pt mutex, unsigned int tick);

/**
 * 제한 시간을 두고 대상 뮤텍스 잠그기를 시도하는 함수 (천분의 일초 단위)
 *
 * @param	mutex		대상 뮤텍스 포인터<br>
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
int mutex_lock_timedms(mutex_pt mutex, unsigned int timems);

/**
 * 대상 뮤텍스를 잠금 해제하는 함수
 *
 * @param	mutex		대상 뮤텍스 포인터<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int mutex_unlock(mutex_pt mutex);

/**
 * 뮤텍스가 잠겨져 있는지 여부를 돌려주는 함수
 *
 * @param	mutex		대상 뮤텍스 포인터<br>
 * 						<br>
 *
 * @return	  1: 잠겨져 있음<br>
 *            0: 잠겨져  있지 않음<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int mutex_islocked(mutex_pt mutex);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_MUTEX_H_ */
