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

#ifndef ITF_UBIK_TASK_H_
#define ITF_UBIK_TASK_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubik_task.h
 *
 * @brief ubik (Ubinos Kernel) 컴포넌트 태스크 인터페이스
 */

/**
 * @example	tasktest00.c
 * @example	tasktest01.c
 * @example	tasktest02.c
 * @example	tasktest03.c
 * @example	tasktest04.c
 * @example	tasktest05.c
 * @example	tasktest06.c
 * @example	tasktest07.c
 * @example	tasktest08.c
 * @example	tasktest09.c
 */

/**
 * \page task_example 태스크 예제
 *
 *         \section tasktest00_sec 간단한 태스크 사용 예제
 *             Simple task example
 *
 *             - <a href="tasktest00_8c-example.html">tasktest00.c</a>
 *
 *             이 예제는 간단한 메시지를 반복적으로 출력하는 태스크를 하나 생성한다.<br>
 *
 *         <br>
 *
 *         \section tasktest01_sec 태스크의 기본 기능 시험
 *             Test on basic functions of task
 *
 *             - <a href="tasktest01_8c-example.html">tasktest01.c</a>
 *
 *             이 예제는 아래에 나열된 태스크의 기본 기능을 시험한다.
 * <pre>
 *     태스크를 생성하는 기능
 *     태스크가 종료될 때까지 기다리는 기능
 * </pre>
 *
 *         <br>
 *
 *         \section tasktest02_sec 태스크의 휴면sleep 기능 시험
 *             Test on sleep function of task
 *
 *             - <a href="tasktest02_8c-example.html">tasktest02.c</a>
 *
 *             이 예제는 태스크의 휴면sleep 기능을 시험한다.<br>
 *             시험 과정은 다음과 같다.<br>
 *
 *             시점 1에서 태스크 1을 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 2에서 태스크 1이 시점 3까지 휴면sleep을 시도한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 1이 수행 중이 아니어야(대기block 상태여야) 한다.<br>
 *
 *             시점 3이되면 태스크 1이 휴면sleep 시간이 다해서 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 4에서 태스크 1이 종료된다.<br>
 *
 *             \image html ubik_test_tasktest02.gif
 *
 *         <br>
 *
 *         \section tasktest03_sec 태스크의 중지suspend 및 재시작resume 기능 시험
 *             Test on suspend and resume function of task
 *
 *             - <a href="tasktest03_8c-example.html">tasktest03.c</a>
 *
 *             이 예제는 태스크의 중지suspend 및 재시작resume 기능을 시험한다.<br>
 *             시험 과정은 다음과 같다.<br>
 *
 *             시점 1에서 태스크 1을 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 2에서 태스크 1을 중지suspend한다. <br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 1이 수행 중이 아니어야(중지suspended 상태여야) 한다.<br>
 *
 *             시점 3에서 태스크 1을 재시작resume한다. <br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 4에서 태스크 1이 스스로 중지suspend한다. <br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 4에서 태스크 1이 수행 중이 아니어야(중지suspended 상태여야) 한다.<br>
 *
 *             시점 5에서 태스크 1을 재시작resume한다. <br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 5에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 6에서 태스크 1이 종료된다.<br>
 *
 *             \image html ubik_test_tasktest03.gif
 *
 *         <br>
 *
 *         \section tasktest04_sec 우선순위 기반 멀티태스킹 기능 시험
 *             Test on priority-based multi-tasking
 *
 *             - <a href="tasktest04_8c-example.html">tasktest04.c</a>
 *
 *             이 예제는 우선순위 기반 멀티태스킹 기능을 시험한다.<br>
 *             시험 과정은 다음과 같다.<br>
 *
 *             시점 1에서 낮은 우선순위로 태스크 4를 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 4가 수행 중이어야 한다.<br>
 *
 *             시점 2에서 높은 우선순위로 태스크 1을 생성한다.<br>
 *             태스크 1의 우선순위가 태스크 4의 우선순위 보다 높다.<br>
 *             따라서 여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 3에서 중간 우선순위로 태스크 2와 3을 생성한다.<br>
 *             태스크 1의 우선순위가 태스크 2와 3의 우선순위 보다 높다.<br>
 *             따라서 여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 4에서 태스크 1이 종료된다.<br>
 *             그러면 태스크 2와 3이 가장 높은 우선순위를 가진 태스크가 된다.<br>
 *             따라서 여기까지 성공적으로 진행되었다면  확인 지점 4에서 태스크 2와 3이 수행 중이어야 한다.<br>
 *             태스크 2와 3은 같은 우선순위를 가지므로 라운드 로빈 방식으로 동시에 수행된다.<br>
 *
 *             시점 5에서 태스크 2와 3이 종료된다.<br>
 *
 *             시점 6에서 태스크 4가 종료된다.<br>
 *
 *             \image html ubik_test_tasktest04.gif
 *
 *         <br>
 *
 *         \section tasktest05_sec 태스크의 우선순위 설정 기능 시험
 *             Test on set priority function of task
 *
 *             - <a href="tasktest05_8c-example.html">tasktest05.c</a>
 *
 *             이 예제는 태스크의 우선순위 설정 기능을 시험한다.<br>
 *             시험 과정은 다음과 같다.<br>
 *
 *             시점 1에서 낮은 우선순위로 태스크 2를 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 2에서 중간 우선순위로 태스크 1을 생성한다.<br>
 *             태스크 1의 우선순위가 태스크 2의 우선순위보다 높다.<br>
 *             따라서 여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 3에서 태스크 2의 우선순위를 높은 우선순위로 설정한다.<br>
 *             그러면 태스크 2의 우선순위가 태스크 1의 우선순위보다 높아진다.<br>
 *             따라서 여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 4에서 태스크 2의 우선순위를 낮은 우선순위로 설정한다.<br>
 *             그러면 태스크 1의 우선순위가 태스크 2의 우선순위보다 높아진다.<br>
 *             따라서 여기까지 성공적으로 진행되었다면 확인 지점 4에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 5에서 태스크 1이 종료된다.<br>
 *
 *             시점 6에서 태스크 2가 종료된다.<br>
 *
 *             \image html ubik_test_tasktest05.gif
 *
 *         <br>
 *
 *         \section tasktest06_sec 태스크에 의한 메모리 누수 시험
 *             Test for memory leak by task
 *
 *             - <a href="tasktest06_8c-example.html">tasktest06.c</a>
 *
 *             이 예제는 태스크에 의한 메모리 누수를 시험한다.<br>
 *             시험 과정은 다음과 같다.<br>
 *
 *             먼저 힙heap의 할당된 메모리 총량과 메모리 블록 수를 가져와 저장한다.<br>
 *             그리고 태스크 생성과 제거를 여러 번 반복한다.<br>
 *             그 다음 힙heap의 할당된 메모리 총량과 메모리 블럭 수를 다시 가져와 저장했던 값과 비교한다.<br>
 *             메모리 누수가 발생하지 않았다면 저장했던 값과 다시 가져온 값이 같아야 한다.<br>
 *
 *         <br>
 *
 *         \section tasktest07_sec 태스크의 다수 신호 객체 기다리기 기능 시험
 *             Test on wait for multiple signal objects function of task
 *
 *             - <a href="tasktest07_8c-example.html">tasktest07.c</a>
 *
 *             이 예제는 태스크의 다수 신호 객체 기다리기 기능을 시험한다.<br>
 *             태스크의 다수 신호 객체 기다리기 기능은 세마포어, 뮤텍스, 메시지큐, 시그널, 조건변수condition variable,
 *             입출력버퍼 등 다양한 종류의 신호 객체 여러 개로부터 신호를 동시에 기다려야 할 경우 사용한다.<br>
 *             이 기능을 사용해 다수 신호 객체를 기다리는 태스크는 기다림을 끝낼 조건으로 다음 두가지 중 하나를 선택할 수 있다.<br>
 *             첫 번째는 기다리는 신호 객체들 중 하나 이상으로부터 신호를 받는 것이고,
 *             두 번째는 기다리는 신호 객체들 모두로부터 신호를 받는 것이다.<br>
 *             모두 기다리기 선택 사항(TASK_WAITOPT__ALL)을 선택하고 이 기능을 사용하면 두 번째가, 그렇지 않으면 첫 번째가 기다림을 끝낼 조건이 된다.<br>
 *             시험 과정은 다음과 같다.<br>
 *
 *             시점 3에서 태스크 1이 모두 기다리기 선택 사항(TASK_WAITOPT__ALL)을 선택하지 않고 다수 신호 객체 기다리기를 시도한다.<br>
 *             여기까지 성공적으로 진행되었다면  확인 지점 3에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 4에서 태스크 2가 태스크 1이 기다리고 있는 신호 객체들 중 하나에게 신호를 보낸다.<br>
 *             그러면  태스크 1이 그 신호를 받고 깨어난다.<br>
 *             이는 태스크 1이 모두 기다리기 선택 사항(TASK_WAITOPT__ALL)을 선택하지 않고 다수 신호 객체를 기다리고 있기 때문이다.<br>
 *             여기까지 성공적으로 진행되었다면  확인 지점 4에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 5에서 태스크 1이 모두 기다리기 선택 사항(TASK_WAITOPT__ALL)을 선택하고 다수 신호 객체 기다리기를 시도한다.<br>
 *             여기까지 성공적으로 진행되었다면  확인 지점 5에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 6에서 태스크 2가 태스크 1이 기다리고 있는 신호 객체들 중 하나에게 신호를 보낸다.<br>
 *             그러면 태스크 1이 그 신호를 받는다. 그러나 태스크 1이 깨어나지는 않는다.<br>
 *             이는 태스크 1이 모두 기다리기 선택 사항(TASK_WAITOPT__ALL)을 선택하고 다수 신호 객체를 기다리고 있기 때문이다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 6에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 7에서 태스크 2가 태스크 1이 기다리고 있는 나머지 모든 신호 객체들에게 신호를 보낸다.<br>
 *             그러면 태스크 1이 그 신호들을 받고 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 7에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 10에서 태스크 1이 모두 기다리기 선택 사항(TASK_WAITOPT__ALL)을 선택하고
 *             시점 12까지를 제한 시간으로 두고 다수 신호 객체 기다리기를 시도한다.<br>
 *             여기까지 성공적으로 진행되었다면  확인 지점 10에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 11에서 태스크 2가 태스크 1이 기다리고 있는 신호 객체들 중 하나에게 신호를 보낸다.<br>
 *             그러면 태스크 1이 그 신호를 받는다. 그러나 태스크 1이 깨어나지는 않는다.<br>
 *             이는 태스크 1이 모두 기다리기 선택 사항(TASK_WAITOPT__ALL)을 선택하고 다수 신호 객체를 기다리고 있기 때문이다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 11에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 12가 되면 태스크 1이 제한 시간이 다해서 기다리는 모든 신호 객체들로부터 신호를 받지 못했더라도 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 12에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             \image html ubik_test_tasktest07.gif
 *
 *         <br>
 *
 *         \section tasktest08_sec 태스크의 남은 제한 시간 가져오기 기능 시험
 *             Test on get remaining timeout tick function of task
 *
 *             - <a href="tasktest08_8c-example.html">tasktest08.c</a>
 *
 *             이 예제는 남은 제한 시간을 가져오는 기능을 시험한다.<br>
 *             제한 시간을 두고 신호 객체를 기다리는 태스크가 제한 시간이 다하기 전에 신호를 받고 깨어났을 때,
 *             이 기능을 사용하면 남은 제한 시간을 알아낼 수 있다.<br>
 *             이 기능은 비 동기적으로 들어오는 신호 여러 개를 순차적으로 받아 처리하는 작업에 제한 시간을 두어야 할 경우 유용하다.<br>
 *             시험 과정은 다음과 같다.<br>
 *
 *             시점 1에서 중간 우선순위로 태스크 2를 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 2에서 높은 우선순위로 태스크 1을 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 3에서 태스크 1이 시점 6까지를 제한 시간으로 두고 첫 번째 신호를 기다린다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 4에서 태스크 2가  첫 번째 신호를 보낸다.<br>
 *             그러면 태스크 1이 그 신호를 받고 깨어난다.<br>
 *             그리고 깨어난 태스크 1은 남은 제한 시간을 가져온다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 4에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 5에서 태스크 1이 가져온 남은 제한 시간과 이미 알고 있는 시점 4와 5 사이의 작업 시간을 가지고 시점 6까지 남은 시간을 계산한다.<br>
 *             그리고 계산한 남은 시간을 활용해 다시 시점 6까지를 제한 시간으로 두고 두 번째 신호를 기다린다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 5에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 6이되면 태스크 1이 제한 시간이 다해서 신호를 받지 못했더라도  깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 6에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 7에서 태스크 1이 종료된다.<br>
 *
 *             시점 8에서 태스크 2가 종료된다.<br>
 *
 *             \image html ubik_test_tasktest08.gif
 *
 *         <br>
 *
 *         \section tasktest09_sec 태스크 중지suspend 및 재시작resume 기능 시험 2
 *             Test 2 on suspend and resume function of task
 *
 *             - <a href="tasktest09_8c-example.html">tasktest09.c</a>
 *
 *             이 예제는 태스크를 중지suspend했다가 재시작resume했을 때, 태스크 상태가 중지suspend하기 직전 상태로 돌아가는지를 시험한다.<br>
 *             시험 과정은 다음과 같다.<br>
 *
 *             시점 1에서 중간 우선순위로 태스크 2를 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 1에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 2에서 높은 우선순위로 태스크 1을 생성한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 2에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 3에서 태스크 1이 신호를 기다린다(대기blocked 상태가 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 3에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 4에서 태스크 2가 태스크 1을 중지suspend한 후 신호를 보낸다.<br>
 *             중지suspend된 태스크는 신호를 받지 못한다.<br>
 *             따라서 여기까지 성공적으로 진행되었다면 확인 지점 4에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 5에서 태스크 2가 태스크 1을 재시작resume한다.<br>
 *             재시작resume되 태스크 1은 시점 4에서 보내진 신호를 받고 깨어난다(준비ready 상태가 된다).<br>
 *             따라서 여기까지 성공적으로 진행되었다면 확인 지점 5에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 6에서 태스크 1이 시점 8까지를 제한 시간으로 두고 신호를 기다린다(대기blocked 상태가 된다).<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 6에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 7에서 태스크 2가 태스크 1을 중지suspend한다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 7에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 8에서 태스크 2가 태스크 1을 재시작resume한다.<br>
 *             재시작resume되 태스크 1은  중지suspend되기 직전 상태인 대기blocked 상태가 된다.<br>
 *             그리고 태스크 1이 이 시점까지를 제한 시간으로 두고 신호를 기다렸지만
 *             중지suspend 상태에 머무른 시간은 지나간 시간으로 인정하지 않기 때문에 깨어나지 않는다.<br>
 *             제한 시간이 다하는 시점은 중지suspend 상태에 머무른 시간을 더한 시점 9가 된다.<br>
 *             따라서 여기까지 성공적으로 진행되었다면 확인 지점 8에서 태스크 2가 수행 중이어야 한다.<br>
 *
 *             시점 9가 되면 태스크 1이 제한 시간이 다해서 신호를 받지 못했더라도 깨어난다.<br>
 *             여기까지 성공적으로 진행되었다면 확인 지점 9에서 태스크 1이 수행 중이어야 한다.<br>
 *
 *             시점 10에서 태스크 1이 종료된다.<br>
 *
 *             시점 11에서 태스크 2가 종료된다.<br>
 *
 *             \image html ubik_test_tasktest09.gif
 *
 * <br>
 *
 * Content is available under Attribution-Noncommercial-No Derivative Works 3.0 Unported.
 */

