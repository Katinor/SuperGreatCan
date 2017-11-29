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

#ifndef ITF_UBICLIB_LIST_H_
#define ITF_UBICLIB_LIST_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubiclib_list.h
 *
 * @brief ubiclib (Ubinos C Library) 컴포넌트 리스트 인터페이스
 *
 * ubiclib 컴포넌트가  제공하는 리스트 인터페이스를 정의합니다.
 */

#include "type.h"

/** 오류: 해당 엘리먼트가 없음 */
#define LIST_ERR__NOTEXIST		-21

/** 리스트 엘리먼트 자료 구조 */
typedef	struct _list_elmt_t {
	struct _list_elmt_t *	next;
	void * 					data;
} list_elmt_t;

/**
 * @var typedef	struct _list_elmt_t list_elmt_t
 *
 * 리스트 엘리먼트 형 정의
 */

/** 리스트 엘리먼트 포인터 형 정의 */
typedef list_elmt_t * list_elmt_pt;

/**
 * 리스트 엘리먼트를 생성하는 함수
 *
 * @param	data		 엘리먼트의 데이터
 *
 * @return	생성한 리스트 엘리먼트 포인터<br>
 * 			<br>
 *          NULL: 오류
 */
list_elmt_pt list_elmt_create(void * data);

/**
 * 리스트 엘리먼트를 제거하는 함수
 *
 * @param	elmt		대상 리스트 엘리먼트 포인터
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int list_elmt_delete(list_elmt_pt elmt);

/** 리스트 자료 구조 */
typedef	struct _list_t {
	unsigned int	count;
	list_elmt_pt	head;
	list_elmt_pt	tail;
} list_t;

/**
 * @var typedef	struct _list_t list_t
 *
 * 리스트 형 정의
 */

/** 리스트 포인터 형 정의 */
typedef list_t * list_pt;

/**
 * 리스트를 초기화하는 매크로
 *
 * @param	list		대상 리스트 포인터
 */
#define list_init(list)                             	                    \
{                                                                           \
	(list)->count	= 0;                                                    \
	(list)->head	= NULL;                                                 \
	(list)->tail	= NULL;                                                 \
}

/**
 * 리스트를 생성하는 함수
 *
 * @param	list_p		생성한 리스트의 주소를 저장할 포인터의 주소
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int list_create(list_pt * list_p);

/**
 * 리스트를 제거하는 함수
 *
 * @param	list_p		제거할 리스트의 주소가 저장된 포인터의 주소<br>
 * 						제거에 성공하면 *list_p는 NULL이 됨
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int list_delete(list_pt * list_p);

/**
 * 엘리먼트를 지정된 엘리먼트 다음에 추가하는 함수
 *
 * @param	list		리스트 포인터
 *
 * @param	ref			이 포인터가 가리키는 엘리먼트 다음에 추가함<br>
 * 						NULL 이면 맨 앞에 추가함
 *
 * @param	elmt		추가할  엘리먼트 포인터
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int list_insertnext(list_pt list, list_elmt_pt ref, list_elmt_pt elmt);

/**
 * 엘리먼트를 맨 뒤에 추가하는 함수
 *
 * @param	list		리스트 포인터
 *
 * @param	elmt		추가할  엘리먼트 포인터
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int list_inserttail(list_pt list, list_elmt_pt elmt);

/**
 * 지정된 엘리먼트 다음 엘리먼트를 제거하는 함수
 *
 * @param	list		대상 리스트 포인터
 *
 * @param	ref			이 포인터가 가리키는 엘리먼트 다음 엘리먼트를 제거함<br>
 * 						NULL이면 맨 앞의 엘리먼트를 제거함
 *
 * @param	elmt_p		제거한  엘리먼트 포인터를 저장할 변수 주소 (NULL이면 무시)
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int list_removenext(list_pt list, list_elmt_pt ref, list_elmt_pt * elmt_p);

/**
 * 지정된 값을 가진 첫번째 엘리먼트를 찾는 함수
 *
 * @param	list		대상 리스트 포인터
 *
 * @param	elmt_p		찾은 엘리먼트 포인터를 저장할 변수 주소
 *
 * @param	data		이 값을 가진 첫번째 엘리먼트를 찾음
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			LIST_ERR__NOTEXIST		: data를 값으로 가지는 엘리먼트가 없음<br>
 */
int list_find(list_pt list, list_elmt_pt * elmt_p, void * data);

/**
 * 지정된 값을 가진 첫번째 엘리먼트를 찾아 제거하는 함수
 *
 * @param	list		대상 리스트 포인터
 *
 * @param	elmt_p		찾은 엘리먼트 포인터를 저장할 변수 주소 (NULL이면 무시)
 *
 * @param	data		이 값을 가진 첫번째 엘리먼트를 찾아 제거함
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			LIST_ERR__NOTEXIST		: data를 값으로 가지는 엘리먼트가 없음<br>
 */
int list_findandremove(list_pt list, list_elmt_pt * elmt_p, void * data);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBICLIB_LIST_H_ */
