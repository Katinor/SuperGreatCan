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

#ifndef ITF_UBICLIB_LOGM_H_
#define ITF_UBICLIB_LOGM_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file	ubiclib_logm.h
 *
 * @brief	ubiclib (Ubinos C Library) 컴포넌트 로그 메세지 인터페이스
 *
 * ubiclib 컴포넌트가  제공하는 로그 메세지 인터페이스를 정의합니다.
 */

#include "type.h"

/** 모든 카테고리 지정 */
#define LOGM_CATEGORY__ALL			0
#define LOGM_CATEGORY__DEFAULT		1
#define LOGM_CATEGORY__HEAP			2
#define LOGM_CATEGORY__USER00		3
#define LOGM_CATEGORY__USER01		4
#define LOGM_CATEGORY__USER02		5
#define LOGM_CATEGORY__END			6

/** 어떤 메시지도 출력하지 않음 */
#define LOGM_LEVEL__NONE			0
/** 항상 기록되어야 하는 메시지 */
#define LOGM_LEVEL__ALWAYS			1
/** 심각한 수준의 문제에 관한 메시지 */
#define LOGM_LEVEL__FATAL			2
/** 일반적인 문제에 관한 메시지 */
#define LOGM_LEVEL__ERROR			3
/** 경고 메시지 */
#define LOGM_LEVEL__WARNING			4
/** 동작 상태 정보 */
#define LOGM_LEVEL__INFO			5
/** 디버깅시 필요한 기타 정보 */
#define LOGM_LEVEL__DEBUG			6
#define LOGM_LEVEL__END				7

#define LOGM_CATEGORY 				LOGM_CATEGORY__DEFAULT
#define LOGM_LEVEL	 				LOGM_LEVEL__WARNING
#define LOGM_TAG					NULL

#if (UBI_BUILDMODE__DEBUG == UBI_BUILDMODE)

/**
 * 기록할 메시지 수준을 설정하는 함수
 *
 * @param	category 메시지 분류 번호 (0은 모든 분류)
 *
 * @param	level    기록할 메시지 수준 (설정한 수준 이상(같거나 작은 값)의 메시지만 기록됨)
 *
 * @return	설정한 기록할 메시지 수준<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int logm_setlevel(int category, int level);

/**
 * 설정되어 있는 기록할 메시지 수준을 돌려주는 함수
 *
 * @param	category 메시지 분류 번호
 *
 * @return	설정되어 있는 기록할 메시지 수준<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int logm_getlevel(int category);

/**
 * 메시지를 기록하는 함수
 *
 * @param category	메시지 분류 번호
 *
 * @param level		메시지 수준
 *
 * @param format	메시지 형식
 *
 * @param ...		메시지에 포함될 데이터들
 *
 * @return			기록한 메시지 길이<br>
 * 					<br>
 *                 -1: 오류<br>
 */
int logm_printf(int category, int level, const char * tag, const char * format, ...);

/**
 * 버퍼에 문자열을 출력하는 함수
 *
 * @param str		문자열을 출력할 버퍼
 *
 * @param num		문자열 최대 길이
 *
 * @param format	문자열 형식
 *
 * @param ...		문자열에 포함될 데이터들
 *
 * @return			출력한 문자열 길이<br>
 * 					<br>
 *                 -1: 오류<br>
 */
int snprintf(char * str, size_t num, const char * format, ...);

#if !(defined(__SOCHECK__) && defined(__NOSOCHECK__))

/** 메시지를 기록하는 매크로 (LOGM_LEVEL__ALWAYS 수준) */
#define logma(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__ALWAYS,  LOGM_TAG, format, ## args)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL__FATAL 수준) */
#define logmf(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__FATAL,   LOGM_TAG, format, ## args)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL__ERROR 수준) */
#define logme(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__ERROR,   LOGM_TAG, format, ## args)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL__WARNING 수준) */
#define logmw(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__WARNING, LOGM_TAG, format, ## args)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL__INFO 수준) */
#define logmi(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__INFO,    LOGM_TAG, format, ## args)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL__DEBUG 수준) */
#define logmd(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__DEBUG,   LOGM_TAG, format, ## args)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL 기본값 수준) */
#define logm (format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL,          LOGM_TAG, format, ## args)

#else

/** 메시지를 기록하는 매크로 (LOGM_LEVEL__ALWAYS 수준) */
#define logma(format, args...) dtty_puts_nosocheck(format, 120)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL__FATAL 수준) */
#define logmf(format, args...) dtty_puts_nosocheck(format, 120)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL__ERROR 수준) */
#define logme(format, args...) dtty_puts_nosocheck(format, 120)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL__WARNING 수준) */
#define logmw(format, args...)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL__INFO 수준) */
#define logmi(format, args...)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL__DEBUG 수준) */
#define logmd(format, args...)
/** 메시지를 기록하는 매크로 (LOGM_LEVEL 기본값 수준) */
#define logm (format, args...)

#endif

#else

#define logm_setlevel(category, level)					(LOGM_LEVEL__NONE)
#define logm_getlevel(category)							(LOGM_LEVEL__NONE)
#define logm_printf(category, level, tag, format, args...)

#define logma(format, args...)
#define logmf(format, args...)
#define logme(format, args...)
#define logmw(format, args...)
#define logmi(format, args...)
#define logmd(format, args...)
#define logm (format, args...)

#endif

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBICLIB_LOGM_H_ */