#include "type.h"

/** 기다리는 방법 선택 사항: 기다리는 신호 객체들 중 NULL이거나 유효성 비트valid bit가 0인 것이 있더라도
 *                        그것들을 제외한 나머지 신호 객체들을 기다림 (유효하지 않은 신호 객체 무시하고 기다리기 선택 사항)*/
#define	TASK_WAITOPT__IGNOREINVALID		0x80

/** 기다리는 방법 선택 사항: 기다리는 신호 객체들 모두로부터 신호를 받을 때까지 기다림 (모두 기다리기 선택 사항) */
#define	TASK_WAITOPT__ALL				0x40

/** 태스크 함수 포인터  형 정의 */
typedef void (* taskfunc_ft)(void *);

typedef	struct __task_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _task_tip_t;

/** 태스크 포인터 형 정의 */
typedef	_task_tip_t * task_pt;

/**
 * 태스크를 생성하는 함수
 *
 * @param	task_p		생성한 태스크의 주소를 저장할 포인터의 주소<br>
 * 						NULL: 생성한 태스크의 주소를 저장하지 않음<br>
 * 						<br>
 *
 * @param	func		태스크가 실행할 함수 포인터<br>
 * 						<br>
 *
 * @param	arg			태스크가 실행할 함수로 전달할 매개변수<br>
 * 						<br>
 *
 * @param	priority	우선순위<br>
 * 						<br>
 * 						큰 숫자가 높은 우선순위<br>
 * 						최고 우선순위는 task_gethighestpriority 함수가 돌려 주는 값<br>
 * 						중간 우선순위는 task_getmiddlepriority 함수가 돌려주는 값<br>
 * 						최저 우선순위는 task_getlowestpriority 함수가 돌려주는 값<br>
 * 						<br>
 *
 * @param	stackdepth	스택 깊이(스택 영역 크기는 stackdepth * INT_SIZE 바이트가 됨)<br>
 * 						0: 기본  스택 깊이 값(task_getdefaultstackdepth 함수가 돌려주는 값)<br>
 * 						1 ~ 최저 스택 깊이 값: 최저 스택 깊이 값(task_getminstackdepth 함수가 돌려주는 값)<br>
 * 						<br>
 *
 * @param	name		태스크 이름<br>
 * 						NULL: 태스크 이름을 지정하지 않음<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_create(task_pt * task_p, taskfunc_ft func, void * arg, int priority, unsigned int stackdepth, const char * name);

/**
 * 태스크를 생성하는 함수 확장형
 *
 * @param	task_p		생성한 태스크의 주소를 저장할 포인터의 주소<br>
 * 						NULL: 생성한 태스크의 주소를 저장하지 않음<br>
 * 						<br>
 *
 * @param	func		태스크가 실행할 함수 포인터<br>
 * 						<br>
 *
 * @param	arg			태스크가 실행할 함수로 전달할 매개변수<br>
 * 						<br>
 *
 * @param	priority	우선순위<br>
 * 						<br>
 * 						큰 숫자가 높은 우선순위<br>
 * 						최고 우선순위는 task_gethighestpriority 함수가 돌려 주는 값<br>
 * 						중간 우선순위는 task_getmiddlepriority 함수가 돌려주는 값<br>
 * 						최저 우선순위는 task_getlowestpriority 함수가 돌려주는 값<br>
 * 						<br>
 *
 * @param	stackdepth	스택 깊이(스택 영역 크기는 stackdepth * INT_SIZE 바이트가 됨)<br>
 * 						0: 기본  스택 깊이 값(task_getdefaultstackdepth 함수가 돌려주는 값)<br>
 * 						1 ~ 최저 스택 깊이 값: 최저 스택 깊이 값(task_getminstackdepth 함수가 돌려주는 값)<br>
 * 						<br>
 *
 * @param	name		태스크 이름<br>
 * 						NULL: 태스크 이름을 지정하지 않음<br>
 * 						<br>
 *
 * @param	tag			커널 객체를 구별하기 위해 사용하는 꼬리표<br>
 * 						0: 0 ~ UBIK_TAG__USRSTART-1 사이의 값으로 자동할당됨<br>
 * 						1 ~ UBIK_TAG__USRSTART-1: 오류<br>
 * 						<br>
 * 						사용자가 직접 지정할 경우 UBIK_TAG__USRSTART ~ UINT_MAX 사이의 값을 사용해야 함<br>
 * 						<br>
 *
 * @param	option		선택 사항 (TASK_OPT__...)<br>
 * 						0: 어느 것도 선택하지 않음<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_create_ext(task_pt * task_p, taskfunc_ft func, void * arg, int priority, unsigned int stackdepth, const char * name, unsigned int tag, unsigned int option);

/**
 * 대상 태스크를 제거하는 함수
 *
 * @param	task_p		제거할 대상 태스크의 주소가 저장된 포인터의 주소<br>
 * 						task_p 또는 *task_p가 NULL: 현재 태스크를 제거함<br>
 * 						<br>
 * 						제거에 성공하면 *task_p는  NULL이 됨<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_delete(task_pt * task_p);

/**
 * 대상 태스크를 중지suspend하는 함수
 *
 * @param	task		대상 태스크 포인터<br>
 * 						NULL: 현재 태스크<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_suspend(task_pt task);

/**
 * 중지suspend된 대상 태스크를 재시작resume하는 함수
 *
 * @param	task		대상 태스크 포인터<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_resume(task_pt task);

/**
 * 현재 태스크를 휴면sleep시키는 함수
 *
 * @param	tick		휴면sleep시킬 시간(시스템 틱tick 수)<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int task_sleep(unsigned int tick);

/**
 * 현재 태스크를 휴면sleep시키는 함수 (천분의 일초 단위)
 *
 * @param	timems		휴면sleep시킬 시간(천분의 일초)<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int task_sleepms(unsigned int timems);

/**
 * 대상 태스크의 우선순위를 변경하는 함수
 *
 * @param	task		대상 태스크 포인터<br>
 * 						NULL: 현재 태스크<br>
 * 						<br>
 *
 * @param	priority	우선순위<br>
 * 						<br>
 * 						큰 숫자가 높은 우선순위<br>
 * 						최고 우선순위는 task_gethighestpriority 함수가 돌려 주는 값<br>
 * 						중간 우선순위는 task_getmiddlepriority 함수가 돌려주는 값<br>
 * 						최저 우선순위는 task_getlowestpriority 함수가 돌려주는 값<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_setpriority(task_pt task, int priority);

/**
 * 대상 태스크의 우선순위를 돌려주는 함수
 *
 * 대상 태스크가 NULL이고 커널이 활성화 상태가 아니면(ubik_comp_start 함수를 호출하기 전이면)
 * 중간 우선 순위(task_getmiddlepriority 함수가 돌려 주는 값)를 돌려줌<br>
 *
 * @param	task		대상 태스크 포인터<br>
 * 						NULL: 현재 태스크<br>
 * 						<br>
 *
 * @return	대상 태스크의 우선순위<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_getpriority(task_pt task);

/**
 * 태스크가 가질 수 있는 최고 우선순위를 돌려주는 함수
 *
 * @return	태스크가 가질 수 있는 최고 우선순위
 */
