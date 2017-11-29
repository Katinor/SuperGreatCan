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
 * @brief ubik (Ubinos Kernel) 컴포넌트 상태변수 인터페이스
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

/** 상태변수 포인터 형 정의 */
typedef	_condv_tip_t *	condv_pt;

/**
 * 상태변수를 생성하는 함수
 *
 * @param	condv_p		생성한 상태변수의 주소를 저장할 포인터의 주소
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int condv_create(condv_pt * condv_p);

/**
 * 상태변수를 생성하는 함수 (확장형)
 *
 * @param	condv_p		생성한 상태변수의 주소를 저장할 포인터의 주소
 *
 * @param	option		옵션 (CONDV_OPT__...)<br>
 * 						0: 기본 설정<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int condv_create_ext(condv_pt * condv_p, unsigned int option);

/**
 * 상태변수를 제거하는 함수
 *
 * @param	condv_p		제거할 상태변수의 주소가 저장된 포인터의 주소<br>
 * 						제거에 성공하면 *condv_p는 NULL이 됨
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int condv_delete(condv_pt * condv_p);

/**
 * 상태변수가 변경되기를 기다리는 함수
 *
 * @param	condv			대상 상태변수 포인터
 *
 * @param	mutex			대상 상태변수를 보호하는 뮤텍스
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int condv_wait(condv_pt condv, mutex_pt mutex);

/**
 * 제한 시간동안 상태변수가 변경되기를 기다리는 함수
 *
 * @param	condv			대상 상태변수 포인터
 *
 * @param	mutex			대상 상태변수를 보호하는 뮤텍스
 *
 * @param	tick			제한 시간 (시스템 틱 수)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__TIMEOUT		: 제한 시간 초과<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int condv_wait_timed(condv_pt condv, mutex_pt mutex, unsigned int tick);

/**
 * 제한 시간동안 상태변수가 변경되기를 기다리는 함수 (천분의 일초 단위)
 *
 * @param	condv			대상 상태변수 포인터
 *
 * @param	mutex			대상 상태변수를 보호하는 뮤텍스
 *
 * @param	timems			제한 시간 (천분의 일초)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__TIMEOUT		: 제한 시간 초과<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int condv_wait_timedms(condv_pt condv, mutex_pt mutex, unsigned int timems);

/**
 * 기다리고 있는 태스크들 중 하나에게 상태변수가 변경되었음을 알려주는 함수
 *
 * @param	condv		대상 상태변수 포인터
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int condv_signal(condv_pt condv);

/**
 * 기다리고 있는 태스크들모두에게 상태변수가 변경되었음을 알려주는 함수
 *
 * @param	condv		대상 상태변수 포인터
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int condv_broadcast(condv_pt condv);

/**
 * 상태변수가 변경되었음을 알려주는 (상태변수를 기다리는 최상위 우선순위 태스크의우선순위를 상속받을) 태스크를 설정하는 함수
 *
 * @param	condv		대상 상태변수 포인터
 *
 * @param	task		대상 태스크 포인터<br>
 * 						NULL이면 등록된 태스크 제거<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int condv_setsender(condv_pt condv, task_pt task);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_CONDV_H_ */
