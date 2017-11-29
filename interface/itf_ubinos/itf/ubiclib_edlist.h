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

#ifndef ITF_UBICLIB_EDLIST_H_
#define ITF_UBICLIB_EDLIST_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubiclib_edlist.h
 *
 * @brief ubiclib (Ubinos C Library) 컴포넌트 내장형 더블 링크드 리스트 인터페이스
 */

#include "type.h"

/** 내장형 더블 링크드 리스트 자료 구조 */
typedef struct _edlist_t {
	unsigned int	count;
	void *			head;
	void *			tail;
	void *			cur;
	void *			data;
} edlist_t;

/**
 * @var typedef	struct _edlist_t edlist_t
 *
 * 내장형 더블 링크드 리스트 형 정의
 */

/** 내장형 더블 링크드 리스트 포인터 형 정의 */
typedef edlist_t * edlist_pt;

/** 내장형 더블 링크드 리스트 엘리먼트 자료 구조 예 */
typedef struct _edlist_elmt_t {
	struct {
		struct _edlist_elmt_t *	prev;
		struct _edlist_elmt_t *	next;
		edlist_t *				list;
	}					link;

	void *				data;
} edlist_elmt_t;

/**
 * @var typedef	struct _edlist_elmt_t edlist_elmt_t
 *
 * 내장형 더블 링크드 리스트 엘리먼트  형 정의 예
 */

/** 내장형 더블 링크드 리스트 엘리먼트 포인터 형 정의 예 */
typedef edlist_elmt_t * edlist_elmt_pt;

/**
 * 내장형 더블 링크드 리스트를 초기화하는 매크로
 *
 * @param	edlist		대상 리스트 포인터
 */
#define edlist_init(edlist)                                                 \
{                                                                           \
	(edlist)->count	= 0;                                                    \
	(edlist)->head	= NULL;                                                 \
	(edlist)->tail	= NULL;                                                 \
	(edlist)->cur	= NULL;                                                 \
	(edlist)->data	= NULL;													\
}

/**
 * 내장형 더블 링크드 리스트 링크를 초기화하는 매크로
 *
 * @param	link		대상 링크 포인터
 */
#define edlist_link_init(link)                                              \
{                                                                           \
	(link)->prev	= NULL;                                                 \
	(link)->next	= NULL;                                                 \
	(link)->list	= NULL;                                                 \
}

/**
 * 지정한 엘리먼트 앞에 엘리먼트를 추가하는 매크로
 *
 * @param	elmttype	엘리먼트 포인터 형
 *
 * @param	linkname	링크 이름
 *
 * @param	edlist		대상 리스트 포인터
 *
 * @param	ref			이 포인터가 가리키는 엘리먼트 앞에 추가함,<br>
 * 						NULL이면 맨 뒤에 추가함
 *
 * @param	elmt		추가할 엘리먼트
 */
#define edlist_insertprev(elmttype, linkname, edlist, ref, elmt)	        \
{                                                                           \
	(elmt)->linkname.next = ((elmttype) ref);                               \
                                                                            \
	if (NULL == ((elmttype) ref)) {                                         \
		(elmt)->linkname.prev = (edlist)->tail;                             \
		(edlist)->tail = (elmt);                                            \
	}                                                                       \
	else {                                                                  \
		(elmt)->linkname.prev = ((elmttype) ref)->linkname.prev;            \
		((elmttype) ref)->linkname.prev = (elmt);                           \
	}                                                                       \
                                                                            \
	if (NULL == (elmt)->linkname.prev) {                                    \
		(edlist)->head = (elmt);                                            \
	}                                                                       \
	else {                                                                  \
		(elmt)->linkname.prev->linkname.next = (elmt);                      \
	}                                                                       \
                                                                            \
	(edlist)->count++;                                                      \
                                                                            \
	(elmt)->linkname.list = (edlist);                                       \
}

/**
 * 지정한 엘리먼트 다음에 엘리먼트를 추가하는 매크로
 *
 * @param	elmttype	엘리먼트 포인터 형
 *
 * @param	linkname	링크 이름
 *
 * @param	edlist		대상 리스트 포인터
 *
 * @param	ref			이 포인터가 가리키는 엘리먼트 다음에 추가함,<br>
 * 						NULL이면 맨 앞에 추가함
 *
 * @param	elmt		추가할 엘리먼트
 */
#define edlist_insertnext(elmttype, linkname, edlist, ref, elmt)            \
{                                                                           \
	(elmt)->linkname.prev = ((elmttype) ref);                               \
                                                                            \
	if (NULL == ((elmttype) ref)) {                                         \
		(elmt)->linkname.next = (edlist)->head;                             \
		(edlist)->head = (elmt);                                            \
	}                                                                       \
	else {                                                                  \
		(elmt)->linkname.next = ((elmttype) ref)->linkname.next;            \
		((elmttype) ref)->linkname.next = (elmt);                           \
	}                                                                       \
                                                                            \
	if (NULL == (elmt)->linkname.next) {                                    \
		(edlist)->tail = (elmt);                                            \
	}                                                                       \
	else {                                                                  \
		(elmt)->linkname.next->linkname.prev = (elmt);                      \
	}                                                                       \
                                                                            \
	(edlist)->count++;                                                      \
                                                                            \
	(elmt)->linkname.list = (edlist);                                       \
}

/**
 * 엘리먼트를 소속된 리스트에서 제거하는 매크로
 *
 * @param	elmttype	엘리먼트 포인터 형
 *
 * @param	linkname	링크 이름
 *
 * @param	elmt		제거할 엘리먼트
 */
