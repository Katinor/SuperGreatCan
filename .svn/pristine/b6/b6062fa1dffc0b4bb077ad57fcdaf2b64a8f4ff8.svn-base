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

#ifndef ITF_UBICLIB_CIRBUF_H_
#define ITF_UBICLIB_CIRBUF_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubiclib_cirbuf.h
 *
 * @brief ubiclib (Ubinos C Library) 컴포넌트 환형버퍼 인터페이스
 *
 * ubiclib 컴포넌트가  제공하는 환형버퍼 인터페이스를 정의합니다.
 */

#include "type.h"

/** 선택 사항: 버퍼가 가득차면 새 데이터를 버림 */
#define CIRBUF_OPT__NOOVERWRITE		0x0001

/** 선택 사항: 다중 태스크 환경에서 사용할 수 있음 (데이터는 손실될 수 있음) */
#define CIRBUF_OPT__MTPROTECTION	0x0002

/** 환형버퍼 자료 구조 */
typedef	struct _cirbuf_t {
	unsigned char *		head;
	unsigned char *		tail;
	unsigned int		size;

	unsigned int		overflowcount	: 16;

	unsigned int		mtprotection	:  1;
	unsigned int		overwrite		:  1;
	unsigned int		reserved		: 14;

	unsigned int		maxsize;
	unsigned char *		buf;
} cirbuf_t;

/**
 * @var typedef struct _cirbuf_t cirbuf_t
 *
 * 객체 형 정의
 */

/** 환형버퍼 포인터 형 정의 */
typedef cirbuf_t * cirbuf_pt;

/**
 * 환형버퍼를 초기화하는 매크로
 *
 * @param	cirbuf		대상 환형버퍼 포인터
 *
 * @param	maxsize		버퍼 최대 크기
 */
#define cirbuf_init(cirbuf, maxsize)                   	                                        \
{                                                                                               \
	(cirbuf)->maxsize		= maxsize;                                                          \
	(cirbuf)->buf			= (unsigned char *) ((unsigned int) cirbuf + sizeof(cirbuf_t));     \
                                                                                                \
	(cirbuf)->head			= (cirbuf)->buf;                                                    \
	(cirbuf)->tail			= (cirbuf)->buf;                                                    \
	(cirbuf)->size			= 0;                                                                \
	                                                                                            \
	(cirbuf)->overflowcount	= 0;                                                                \
	(cirbuf)->mtprotection	= 0;                                                                \
	(cirbuf)->overwrite		= 0;                                                                \
	(cirbuf)->reserved		= 0;                                                                \
}

/**
 * 환형버퍼를 생성하는 함수
 *
 * @param	cirbuf_p	생성한 환형버퍼의 주소를 저장할 포인터의 주소
 *
 * @param	maxsize		버퍼 최대 크기
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int cirbuf_create(cirbuf_pt * cirbuf_p, unsigned int maxsize);

/**
 * 환형버퍼를 생성하는 함수 (확장형)
 *
 * @param	cirbuf_p	생성한 환형버퍼의 주소를 저장할 포인터의 주소
 *
 * @param	maxsize		버퍼 최대 크기
 *
 * @param	option		옵션 (CIRBUF_OPT__...)<br>
 * 						0: 기본 설정<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int cirbuf_create_ext(cirbuf_pt * cirbuf_p, unsigned int maxsize, unsigned int option);

/**
 * 환형버퍼를 제거하는 함수
 *
 * @param	cirbuf_p	제거할 환형버퍼의 주소가 저장된 포인터의 주소<br>
 * 						제거에 성공하면 *cirbuf_p는 NULL이 됨
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int cirbuf_delete(cirbuf_pt * cirbuf_p);

/**
 * 환형버퍼에 데이터를 쓰는 함수
 *
 * @param	cirbuf		대상 환형버퍼 포인터
 *
 * @param	buf			쓸 데이터가 저장되어 있는 버퍼
 *
 * @param	size		쓸 데이터 크기
 *
 * @param	written_p	쓴 데이터 크기를 저장할 변수의 주소 (NULL이면 무시)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int cirbuf_write(cirbuf_pt cirbuf, unsigned char * buf, unsigned int size, unsigned int * written_p);

/**
 * 환형버퍼에서 데이터를 읽는 함수
 *
 * @param	cirbuf		대상 환형버퍼 포인터
 *
 * @param	buf			읽은 데이터를 저장할 버퍼
 *
 * @param	size		읽을 데이터 크기
 *
 * @param	read_p		읽은 데이터 크기를 저장할 변수의 주소 (NULL이면 무시)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int cirbuf_read(cirbuf_pt cirbuf, unsigned char * buf, unsigned int size, unsigned int * read_p);

/**
 * 환형버퍼의 데이터를 모두 지우는 함수
 *
 * @param	cirbuf		대상 환형버퍼 포인터
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int cirbuf_clear(cirbuf_pt cirbuf);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBICLIB_CIRBUF_H_ */
