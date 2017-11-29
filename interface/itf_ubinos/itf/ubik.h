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

#ifndef ITF_UBIK_H_
#define ITF_UBIK_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubik.h
 *
 * @brief ubik (Ubinos Kernel) 컴포넌트 인터페이스
 *
 * ubik 컴포넌트 인터페이스를 정의합니다.
 */

#include "type.h"

/** 오류: 오류 없음 */
#define	UBIK_ERR__SUCCESS				  0
/** 오류: 실패 */
#define	UBIK_ERR__FAIL					 -1

/** 오류: 제한 시간 초과 */
#define	UBIK_ERR__TIMEOUT				-20
/** 오류: 교착 상태에 빠질 가능성이 있음 */
#define	UBIK_ERR__DEADLOCK				-21
/** 오류: 대상 객체가 제거되었음 */
#define	UBIK_ERR__TERMINATED			-30
/** 오류: 오버플로우 발생 */
#define	UBIK_ERR__OVERFLOWED			-50

/** 사용자 태그 시작값 */
#define UBIK_TAG__USRSTART				0x8000

/** 아이들 태스크 후크 함수 포인터  형 정의 */
typedef int (* idletaskhookfunc_ft)(void *);

/** 아이들 태스크 후크 함수 선택 사항: 설정된 후크 함수를 반복해서 수행함(기본 설정) */
#define IDLEHOOKFUNC_OPT__REPEAT		0x00
/** 아이들 태스크 후크 함수 선택 사항: 설정된 후크 함수를 한번만 수행함 */
#define IDLEHOOKFUNC_OPT__ONCE			0x80

/** 틱 후크 함수 포인터  형 정의 */
typedef void (* tickhookfunc_ft) (void);

/**
 * tick 수 자료 구조
 */
typedef struct _tickcount_t {
	unsigned int	high;
	unsigned int	low;
} tickcount_t;

/**
 * @typedef	tickcount_t
 *
 * @brief	tick 수 형 정의
 */

/**
 * ubik 컴포넌트를 초기화하는 함수
 *
 * 이 함수는 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.
 *
 * @param	idle_stackdepth: idle task 스택 깊이(스택 영역 크기는 stackdepth * INT_SIZE 바이트가 됨)<br>
 * 			0: 기본  스택 깊이 값(task_getdefaultstackdepth 함수가 돌려주는 값)<br>
 * 			1 ~ 최저 스택 깊이 값: 최저 스택 깊이 값(task_getminstackdepth 함수가 돌려주는 값)<br>
 * 			<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int ubik_comp_init(unsigned int idle_stackdepth);

/**
 * ubik 컴포넌트를 시작하는 함수
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int ubik_comp_start(void);

/**
 * 초당 tick 수를 돌려주는 함수
 *
 * @return	초당 tick 수<br>
 */
unsigned int ubik_gettickpersec(void);

/**
 * 현재까지 누적된 tick 수를 돌려주는 함수
 *
 * @return	현재까지 누적된 tick 수<br>
 */
tickcount_t ubik_gettickcount(void);

/**
 * tick 수의 차이(tick2에서 tick1을 뺀 값)를 돌려주는 함수
 *
 * 비교 대상 tick 1이 비교 대상 tick 2보다 클 경우 오버플로우가 일어난 것으로 간주하고 차이를 계산함
 *
 * @param	tick1		비교 대상 tick 1<br>
 * 			<br>
 *
 * @param	tick2		비교 대상 tick 2<br>
 * 			<br>
 *
 * @return	tick 수의 차이<br>
 */
tickcount_t ubik_gettickdiff(tickcount_t tick1, tickcount_t tick2);

/**
 * 시간(천분의 일초)을 시스템 tick 수로 환산하는 함수
 *
 * @param	timems	시간(천분의 일초)<br>
 * 			<br>
 *
 * @return	시스템 tick 수<br>
 * 			시스템 tick 수로 환산했을 때 0이지만 timems가 0이 아니면 1<br>
 */
unsigned int ubik_timemstotick(unsigned int timems);

/**
 * 시스템 tick 수를 시간(천분의 일초)으로 환산하는 함수
 *
 * @param	tick	시스템 tick 수<br>
 * 			<br>
 *
 * @return	시간(천분의 일초)<br>
 * 			시간(천분의 일초)으로 환산했을 때 0이지만 tick이 0이 아니면 1<br>
 */
unsigned int ubik_ticktotimems(unsigned int tick);

/**
 * 틱 후크 함수를 등록하는 함수
 *
 * 틱 후크 함수는 시스템 틱 인터럽트가 발생할 때마다 자동적으로 호출된다.
 *
 * @param	틱 후크 함수 포인터<br>
 * 			NULL: 틱 후크 함수 등록 해제<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int ubik_settickhookfunc(tickhookfunc_ft tickhookfunc);

/**
 * ubik 컴포넌트가 active 상태인지 여부를 돌려주는 함수
 *
 * @return	1: active<br>
 * 			0: inactive<br>
 */
