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

#ifndef ITF_BSP_DTTY_H_
#define ITF_BSP_DTTY_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file bsp_dtty.h
 *
 * @brief BSP (Board Support Package) 컴포넌트 디버깅 터미널 인터페이스
 *
 * BSP 컴포넌트가  제공하는 디버깅 터미널 인터페이스를 정의합니다.
 */

#include "type.h"

/**
 * 디버깅 터미널을 초기화하는 함수
 *
 * 이 함수는 시스템 시작시 bsp_comp_init 함수에서 자동적으로 호출됩니다.
 *
 * @return	 0: 성공<br>
 * 			<br>
 * 			-1: 오류
 */
int dtty_init(void);

/**
 * 디버깅 터미널을 활성화하는 함수
 *
 * 이 함수는 dtty_init 함수에서 자동적으로 호출됩니다.
 *
 * @return	 0: 성공<br>
 * 			<br>
 * 			-1 : 오류
 * 			-10: 시스템이 이 함수를 지원하지 않음
 */
int dtty_enable(void);

/**
 * 디버깅 터미널을 비활성화하는 함수
 *
 * @return	 0: 성공<br>
 * 			<br>
 * 			-1 : 오류
 * 			-10: 시스템이 이 함수를 지원하지 않음
 */
int dtty_disable(void);

/**
 * 디버깅 터미널에 문자를 출력하는 함수
 *
 * @param	ch		출력할 문자
 *
 * @return	 0: 성공<br>
 * 			<br>
 * 			-1: 오류
 */
int dtty_putc(int ch);

/**
 * 디버깅 터미널에 문자를 출력하는 함수 (스택 오버플로우 검사를 하지 않음)
 *
 * @param	ch		출력할 문자
 *
 * @return	 0: 성공<br>
 * 			<br>
 * 			-1: 오류
 */
int dtty_putc_nosocheck(int ch);

/**
 * 디버깅 터미널에서 문자를 입력받는 함수
 *
 * @param	ch_p	입력받은 문자를 저장할 변수의 주소
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_getc(char * ch_p);

/**
 * 디버깅 터미널에 문자열을 출력하는 함수
 *
 * @param	str		출력할 문자열
 *
 * @param	max		출력할 문자열의 최대 크기
 *
 * @return	 출력한 문자열의 크기
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_puts(const char * str, int max);

/**
 * 디버깅 터미널에서 문자열을 입력받는 함수
 *
 * '\n', '\r', 또는 '\0' 이 입력되거나 최대 크기보다 작을 때까지 입력받는다.
 * 입력받은 '\n', '\r'은 퍼버에 저장되지 않는다.
 *
 * @param	str		입력받은 문자열을 저장할 버퍼
 *
 * @param	max		입력받을 문자열의 최대 크기
 *
 * @return	 입력받은 문자열의 크기
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_gets(char * str, int max);

/**
 * 디버깅 터미널에 문자열을 출력하는 함수 (스택 오버플로우 검사를 하지 않음)
 *
 * @param	str		출력할 문자열
 *
 * @param	max		출력할 문자열의 최대 크기
 *
 * @return	 출력한 문자열의 크기
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_puts_nosocheck(const char * str, int max);

/**
 * 디버깅 터미널 입력 버퍼에 입력받은 문자가 있는지를 검사하는 함수
 *
 * @return	 1: 있음<br>
 * 			 0: 없음<br>
 */
int dtty_kbhit(void);

/**
 * 디버깅 터미널 에코 설정 함수
 *
 * @param	echo	0: echo off<br>
 * 					1: echo on<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int dtty_setecho(int echo);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_BSP_DTTY_H_ */
