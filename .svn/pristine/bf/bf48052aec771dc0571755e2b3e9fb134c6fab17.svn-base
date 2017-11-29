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

#ifndef ITF_UBINOS_H_
#define ITF_UBINOS_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubinos.h
 *
 * @brief ubinos 인터페이스
 *
 * ubinos 인터페이스를 정의합니다.
 */

/**
 * \mainpage 유비노스 인터페이스
 *
 *     문서 버전 01.00.01<br>
 *     <br>
 *     유비노스 인터페이스 버전 01.00.02<br>
 *
 * <br>
 *
 * \section intro_sec 유비노스 소개
 *
 *     유비노스는 내장형 컴퓨터를 위한 운영체제이다. 유비노스는 우선순위 기반 선점형 멀티태스킹과 세마포어, 뮤텍스, 메시지큐, 시그널, 조건변수condition variable 등의 풍부한 태스크간 통신 기능을 지원하면서도, 수K 램RAM과 수십K 롬ROM만을 가진 컴퓨터에서도 잘 동작한다. 또한 수행할 작업이 없는 기간 동안 스스로 전력을 차단함으로써 전력 소비량을 줄일 수 있다. (이 기능은 현재 개발 중이다.) 때문에 유비노스는 램RAM, 롬ROM, 전력 자원 제약이 매우 심한 초소형 초 저전력 내장형 시스템 개발에 유리하다. 또한 유비노스는 우선순위 기반 라운드 로빈 선점형 멀티태스킹을 지원하며 주요 기능들이 최악 수행 시간을 예측할 수 있도록 만들어져 있기 때문에 실시간 시스템 개발에도 사용될 수 있다.
 *
 * <br>
 *
 * \section task_sec 멀티태스킹 multi-tasking
 *
 *     유비노스는 태스크 여러 개를 동시에 수행할 수 있다. 유비노스의 태스크는 스레드 개념을 구현한 것이며, 동적으로 생성되고 제거될 수 있다. 스레드는 각각이 하나의 독립적인 수행 단위이며, 고유한 스택을 가지지만 그 외의 메모리 공간은 다른 스레드와 공유한다. 유비노스의 태스크는 우선순위를 가지며, 높은 우선순위를 가진 태스크가 낮은 우선순위를 가진 태스크 보다 먼저 수행된다. 같은 우선순위를 가진 태스크들은 동시에 수행된다. 실제로는 라운드 로빈 round robin 방식으로 순서를 정해 일정한 주기로 번갈아 수행되는 것이지만, 그 주기가 매우 짧기 때문에 동시에 수행되는 것과 유사하게 동작한다.
 *
 *     <b>태스크 상태전이표 task state transition diagram</b>
 *
 *     다음 그림은 태스크의 상태 변화를 나타내는 상태전이표 state transition diagram 이다.
 *
 *         \image html ubik_task_state_transition_diagram.png
 *
 *     태스크는 다음과 같은 상태가 될 수 있다.
 *
 * <pre>
 *     준비ready 상태
 *         수행될 준비가 되었지만 수행될 순서가 되지 않은 상태
 *
 *     수행running 상태
 *         수행될 순서가 되어 수행되고 있는 상태
 *
 *     대기blocked 상태
 *         지정한 조건이 만족되기를 기다리는 상태
 *         조건은 지정한 시간이 지나감, 지정한 자원이 사용 가능해짐 등이 될 수 있음
 *
 *     중지suspended 상태
 *         조건 없이 기다리는 상태
 *         다른 태스크가 재시작resume 시켜 주어야 이 상태에서 빠져 나올 수 있음
 * </pre>
 *
 *     태스크의 최초 상태는 준비ready 상태이다. 준비ready 상태인 태스크는 수행될 순서가 되었을 때 수행running 상태가 된다. 수행running 상태인 태스크는 한 수행 주기 time slice 만큼의 시간 동안 수행되었을 때 또는 자신의 우선순위보다 높은 우선순위를 가진 태스크가 준비ready 상태가 되었을 때 수행을 중지하고 준비ready 상태가 된다. 또는 스스로 어떤 조건이 만족될 때까지 기다리기를 원할 때 대기blocked 상태가 된다. 기다리는 조건은 지정한 시간이 지나는 것(task_sleep 함수를 호출했을 경우 이것이 조건이 된다), 지정한 자원이 사용 가능해지는 것(sem_take, mutex_lock 함수 등을 호출했을 경우 이것이 조건이 된다) 등이 될 수 있다. 대기blocked 상태인 태스크는 지정한 조건이 만족되었을 때 준비ready 상태가 된다.
 *
 *     준비ready 상태, 수행running 상태, 대기blocked 상태인 태스크는 스스로 또는 다른 태스크가 중지suspend하기를 원할 때(task_suspend 함수를 호출 할 때) 중지suspended 상태가 된다. 중지suspended 상태인 태스크는 다른 태스크가 재시작resume 시켜 줄 때(task_resume 함수를 호출 할 때)만 중지suspended 상태에서 벗어날 수 있다. 대기blocked 상태에서 중지suspended 상태가 된 태스크는 지정한 조건이 만족되더라도 준비ready 상태가 되지 않는다. 그리고 만족되기를 기다리는 조건이 지정한 시간이 지나는 것일 경우, 중지suspended 상태에 머무른 시간은 지나간 시간으로 인정되지 않는다. 중지suspended 상태인 태스크는 재시작resume되면 중지suspended 상태가 되기 직전 상태로 돌아간다. 좀 더 정확히 말하자면, 직전 상태가 준비ready 상태와 수행 running 상태였던 태스크는 준비ready 상태가 된다. 그리고 직전 상태가 대기blocked 상태였던 태스크는 재시작resume된 시점에 지정한 조건이 만족될 경우 준비ready 상태가, 그렇지 않을 경우 대기blocked 상태가 된다.
 *
 *     <b>문맥 교환 context switching</b>
 *
 *     수행 태스크를 전환하려면, 먼저 현재 태스크가 수행하던 작업을 나중에 이어서 수행할 수 있도록 현재 작업 내용을 저장한 후, 다음 태스크가 이전에 수행하는 작업을 이어서 수행할 수 있도록 저장되어 있는 다음 태스크의 작업 내용을 복원해야 한다. 저장하고 복원해야 하는 작업 내용은 CPU 레지스터 및 몇몇 전역 변수의 값이다. 이 값들은 문맥context이라 불리며, 이 문맥context을 저장하고 복원하는 작업은 문맥 교환 context switching 이라 불린다.
 *
 *     다음 그림은 ARM 플랫폼 용 유비노스 커널의 문맥 교환 context switching 과정을 보여준다.
 *
 *         \image html ubik_task_context_switching_diagram.png
 *
 *     ARM 플랫폼용 유비노스의 문맥에는 ARM CPU의 레지스터인 R0~R12, SP, LR, PC, SPSR의 값, 그리고 전역변수인 _ubik_critcount의 값이 포함된다. R0~R12는 범용 레지스터이고, SP는 스택 포인터, LR은 프로시저 복귀 주소, PC는 다음에 수행할 CPU 인스트럭션instruction 주소를 보관하는 레지스터이다. _ubik_critcount에는 현재 태스크의 크리티컬 섹션 critical section 재진입 횟수가 보관된다. 유비노스 커널은 문맥context을 스택에 저장한다. 스택에 저장된 문맥context의 시작 주소는 태스크 컨트롤 블록의 한 필드인 stacktop에 보관되며, 문맥context을 복원할 때 유비노스 커널은 이 필드의 값을 사용해 저장된 문맥에 접근한다. 전역변수인 _task_cur 에는 현재 태스크의 태스크 컨트롤 블록 주소가 보관된다. 문맥 교환 context switching 과정을 좀 더 자세히 설명하면 다음과 같다.
 *
 * <pre>
 *     1. R0~R12, SP, LR, PC, SPSR의 값을 현재 태스크의 스택에 저장한다.
 *
 *     2. _ubik_critcount의 값을 현재 태스크의 스택에 저장한다.
 *
 *     3. 스택에 저장된 문맥context의 시작 주소를 현재 태스크 컨트롤 블록의 stacktop에 저장한다.
 *
 *     4. _task_cur의 값을 다음 태스크의 태스크 컨트롤 블록 주소로 변경한다.
 *
 *     5. _ubik_critcount의 값을 다음 태스크의 스택에 저장되어 있는 값으로 변경한다.
 *
 *     6. R0~R12, SP, LR, PC, SPSR의 값을 다음 태스크의 스택에 저장되어 있는 값으로 변경한다.
 * </pre>
 *
 *     이 문맥 교환 context switching 작업은 인터럽트의 핸들러 내에서 수행된다. 때문에 이 문맥 교환 context switching 작업이 수행되는 시점에, 현재 태스크의 CPU 상태 정보는 인터럽트 발생 직전의 CPU 상태 정보가 저장되는 SPSR에 저장되어 있다. 또한 저장되어 있는 다음 태스크의 CPU 상태 정보를 SPSR에 설정한 후 인터럽트를 종료함으로써 다음 태스크의 CPU 상태 정보를 복원할 수 있다.
 *
 *     <b>선점형 스케줄링 preemptive scheduling</b>
 *
 *     현재 태스크보다 높은 우선순위를 가진 태스크가 수행될 준비가 되었거나, 한 수행 주기 time slice 가 끝나서 같은 우선순위를 가진 다음 순서의 태스크를 수행할 시점이 되었을 때, 수행 태스크 전환은 유비노스 커널에 의해 자동적이고 강제적으로 이루어진다. 이러한 태스크 전환 방식은 선점형 방식이라 불린다.
 *
 *     다음 그림은 현재 태스크보다 높은 우선순위를 가진 태스크가 수행될 준비가 되었을 때 수행 태스크가 전환되는 과정을 보여준다.
 *
 *         \image html ubik_task_preemption_diagram_01.png
 *
 *     더 높은 우선순위를 가진 태스크 1이 수행될 준비가 되는 시점 1과 2에 수행 태스크는 곧바로 태스크 1로 전환된다. 시점 3과 4에서의 수행 태스크 전환은 태스크 1이 스스로 필요한 이벤트를 기다리기 위해 수행 권한을 양보하기 때문에 발생한다.
 *
 *     다음 그림은 한 수행 주기 time slice 가 끝나서 같은 우선순위를 가진 다음 순서의 태스크를 수행할 시점이 되었을 때 수행 태스크가 전환되는 과정을 보여준다.
 *
 *         \image html ubik_task_preemption_diagram_02.png
 *
 *     시스템 틱 하드웨어 타이머에 의해 한 수행 주기 time slice 가 끝났음을 알리는 인터럽트가 주기적으로 시점 1, 2, 3, 4에서 발생하며, 이 인터럽트를 처리하기 위한 핸들러에 의해 수행 태스크는 같은 우선순위를 가진 다음 순서의 태스크로 전환된다.
 *
 *     - <a href="ubik__task_8h.html">태스크 인터페이스</a>
 *
 *     - \subpage task_example
 *
 * <br>
 *
 * \section sem_sec 세마포어
 *
 *     세마포어를 사용하면 다양한 태스크 간 동기화 문제를 해결할 수 있다. 태스크간 동기화란 태스크들이 서로 일 수행 시점을 맞추는 것이다. 예를 들어, 다른 태스크가 어떤 일을 완료해야 자신의 일을 시작할 수 있는 태스크가 있을 수 있다. 이런 태스크는 다른 태스크가 그 어떤 일 수행을 완료할 때까지 기다리다가 완료하는 순간 자신의 일 수행을 시작해야 한다. 다시 말해 자신의 일 수행 시작 시점을 다른 태스크의 일 수행 완료 시점에 맞추어야 한다. 세마포어는 그런 태스크를 구현할 때 사용된다.
 *
 *     세마포어는 상호 배제적mutual exclusive 사용 구현에도 사용될 수 있다. 그러나 상호 배제적mutual exclusive 사용 구현에는 뮤텍스를 사용하는 것이 더 낫다. 상호 배제적mutual exclusive 사용과, 그것의 구현에 뮤텍스가 왜 더 적합한지는 뮤텍스 부분에서 자세히 설명한다.
 *
 *     세마포어는 엣져 위베 다익스트라Edsger Wybe Dijkstra가 제안한 특수 명령으로만 접근할 수 있는 보호된 변수이다. 형태는 부호 없는 정수 또는 한자리 이진수가 될 수 있다. 형태가 부호 없는 정수인 세마포어는 카운팅counting 세마포어로, 한자리 이진수인 세마포어는 이진binary 세마포어로 불린다. 세마포어는 원자적atomic 명령인 P 명령(sem_take 함수)과 V 명령(sem_give 함수)을 통한 접근만을 허용한다. 원자적atomic이란 수행 도중 중단될 수 없음을 의미한다. 이 명령들을 의사코드pseudocode로 표현하면 다음과 같다.
 *
 * <pre>
 *     P (semaphore S) {
 *         wait until S > 0, then S = S - 1;
 *     }
 *
 *     V (semaphore S) {
 *        S = S + 1;
 *     }
 * </pre>
 *
 *     위의 의사코드pseudocode는 운영체제의 도움 없이 구현할 수 있다. 그러나 바쁜 기다림busy waiting을 한다. 운영체제가 제공하는 기능을 활용하는 방식으로 다시 표현하면 아래와 같다. 아래의 의사코드pseudocode는 바쁜 기다림busy waiting을 하지 않는다.
 *
 * <pre>
 *     P (semaphore S) {
 *         if S > 0, then
 *             S = S - 1;
 *         else
 *             insert current task into waiting task queue and sleep current task;
 *     }
 *
 *     V (semaphore S) {
 *         if waiting task queue is not empty, then
 *             get a task from waiting task queue and wake the task up ;
 *         else
 *             S = S + 1;
 *     }
 * </pre>
 *
 *     - <a href="ubik__sem_8h.html">세마포어 인터페이스</a>
 *
 *     - \subpage sem_example
 *
 * <br>
 *
 * \section mutex_sec 뮤텍스
 *
 *     자원 중에는 태스크 여러 개가 동시에 사용할 경우 문제가 생기는 것이 있다. 그런 자원은 상호 배제적으로mutual exclusively 사용해야 한다. 상호 배제적mutual exclusive 사용이란 한 태스크가 어떤 자원을 다른 태스크들이 사용하는 것을 금지하고 사용하는 것을 의미한다. 뮤텍스를 사용하면 이 상호 배제적mutual exclusive 사용을 쉽게 구현할 수 있다. 뮤텍스를 하나 생성한 다음, 자원 사용을 시작하기 전에 그 뮤텍스에 잠그기 명령(mutex_lock 함수)을, 자원 사용을 끝마친 후에 그 뮤텍스에 잠금 해제 명령(mutex_unlock 함수)을 수행하기만 하면 된다.
 *
 *     뮤텍스는 상호 배제적mutual exclusive 사용 구현에 특화된 세마포어이다. 초기값이 1이며, 일반 세마포어에는 없는 특성을 두 가지 가진다. 물론 일반 세마포어로도 상호 배제적mutual exclusive 사용을 구현할 수 있다. 초기값이 1인 세마포어를 생성한 다음, 자원 사용을 시작하기 전에 그 세마포어에 P 명령(sem_take 함수)을, 자원 사용을 끝마친 후에 그 세마포어에 V 명령(sem_give 함수)을 수행하면 된다. 그러나 일반 세마포어는 가지지 않는, 뮤텍스만이 가지는 특성 두 가지 때문에 상호 배제적mutual exclusive 사용 구현에는 뮤텍스를 사용하는 것이 더 낫다. 그 특성들은 다음과 같다. (유비노스의 뮤텍스는 이 특성 두 가지를 모두 가진다. 그러나 다른 모든 운영체제의 뮤텍스가 그러한 것은 아니다.)
 *
 *     첫 번째 특성은 '뮤텍스는 자신을 잠근 태스크에 한해서 다시 잠그는 것을 허용한다'이다. 이를 '재귀적recursive 잠금을 허용한다'라고 표현하기도 한다. 다음과 같은 함수가 있을 수 있다. 이 함수는 어떤 자원을 상호 배제적으로mutual exclusively 사용해야 한다. 그리고 이 함수는 역시 그 어떤 자원을 상호 배제적으로mutual exclusively 사용해야 하는 또 다른 함수를 호출해야 한다. 만일 세마포어를 사용해 상호 배제적mutual exclusive 사용을 구현하면 이 함수를 호출하는 태스크는 교착deadlock 상태에 빠진다. 이 함수 시작 시점에서 한번, 이 함수가 호출하는 또 다른 함수 시작 시점에서 다시 한번 P 명령(sem_take 함수)을 수행하기 때문이다. 반면 뮤텍스를 사용해 구현하면 태스크가 교착deadlock 상태에 빠지는 것을 방지할 수 있다. 뮤텍스는 자신을 잠근 태스크에 한해서 다시 잠그는 것을 허용하기 때문이다.
 *
 *     두 번째 특성은 '뮤텍스 잠그기를 시도한 태스크의 우선순위가 뮤텍스를 잠금 해제할(뮤텍스를 잠근) 태스크의 우선순위 보다 높을 때, 뮤텍스를 잠금 해제할(뮤텍스를 잠근) 태스크가 그 높은 우선순위를 상속받는다'이다. 이것은 우선순위 상속priority inheritance 특성이라고 불린다. 이 우선순위 상속priority inheritance 특성은 상호 배제적mutual exclusive 사용 때문에 발생할 수 있는 우선순위 역전priority inversion 상황을 방지해준다. 우선순위 역전priority inversion 상황과, 우선순위 상속priority inheritance 특성이 어떻게 이 상황을 방지해주는지는 뮤텍스 예제 부분에서 자세히 설명한다.
 *
 *     - <a href="ubik__mutex_8h.html">뮤텍스 인터페이스</a>
 *
 *     - \subpage mutex_example
 *
 * <br>
 *
 * \section msgq_sec 메시지큐
 *
 *     태스크들은 메시지큐를 사용해 서로 메시지(작은 크기의 데이터)를 주고 받을 수 있다. 메시지큐는 메시지를 큐 방식으로 전달한다. 받은 순서대로 전달하며 전달하지 못한 데이터는 내부에 쌓아둔다.
 *
 *     메시지큐는 메시지 전달 기능이 추가된 세마포어로 볼 수 있다. 메시지를 전달할 수 있다는 점만 빼면 세마포어와 동일하다. 따라서 메시지를 전달함과 동시에 태스크 간 동기화 문제도 해결할 수 있다. 메시지큐의 메시지 수신 명령(msgq_receive 함수)과 송신 명령(msgq_send 함수)이 각각 세마포어의 P 명령(sem_take 함수)과 V 명령(sem_give 함수)의 역할을 대신할 수 있다.
 *
 *     - <a href="ubik__msgq_8h.html">메시지큐 인터페이스</a>
 *
 *     - \subpage msgq_example
 *
 * <br>
 *
 * \section stimer_sec 세마포어 타이머
 *
 *     세마포어 타이머를 사용하면 일정한 주기로 반복적으로 주어진 일을 처리하는 태스크를 쉽게 구현할 수 있다. 세마포어 타이머는 설정된 주기마다 지정한 세마포어에 V 명령(sem_give 함수)을 수행한다. 따라서 태스크를 그 지정한 세마포어에 동기되어 반복적으로 주어진 일을 처리하도록 구현하면, 그 태스크는 일정한 주기로 반복적으로 주어진 일을 처리하게 된다.
 *
 *     - <a href="ubik__stimer_8h.html">세마포어 타이머  인터페이스</a>
 *
 *     - \subpage stimer_example
 *
 * <br>
 *
 * \section intr_sec 인터럽트
 *
 *     인터럽트란 주변장치가 프로세서 코어에게 처리해야 하는 일이 생겼음을 비동기적으로 알리는 수단이다. 일반적으로 인터럽트는 다음 시퀀스 다이어그램sequence diagram처럼 처리된다.
 *
 *     \image html ubik_intr_sequence_chart_general.gif
 *
 *     인터럽트가 발생하면 프로세서 코어는 인터럽트 비활성화 등의 일들을 수행한 다음 제어권을 예외 벡터exception vector에게 넘긴다. 제어권을 넘겨 받은 예외 벡터exception vector는 그 제어권을 사용자가 지정한 인터럽트 서비스 루틴에게 넘긴다. 제어권을 넘겨 받은 인터럽트 서비스 루틴은 기존 작업 내용을 저장하고 인터럽트를 발생 시킨 주변장치가 요구하는 일을 처리한  후, 다시 기존 작업 내용을 복원하고 인터럽트 처리를 종료한다. 그러면 프로세서 코어는 인터럽트 활성화 등의 일들을 처리한 다음 인터럽트 발생 이전 작업을 다시 수행한다.
 *
 *     유비노스 커널의 인터럽트 처리 방식은 다음 시퀀스 다이어그램sequence diagram과 같다. 앞서 언급한 일반적인 인터럽트 처리 방식과 조금 다르다.
 *
 *     \image html ubik_intr_sequence_chart_ubik.gif
 *
 *     인터럽트 발생부터 예외 벡터exception vector로 제어권이 넘어가는 과정까지는 같다. 그러나 예외 벡터exception vector는 제어권을 사용자가 지정한 인터럽트 서비스 루틴이 아닌 유비노스 커널 인터럽트 처리기(ubik_irq_handler)로 넘긴다. 제어권을 넘겨 받은 유비노스 커널 인터럽트 처리기(ubik_irq_handler)는 기존 작업 내용을 저장한 후 제어권을 비에스피 예외 처리기(bsp_exception_handler)로 넘긴다. 제어권을 넘겨 받은 비에스피 예외 처리기(bsp_exception_handler)는 제어권을 사용자 지정 인터럽트 서비스 루틴으로 넘긴다. 제어권을 넘겨 받은 인터럽트 서비스 루틴은 인터럽트를 발생 시킨 주변장치가 요구하는 일을 처리한 후 제어권을 다시 비에스피 예외 처리기(bsp_exception_handler)로 넘긴다. 제어권을 다시 넘겨 받은 비에스피 예외 처리기(bsp_exception_handler)는 인터럽트 처리가 태스크의 수행 순서를 변화시킬 수 있으므로 작업 수행 순서를 다시 정한다(reschedule). 그리고 제어권을 유비노스 커널 인터럽트 처리기(ubik_irq_handler)로 다시 넘긴다. 그러면 유비노스 커널 인터럽트 처리기(ubik_irq_handler)가 다시 수행할 작업 내용을 복원하고 인터럽트 처리를 종료한다. 그 다음 과정은 일반적인 인터럽트 처리 방식과 같다.
 *
 *     이 처리 방식에서는 모든 인터럽트 서비스 루틴이 공통적으로 수행해야 하는 작업 내용 저장과 복원 등의 일을 이미 구현되어 있는 유비노스 커널 인터럽트 처리기(ubik_irq_handler)와 비에스피 예외 처리기(bsp_exception_handler)가 처리한다. 인터럽트 서비스 루틴은 인터럽트를 발생 시킨 주변장치가 요구하는 일 처리만 하면 된다. 따라서 앞서 설명한 일반적인 인터럽트 처리 방식에 비해 인터럽트 서비스 루틴 작성이 쉽다.
 *
 *     - <a href="bsp__intr_8h.html">인터럽트  인터페이스</a>
 *
 *     - \subpage intr_example
 *
 * <br>
 *
 * \section ref_sec 참고 자료
 *
 * - <a href="http://www.ubinos.org">유비노스 홈페이지</a>
 *
 * - <a href="http://www.ubinos.org/phpbb">유비노스 토론장</a> <a href="https://www.ubinos.org/phpbb">(에스에스엘 로그인)</a>
 *
 * <br>
 *
 * Content is available under Attribution-Noncommercial-No Derivative Works 3.0 Unported.
 */

#include "type.h"

#include "../itf/ubiclib.h"
#include "../itf/ubiclib_edlist.h"
#include "../itf/ubiclib_logm.h"
#include "../itf/ubiclib_heap.h"

#include "../itf/ubik.h"
#include "../itf/ubik_hrtick.h"
#include "../itf/ubik_task.h"
#include "../itf/ubik_signal.h"
#include "../itf/ubik_condv.h"
#include "../itf/ubik_mutex.h"
#include "../itf/ubik_sem.h"
#include "../itf/ubik_msgq.h"
#include "../itf/ubik_stimer.h"
#include "../itf/ubik_iobuf.h"

#include "../itf/ubishell.h"

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBINOS_H_ */
