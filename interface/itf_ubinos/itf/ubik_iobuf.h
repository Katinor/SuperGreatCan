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

#ifndef ITF_UBIK_IOBUF_H_
#define ITF_UBIK_IOBUF_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubik_iobuf.h
 *
 * @brief ubik (Ubinos Kernel) 컴포넌트 입출력버퍼 인터페이스
 *
 * ubik 컴포넌트가  제공하는 입출력버퍼 인터페이스를 정의합니다.
 */

/**
 * @example	iobuftest01.c
 * @example	iobuftest02.c
 * @example	iobuftest03.c
 * @example	iobuftest04.c
 * @example	iobuftest05.c
 */

#include "type.h"

#include "../itf/ubik_task.h"

/** 선택 사항: 버퍼가 가득차면 새 데이터를 버림 */
#define IOBUF_OPT__NOOVERWRITE		0x0001

typedef	struct __iobuf_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _iobuf_tip_t;

/** 입출력버퍼 포인터 형 정의 */
typedef _iobuf_tip_t *	iobuf_pt;

/**
 * 입출력버퍼를 생성하는 함수
 *
 * @param	iobuf_p		생성한 입출력버퍼의 주소를 저장할 포인터의 주소
 *
 * @param	maxsize		버퍼 최대 크기
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int iobuf_create(iobuf_pt * iobuf_p, unsigned int maxsize);

/**
 * 입출력버퍼를 생성하는 함수 (확장형)
 *
 * @param	iobuf_p		생성한 입출력버퍼의 주소를 저장할 포인터의 주소
 *
 * @param	maxsize		버퍼 최대 크기
 *
 * @param	option		옵션 (IOBUF_OPT__...)<br>
 * 						0: 기본 설정<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int iobuf_create_ext(iobuf_pt * iobuf_p, unsigned int maxsize, unsigned int option);

/**
 * 입출력버퍼를 제거하는 함수
 *
 * @param	iobuf_p		제거할 입출력버퍼의 주소가 저장된 포인터의 주소<br>
 * 						제거에 성공하면 *iobuf_p는 NULL이 됨
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int iobuf_delete(iobuf_pt * iobuf_p);

/**
 * 입출력버퍼에 데이터를 쓰는 함수
 *
 * @param	iobuf		대상 입출력버퍼 포인터
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
int iobuf_write(iobuf_pt iobuf, unsigned char * buf, unsigned int size, unsigned int * written_p);

/**
 * 입출력버퍼에서 데이터를 읽는 함수
 *
 * @param	iobuf		대상 입출력버퍼 포인터
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
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int iobuf_read(iobuf_pt iobuf, unsigned char * buf, unsigned int size, unsigned int * read_p);

/**
 * 입출력버퍼에서 데이터를 읽는 함수 (데이터가 없으면 제한 시간동안 기다림)
 *
 * @param	iobuf		대상 입출력버퍼 포인터
 *
 * @param	buf			읽은 데이터를 저장할 버퍼
 *
 * @param	size		읽을 데이터 크기
 *
 * @param	read_p		읽은 데이터 크기를 저장할 변수의 주소 (NULL이면 무시)
 *
 * @param	tick		제한 시간 (시스템 틱 수)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__TIMEOUT		: 제한 시간 초과<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int iobuf_read_timed(iobuf_pt iobuf, unsigned char * buf, unsigned int size, unsigned int * read_p, unsigned int tick);

/**
 * 입출력버퍼에 읽을 데이터가 준비되기를 기다리는 함수
 *
 * @param	iobuf		대상 입출력버퍼 포인터
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int iobuf_wait(iobuf_pt iobuf);

/**
 * 제한 시간동안 입출력버퍼에 읽을 데이터가 준비되기를 기다리는 함수
 *
 * @param	iobuf		대상 입출력버퍼 포인터
 *
 * @param	tick		제한 시간 (시스템 틱 수)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			UBIK_ERR__TIMEOUT		: 제한 시간 초과<br>
 * 			UBIK_ERR__DEADLOCK		: 데드락 발생 가능성이 있음<br>
 * 			UBIK_ERR__TERMINATED	: 기다리던 객체가 제거되었음<br>
 */
int iobuf_wait_timed(iobuf_pt iobuf, unsigned int tick);

/**
 * 입출력버퍼에 들어있는 데이터의 크기를 돌려주는 함수
 *
 * @param	iobuf		대상 입출력버퍼 포인터
 *
 * @param	size_p		크기를 저장할 변수의 주소
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int iobuf_getsize(iobuf_pt iobuf, unsigned int * size_p);

/**
 * 입출력버퍼의 데이터를 모두 지우는 함수
 *
 * @param	iobuf		대상 입출력버퍼 포인터
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int iobuf_clear(iobuf_pt iobuf);

/**
 * 입출력버퍼에 데이터를 쓰는 (데이터를 기다리는 최상위 우선순위 태스크의우선순위를 상속받을) 태스크를 설정하는 함수
 *
 * @param	iobuf		대상 입출력버퍼 포인터
 *
 * @param	task		대상 태스크 포인터<br>
 * 						NULL이면 등록된 태스크 제거<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int iobuf_setsender(iobuf_pt iobuf, task_pt task);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBIK_IOBUF_H_ */
