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

#ifndef ITF_BSP_INTR_H_
#define ITF_BSP_INTR_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file bsp_intr.h
 *
 * @brief BSP (Board Support Package) 컴포넌트 인터럽트 인터페이스
 *
 * BSP 컴포넌트가  제공하는 인터럽트 인터페이스를 정의합니다.
 */

/**
 * @example	intrtest00.c
 */

/**
 * \page intr_example 인터럽트 예제
 *
 *         \section intrtest00_sec 간단한 인터럽트 처리 예제
 *             Simple interrupt handling example
 *
 *             - <a href="intrtest00_8c-example.html">intrtest00.c</a>
 *
 *             이 예제는 sam7x256ek 보드의 하드웨어 타이머를 1초를 주기로 반복적으로 인터럽트를 발생시키도록 설정하고,
 *             그 인터럽트를 받아 처리한다.<br>
 *
 * <br>
 *
 * Content is available under Attribution-Noncommercial-No Derivative Works 3.0 Unported.
 */

#include "type.h"

/** 인터럽트 유형 선택 사항: 	엣지 감지edge triggered */
#define	INTR_OPT__EDGE			0x00

/** 인터럽트 유형 선택 사항: 	레벨 감지level sensitive */
#define	INTR_OPT__LEVEL			0x80

/** 인터럽트 유형 선택 사항: 	엣지 감지edge triggered 선택 사항과 함께 쓰일 경우 네거티브 엣지 감지negative edge triggered,
							레벨 감지level sensitive 선택 사항과 함께 쓰일 경우 로우 레벨 감지low level sensitive */
#define	INTR_OPT__LOW			0x00

/** 인터럽트 유형 선택 사항: 	엣지 감지edge triggered 선택 사항과 함께 쓰일 경우 포지티브 엣지 감지positive edge triggered,
							레벨 감지level sensitive 선택 사항과 함께 쓰일 경우 하이 레벨 감지high level sensitive */
#define	INTR_OPT__HIGH			0x40

/** 인터럽트 서비스 루틴 함수 포인터  형 정의  */
typedef void (* isr_ft) (void);

/**
 * 대상 인터럽트에 인터럽트 서비스 루틴을 연결시키는 함수
 *
 * @param	no			대상 인터럽트 번호<br>
 *						<br>
 *
 * @param	isr			인터럽트 서비스 루틴 함수 포인터<br>
 *						<br>
 *
 * @param	priority	우선 순위<br>
 *						<br>
 *
 * @param	option		선택 사항 (BSP_INTR_OPT__...)<br>
 * 						0: 네거티브 엣지 감지negative edge triggered<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int intr_connectisr(int no, isr_ft isr, int priority, unsigned int option);

/**
 * 대상 인터럽트를 활성화하는 함수
 *
 * @param	no			대상 인터럽트 번호<br>
 *						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int intr_enable(int no);

/**
 * 대상 인터럽트를 비활성화하는 함수
 *
 * @param	no			대상 인터럽트 번호<br>
 *						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int intr_disable(int no);

/**
 * 대상 펜딩 인터럽트를 클리어하는 함수
 *
 * @param	no			대상 인터럽트 번호<br>
 *						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int intr_ClearPending(int no);

/**
 * 현재 처리중인 인터럽트 번호를 돌려주는 함수
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -2: 인터럽트 처리중이 아님<br>
 * 			 -3: 이 기능을 지원하지 않음<br>
 */
int intr_getcurno();

/**
 * 인터럽트가 가질 수 있는 최고 우선순위를 돌려주는 함수
 *
 * @return	인터럽트가 가질 수 있는 최고 우선순위
 */
int intr_gethighestpriority(void);

/**
 * 인터럽트가 가질 수 있는 최저 우선순위를 돌려주는 함수
 *
 * @return	인터럽트가 가질 수 있는 최저 우선순위
 */
int intr_getlowestpriority(void);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_BSP_INTR_H_ */