int task_gethighestpriority(void);

/**
 * 태스크가 가질 수 있는 최저 우선순위를 돌려주는 함수
 *
 * @return	태스크가 가질 수 있는 최저 우선순위
 */
int task_getlowestpriority(void);

/**
 * 태스크가 가질 수 있는 중간 우선순위를 돌려주는 함수
 *
 * @return	태스크가 가질 수 있는 중간 우선순위
 */
int task_getmiddlepriority(void);

/**
 * 태스크 전환을 금지하는 함수
 */
void task_lock(void);

/**
 * 태스크 전환을 허가하는 함수
 */
void task_unlock(void);

/**
 * 대상 태스크가 동시에 기다릴 수 있는 신호 객체 최대 수를 설정하는 함수
 *
 * @param	task		대상 태스크 포인터<br>
 * 						NULL: 현재 태스크<br>
 * 						<br>
 *
 * @param	max			동시에 기다릴 수 있는 신호 객체 최대 수 (TASK_MAXWAITSIGOBJ_MAX 보다 작거나 같아야 함)<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_setmaxwaitsigobj(task_pt task, int max);

/**
 * 대상 태스크가 동시에 기다릴 수 있는 신호 객체 최대 수를 돌려주는 함수
 *
 * @param	task		대상 태스크 포인터<br>
 * 						NULL: 현재 태스크<br>
 * 						<br>
 *
 * @return	동시에 기다릴 수 있는 신호 객체 최대 수<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_getmaxwaitsigobj(task_pt task);

/**
 * 신호 객체 여러 개를 동시에 기다리는 함수
 *
 * 동시에 기다릴 수 있는 신호 객체 수 기본값은 1이며, task_setmaxwaitsigobj 함수로 변경할 수 있음<br>
 * 동시에 기다리는 신호 객체 수가 많으면 함수 내부의 크리티컬 상태(인터럽트 비활성화 상태) 유지 시간이 길어지기 때문에,
 * 성능이 낮은 시스템에서 문제가 발생할 수 있음
 *
 * @param	sigobj_p	대상 개체 포인터 배열<br>
 * 						<br>
 *
 * @param	sigtype_p	받은 신호를 저장할 배열<br>
 * 						<br>
 *
 * @param	param_p		신호 객체와 같이 전달할 매개변수<br>
 * 						NULL: 매개변수를 전달하지 않음<br>
 * 						<br>
 * 						매개변수<br>
 * 						신호 객체가 메시지큐일 경우 받은 메시지를 저장할 버퍼<br>
 * 						신호 객체가 조건변수condition variable일 경우 조건변수condition variable를 보호하는 뮤텍스<br>
 * 						<br>
 *
 * @param	count		배열 길이<br>
 * 						<br>
 *
 * @param	waitopt		기다리는 방법 선택 사항 (TASK_WAITOPT__...)<br>
 * 						0: 어느 것도 선택하지 않음<br>
 * 						<br>
 * 						기다리는 방법을 선택하지 않으면 기다리는 신호 객체들 중 하나 이상으로부터 신호를 받을 때까지 기다림<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 */