#define edlist_remove(elmttype, linkname, elmt)                             \
{                                                                           \
	if (NULL != ((elmt)->linkname.list)) {                                  \
		if ((elmt) == ((elmt)->linkname.list)->cur) {                       \
			((elmt)->linkname.list)->cur = (elmt)->linkname.prev;           \
		}                                                                   \
	                                                                        \
		if (NULL == (elmt)->linkname.prev) {                                \
			((elmt)->linkname.list)->head = (elmt)->linkname.next;          \
		}                                                                   \
		else {                                                              \
			(elmt)->linkname.prev->linkname.next = (elmt)->linkname.next;   \
		}                                                                   \
	                                                                        \
		if (NULL == (elmt)->linkname.next) {                                \
			((elmt)->linkname.list)->tail = (elmt)->linkname.prev;          \
		}                                                                   \
		else {                                                              \
			(elmt)->linkname.next->linkname.prev = (elmt)->linkname.prev;   \
		}                                                                   \
	                                                                        \
		((elmt)->linkname.list)->count--;                                   \
	                                                                        \
		(elmt)->linkname.prev = NULL;                                       \
		(elmt)->linkname.next = NULL;                                       \
		(elmt)->linkname.list = NULL;                                       \
	}                                                                       \
}

void * _edlist_setcur(edlist_pt edlist, void * cur);

/**
 * 현재 엘리먼트를 설정하는 매크로
 *
 * @param	elmttype	엘리먼트 포인터 형
 *
 * @param	linkname	링크 이름
 *
 * @param	edlist		대상 리스트 포인터
 *
 * @param	cur			현재 엘리먼트로 설정할 엘리먼트 포인터
 *
 * @return	현재 엘리먼트<br>
 * 			<br>
 * 			NULL: 오류
 */
#define edlist_setcur(elmttype, linkname, edlist, cur)	((elmttype) _edlist_setcur(edlist, cur))

void * _edlist_getcur(edlist_pt edlist);

/**
 * 현재 엘리먼트를 돌려주는 매크로
 *
 * @param	elmttype	엘리먼트 포인터 형
 *
 * @param	linkname	링크 이름
 *
 * @param	edlist		대상 리스트 포인터
 *
 * @return	현재 엘리먼트<br>
 * 			<br>
 * 			NULL: 대상 리스트가 비어 있음
 */
#define edlist_getcur(elmttype, linkname, edlist)		((elmttype) _edlist_getcur(edlist))

/**
 * 현재 엘리먼트 포인터(cur)를 그 다음 엘리먼트 포인터로 변경하고 변경된 현재 엘리먼트 포인터를 돌려주는 매크로,<br>
 * 현재 엘리먼트 포인터가 NULL이거나 마지막 엘리먼트를 가리키면 현재 엘리먼트 포인터를 첫번째 엘리먼트 포인터로 변경
 *
 * @param	elmttype	엘리먼트 포인터 형
 *
 * @param	linkname	링크 이름
 *
 * @param	edlist		대상 리스트 포인터
 *
 * @return	변경된 현재 엘리먼트 포인터<br>
 * 			<br>
 * 			NULL: 대상 리스트가 비어 있음
 */
#define edlist_getcurnext(elmttype, linkname, edlist)                                       \
	(   (elmttype)                                                                          \
	    ((NULL == (edlist)->cur || NULL == ((elmttype) (edlist)->cur)->linkname.next)   ?   \
		_edlist_setcur((edlist), (edlist)->head)                                        :   \
		_edlist_setcur((edlist), ((elmttype) (edlist)->cur)->linkname.next)              ) )

/**
 * 첫 엘리먼트를 지정하는 매크로
 *
 * @param	elmttype	엘리먼트 포인터 형
 *
 * @param	linkname	링크 이름
 *
 * @param	edlist		대상 리스트 포인터
 */
#define edlist_head(elmttype, linkname, edlist)		((elmttype) (edlist)->head)

/**
 * 마지막 엘리먼트를 지정하는 매크로
 *
 * @param	elmttype	엘리먼트 포인터 형
 *
 * @param	linkname	링크 이름
 *
 * @param	edlist		대상 리스트 포인터
 */
#define edlist_tail(elmttype, linkname, edlist)		((elmttype) (edlist)->tail)

/**
 * 지정한 엘리먼트 다음 엘리먼트를 지정하는 매크로
 *
 * @param	elmttype	엘리먼트 포인터 형
 *
 * @param	linkname	링크 이름
 *
 * @param	elmt		이 포인터가 가리키는 엘리먼트 다음 엘리먼트 포인터를 지정
 */
#define edlist_next(elmttype, linkname, elmt)		((elmt)->linkname.next)

/**
 * 지정한 엘리먼트 이전 엘리먼트를 지정하는 매크로
 *
 * @param	elmttype	엘리먼트 포인터 형
 *
 * @param	linkname	링크 이름
 *
 * @param	elmt		이 포인터가 가리키는 엘리먼트 이전 엘리먼트 포인터를 지정
 */
#define edlist_prev(elmttype, linkname, elmt)		((elmt)->linkname.prev)

/**
 * 지정한 엘리먼트가 소속된 내장형 더블 링크드 리스트를 지정하는 매크로
 *
 * @param	elmttype	엘리먼트 포인터 형
 *
 * @param	linkname	링크 이름
 *
 * @param	elmt		이 포인터가 가리키는 엘리먼트가 소속된 내장형 더블 링크드 리스트 포인터를 지정
 */
#define edlist_list(elmttype, linkname, elmt)		((elmt)->linkname.list)

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBICLIB_EDLIST_H_ */
