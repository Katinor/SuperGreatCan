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

#ifndef ITF_UBICLIB_H_
#define ITF_UBICLIB_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubiclib.h
 *
 * @brief ubiclib (Ubinos C Library) 컴포넌트 인터페이스
 *
 * ubiclib 컴포넌트 인터페이스를 정의합니다.
 */

#include "type.h"

/**
 * ubiclib 컴포넌트를 초기화하는 함수
 *
 * 초기화 작업들 중 재진입(reenterance)을 지원하기 위한 부분은 수행하지 않습니다.<br>
 * 재진입(reenterance)을 지원하려면 ubiclib_comp_init_reent 함수를 추가로 호출해 주어야 합니다.<br>
 * 이 함수는 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int ubiclib_comp_init(void);

/**
 * ubiclib 컴포넌트의 초기화 작업들 중 재진입(reenterance)을 지원하기 위한 부분을 수행하는 함수
 *
 * 이 함수는 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int ubiclib_comp_init_reent(void);

/**
 * 정수값을 10진수 아스키 코드 문자열로 변환하는 함수
 *
 * @param	value	변환할 정수값
 *
 * @param	buf		변환한 문자열을 저장할 버퍼
 *
 * @param	max		변환한 문자열을 저장할 버퍼의 최대 크기
 *
 * @return	변환된 문자열 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int itoa(int value, char * buf, int max);

/**
 * 정수값을 16진수 아스키 코드 문자열로 변환하는 함수
 *
 * @param	value	변환할 정수값
 *
 * @param	buf		변환한 문자열을 저장할 버퍼
 *
 * @param	max		변환한 문자열을 저장할 버퍼의 최대 크기
 *
 * @return	변환된 문자열 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int itoah(int value, char * buf, int max);

/**
 * 정수값의 바이트 순서를 big endian으로 변환하는 함수
 *
 * @param	value	변환할 값
 *
 * @return	변환된 값
 */
int htobi(int value);

/**
 * 언사인드 정수값을 10진수 아스키 코드 문자열로 변환하는 함수
 *
 * @param	value	변환할 언사인드 정수값
 *
 * @param	buf		변환한 문자열을 저장할 버퍼
 *
 * @param	max		변환한 문자열을 저장할 버퍼의 최대 크기
 *
 * @return	변환된 문자열 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int uitoa(unsigned int value, char * buf, int max);

/**
 * 언사인드 정수값을 10진수 아스키 코드 문자열로 변환하는 함수 (스택 오버플로우 검사 생략)
 *
 * @param	value	변환할 언사인드 정수값
 *
 * @param	buf		변환한 문자열을 저장할 버퍼
 *
 * @param	max		변환한 문자열을 저장할 버퍼의 최대 크기
 *
 * @return	변환된 문자열 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int uitoa_nosocheck(unsigned int value, char * buf, int max);

/**
 * 언사인드 정수값을 16진수 아스키 코드 문자열로 변환하는 함수
 *
 * @param	value	변환할 언사인드 정수값
 *
 * @param	buf		변환한 문자열을 저장할 버퍼
 *
 * @param	max		변환한 문자열을 저장할 버퍼의 최대 크기
 *
 * @return	변환된 문자열 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int uitoah(unsigned int value, char * buf, int max);

/**
 * 언사인드 정수값을 16진수 아스키 코드 문자열로 변환하는 함수 (스택 오버플로우 검사 생략)
 *
 * @param	value	변환할 언사인드 정수값
 *
 * @param	buf		변환한 문자열을 저장할 버퍼
 *
 * @param	max		변환한 문자열을 저장할 버퍼의 최대 크기
 *
 * @return	변환된 문자열 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int uitoah_nosocheck(unsigned int value, char * buf, int max);

/**
 * 언사인드 정수값의 바이트 순서를 big endian으로 변환하는 함수
 *
 * @param	value	변환할 값
 *
 * @return	변환된 값
 */
unsigned int htobui(unsigned int value);

/**
 * 16진수 아스키 코드 문자열을 정수값으로 변환하는 함수
 *
 * @param	buf		변환할 문자열이 저장된 버퍼
 *
 * @return	변환된 값
 */
int ahtoi(const char * buf);

/**
 * 16진수 아스키 코드 문자열을 롱 정수값으로 변환하는 함수
 *
 * @param	buf		변환할 문자열이 저장된 버퍼
 *
 * @return	변환된 값
 */
long ahtol(const char * buf);

/**
 * 10진수 아스키 코드 문자열을 언사인드 정수값으로 변환하는 함수
 *
 * @param	buf		변환할 문자열이 저장된 버퍼
 *
 * @return	변환된 값
 */
unsigned int atoui(const char * buf);

/**
 * 10진수 아스키 코드 문자열을 언사인드 롱 정수값으로 변환하는 함수
 *
 * @param	buf		변환할 문자열이 저장된 버퍼
 *
 * @return	변환된 값
 */
unsigned long atoul(const char * buf);

/**
 * 16진수 아스키 코드 문자열을 언사인드 정수값으로 변환하는 함수
 *
 * @param	buf		변환할 문자열이 저장된 버퍼
 *
 * @return	변환된 값
 */
unsigned int ahtoui(const char * buf);

/**
 * 16진수 아스키 코드 문자열을 언사인드 롱 정수값으로 변환하는 함수
 *
 * @param	buf		변환할 문자열이 저장된 버퍼
 *
 * @return	변환된 값
 */
unsigned long ahtoul(const char * buf);

#undef toupper
#undef tolower

/**
 * 아스키 코드 소문자를 대문자로 변환하는 함수
 *
 * @param	c		변환할 문자
 *
 * @return	변환된 문자
 */
int toupper ( int c );

/**
 * 아스키 코드 대문자를 소문자로 변환하는 함수
 *
 * @param	c		변환할 문자
 *
 * @return	변환된 문자
 */
int tolower ( int c );

/**
 * 메모리를 0으로 초기화하는 함수
 *
 * @param	dst		초기화할 메모리 주소
 *
 * @param	num		초기화할 메모리 크기
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int memset0(void * dst, size_t num);

/**
 * 부호 없는 정수 지수승 연산을 수행하는 함수
 *
 * @param	x		지수
 *
 * @param	y		승수
 *
 * @return	y power of x<br>
 */
unsigned int uipow(unsigned int x, unsigned int y);

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBICLIB_H_ */