int task_waitforsigobjs(void ** sigobj_p, int * sigtype_p, void ** param_p, int count, unsigned int waitopt);

/**
 * 제한 시간 동안 신호 객체 여러 개를 동시에 기다리는 함수
 *
 * 동시에 기다릴 수 있는 신호 객체 수 기본값은 1이며, task_setmaxwaitsigobj 함수로 변경할 수 있음<br>
 * 동시에 기다리는 신호 객체 수가 많으면 함수 내부의 크리티컬 상태(인터럽트 비활성화 상태) 유지 시간이 길어지기 때문에,
 * 성능이 낮은 시스템에서 문제가 발생할 수 있음
 *
 * @param	sigobj_p	대상 개체 포인터 배열<br>
 * 						<br>
 *
 * @param	sigtype_p	받은 신호를 저장할 배열<br>
 * 						<br>
 *
 * @param	param_p		신호 객체와 같이 전달할 매개변수<br>
 * 						NULL: 매개변수를 전달하지 않음<br>
 * 						<br>
 * 						매개변수<br>
 * 						신호 객체가 메시지큐일 경우 받은 메시지를 저장할 버퍼<br>
 * 						신호 객체가 조건변수condition variable일 경우 조건변수condition variable를 보호하는 뮤텍스<br>
 * 						<br>
 *
 * @param	count		배열 길이<br>
 * 						<br>
 *
 * @param	waitopt		기다리는 방법 선택 사항 (TASK_WAITOPT__...)<br>
 * 						0: 어느 것도 선택하지 않음<br>
 * 						<br>
 * 						기다리는 방법을 선택하지 않으면 기다리는 신호 객체들 중 하나 이상으로부터 신호를 받을 때까지 기다림<br>
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
 */