int ubik_isactive(void);

/**
 * 크리티컬 섹션으로 들어가는 함수
 *
 */
void ubik_entercrit(void);

/**
 * 크리티컬 섹션으로 들어가는 함수 (스택 오버플로우 검사 생략)
 *
 */
void ubik_entercrit_nosocheck(void);

/**
 * 크리티컬 섹션에서 빠져나오는 함수
 *
 */
void ubik_exitcrit(void);

/**
 * 크리티컬 섹션에서 빠져나오는 함수 (스택 오버플로우 검사 생략)
 *
 */
void ubik_exitcrit_nosocheck(void);

/**
 * 크리티컬 섹션 또는 인터럽트 서비스 루틴을 수행 중인지 여부를 알려주는 함수
 *
 * @return	1: 크리티컬 섹션 또는 인터럽트 서비스 루틴을 수행 중임<br>
 * 			0: 크리티컬 섹션 또는 인터럽트 서비스 루틴을 수행 중이 아님<br>
 */
int ubik_iscrit(void);

/**
 * 태스크를 수행 중인지 여부를 알려주는 함수
 *
 * @return	1: 태스크를 수행 중임<br>
 * 			0: 시스템 초기화 루틴 또는 인터럽트 서비스 루틴을 수행 중임<br>
 */
int ubik_istask(void);

/**
 * 사용 중이 아닌 자원들을 정리하는 함수
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int ubik_collectgarbage(void);

/**
 * 커널 정보를 문자열 형태로 메모리 버퍼에 기록하는 함수
 *
 * @param	buf		기록할 메모리  버퍼<br>
 * 			<br>
 *
 * @param	max		메모리 버퍼의 크기<br>
 * 			<br>
 *
 * @return	기록된 문자열 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_sprintkernelinfo(char * buf, int max);

/**
 * 커널 정보를 출력하는 함수
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int ubik_printkernelinfo(void);

/**
 * 아이들 태스크 후크 함수를 설정하는 함수
 *
 * 설정된 후크 함수는 아이들 태스크 내에서 호출됨<br>
 * <br>
 * 아이들 태스크는 일반 태스크와는 달리 task_sleep 함수를 호출할 경우  바쁜 기다림busy waiting을 수행한다.
 * 그리고 sem_take, sem_take_timed 함수와 같은 특정 조건이 만족되기를  기다리는 함수를 호출할 경우
 * 스핀 기다림spin waiting을 수행하기 때문에 최대 IDLETASK_SPINWAIT_INTERVALTICK 으로 정의된 시스템 틱 수 만큼
 * 응답 시간이 지연될 수 있다.
 *
 * @param	func		설정할 후크 함수 포인터<br>
 * 						NULL: 후크 함수 설정 해제<br>
 * 						<br>
 *
 * @param	arg			후크 함수로 전달할 매개변수<br>
 * 						<br>
 *
 * @param	name		후크 함수 이름<br>
 * 						NULL: 후크 함수 이름을 지정하지 않음<br>
 * 						<br>
 *
 * @param	option		선택 사항 (IDLEHOOKFUNC_OPT__...)<br>
 * 						0: 기본 설정 사용<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_setidletaskhookfunc(idletaskhookfunc_ft func, void * arg, const char * name, unsigned int option);

/**
 * 실시간 태스크 우선순위를 설정하는 함수
 *
 * 이 함수로 설정된 실시간 태스크 우선순위 이상의 우선 순위를 가진 태스크를 실시간 태스크로 간주한다.<br>
 * 실기간 태스트 우선순위가 설정되어 있지 않으면 모든 태스크를 비실시간 태스크로 간주한다.<br>
 *
 * @param	rtpriority	실시간 태스크 우선순위
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_setrtpriority(int rtpriority);

/**
 * 실시간 태스크 우선순위를 가져오는 함수
 *
 * @return	실시간 태스크 우선순위<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_getrtpriority(void);

/**
 * 실시간 태스크, 태스크 잠금 상태의 태스크, 크리티컬 섹션, 또는 인터럽트 서비스 루틴을 수행 중인지 여부를 알려주는 함수
 *
 * @return	  1: 실시간 태스크, 태스크 잠금 상태의 태스크, 크리티컬 섹션, 또는 인터럽트 서비스 루틴을 수행 중임<br>
 * 			  0: 실시간 태스크, 태스크 잠금 상태의 태스크, 크리티컬 섹션, 또는 인터럽트 서비스 루틴을 수행 중이 아님<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int ubik_isrt(void);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_H_ */