int task_waitforsigobjs_timed(void ** sigobj_p, int * sigtype_p, void ** param_p, int count, unsigned int waitopt, unsigned int tick);

/**
 * 제한 시간 동안 신호 객체 여러 개를 동시에 기다리는 함수 (천분의 일초 단위)
 *
 * 동시에 기다릴 수 있는 신호 객체 수 기본값은 1이며, task_setmaxwaitsigobj 함수로 변경할 수 있음<br>
 * 동시에 기다리는 신호 객체 수가 많으면 함수 내부의 크리티컬 상태(인터럽트 비활성화 상태) 유지 시간이 길어지기 때문에,
 * 성능이 낮은 시스템에서 문제가 발생할 수 있음
 *
 * @param	sigobj_p	대상 개체 포인터 배열<br>
 * 						<br>
 *
 * @param	sigtype_p	받은 신호를 저장할 배열<br>
 * 						<br>
 *
 * @param	param_p		신호 객체와 같이 전달할 매개변수<br>
 * 						NULL: 매개변수를 전달하지 않음<br>
 * 						<br>
 * 						매개변수<br>
 * 						신호 객체가 메시지큐일 경우 받은 메시지를 저장할 버퍼<br>
 * 						신호 객체가 조건변수condition variable일 경우 조건변수condition variable를 보호하는 뮤텍스<br>
 * 						<br>
 *
 * @param	count		배열 길이<br>
 * 						<br>
 *
 * @param	waitopt		기다리는 방법 선택 사항 (TASK_WAITOPT__...)<br>
 * 						0: 어느 것도 선택하지 않음<br>
 * 						<br>
 * 						기다리는 방법을 선택하지 않으면 기다리는 신호 객체들 중 하나 이상으로부터 신호를 받을 때까지 기다림<br>
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
 */
int task_waitforsigobjs_timedms(void ** sigobj_p, int * sigtype_p, void ** param_p, int count, unsigned int waitopt, unsigned int timems);

/**
 * 대상 태스크들이 종료되기를 기다리는 함수
 *
 * @param	task_p		대상 태스크 포인터 배열<br>
 * 						<br>
 *
 * @param	result_p	결과값을 저장할  배열<br>
 * 						NULL: 결과값을 저장하지 않음<br>
 * 						<br>
 * 						결과값<br>
 * 						0: 정상 종료<br>
 * 						UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 						<br>
 *
 * @param	count		배열 길이  (TASK_MAXWAITSIGOBJ_MAX 보다 작거나 같아야 함)<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 */
int task_join(task_pt * task_p, int * result_p, int count);

/**
 * 제한 시간 동안 대상 태스크들이 종료되기를 기다리는 함수
 *
 * @param	task_p		대상 태스크 포인터 배열<br>
 * 						<br>
 *
 * @param	result_p	결과값을 저장할  배열<br>
 * 						NULL: 결과값을 저장하지 않음<br>
 * 						<br>
 * 						결과값<br>
 * 						0: 정상 종료<br>
 *			 			UBIK_ERR__TIMEOUT		: 제한 시간 초과<br>
 * 						UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 						<br>
 *
 * @param	count		배열 길이  (TASK_MAXWAITSIGOBJ_MAX 보다 작거나 같아야 함)<br>
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
 */
int task_join_timed(task_pt * task_p, int * result_p, int count, unsigned int tick);

/**
 * 제한 시간 동안 대상 태스크들이 종료되기를 기다리는 함수 (천분의 일초 단위)
 *
 * @param	task_p		대상 태스크 포인터 배열<br>
 * 						<br>
 *
 * @param	result_p	결과값을 저장할  배열<br>
 * 						NULL: 결과값을 저장하지 않음<br>
 * 						<br>
 * 						결과값<br>
 * 						0: 정상 종료<br>
 *			 			UBIK_ERR__TIMEOUT		: 제한 시간 초과<br>
 * 						UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 						<br>
 *
 * @param	count		배열 길이  (TASK_MAXWAITSIGOBJ_MAX 보다 작거나 같아야 함)<br>
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
 */
int task_join_timedms(task_pt * task_p, int * result_p, int count, unsigned int timems);

/**
 * 현재 태스크 포인터를 돌려주는 함수
 *
 * @return	현재 태스크 포인터<br>
 * 			<br>
 * 			NULL: ubik이 활성화되지 않았음
 */
task_pt task_getcur(void);

/**
 * 현재 태스크의 남은 제한 시간을 돌려주는 함수
 *
 * 제한 시간을 두고 신호 객체를 기다리는 태스크가 제한 시간이 다하기 전에 신호를 받고 깨어났을 경우,
 * 이 함수를 사용해 남은 제한 시간을 알아낼 수 있음
 *
 * @return	남은 제한 시간 (시스템 틱tick 수)
 */
unsigned int task_getremainingtimeout(void);

/**
 * 현재 태스크의 남은 제한 시간을 돌려주는 함수
 *
 * 제한 시간을 두고 신호 객체를 기다리는 태스크가 제한 시간이 다하기 전에 신호를 받고 깨어났을 경우,
 * 이 함수를 사용해 남은 제한 시간을 알아낼 수 있음
 *
 * @return	남은 제한 시간 (천분의 일초 단위)
 */
unsigned int task_getremainingtimeoutms(void);

/**
 * 스택 깊이 최저값을 돌려주는 함수
 *
 * @return	스택 깊이 최저값
 */
unsigned int task_getminstackdepth(void);

/**
 * 스택 깊이 기본값을 돌려주는 함수
 *
 * @return	스택 깊이 기본값
 */
unsigned int task_getdefaultstackdepth(void);

/**
 * 해당 태스크의 스택 영역 크기를 돌려주는 함수
 *
 * @param	task			대상 태스크 포인터<br>
 * 							NULL: 현재 태스크<br>
 * 							<br>
 *
 * @param	stacksize_p		스택 영역 크기를 저장할 변수의 주소<br>
 *							<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_getstacksize(task_pt task, unsigned int * stacksize_p);

/**
 * 해당 태스크의 스택 영역 크기를 돌려주는 함수 (스택 오버플로우 검사 생략)
 *
 * @param	task			대상 태스크 포인터<br>
 * 							NULL: 현재 태스크<br>
 * 							<br>
 *
 * @param	stacksize_p		스택 영역 크기를 저장할 변수의 주소<br>
 *							<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_getstacksize_nosocheck(task_pt task, unsigned int * stacksize_p);

/**
 * 해당 태스크의 최대 스택 사용량을 돌려주는 함수
 *
 * @param	task			대상 태스크 포인터<br>
 * 							NULL: 현재 태스크<br>
 * 							<br>
 *
 * @param	maxstackusage_p	 최대 스택 사용량을 저장할 변수의 주소<br>
 *							<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_getmaxstackusage(task_pt task, unsigned int * maxstackusage_p);

/**
 * 해당 태스크의 최대 스택 사용량을 돌려주는 함수 (스택 오버플로우 검사 생략)
 *
 * @param	task			대상 태스크 포인터<br>
 * 							NULL: 현재 태스크<br>
 * 							<br>
 *
 * @param	maxstackusage_p	 최대 스택 사용량을 저장할 변수의 주소<br>
 *							<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int task_getmaxstackusage_nosocheck(task_pt task, unsigned int * maxstackusage_p);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_TASK_H_ */
