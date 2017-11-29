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

#ifndef ITF_UBICLIB_HEAP_H_
#define ITF_UBICLIB_HEAP_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubiclib_heap.h
 *
 * @brief ubiclib (Ubinos C Library) 컴포넌트 힙 인터페이스
 *
 * ubiclib 컴포넌트가 제공하는 힙 인터페이스를 정의합니다.
 */

#include "type.h"

/** 데이터 상위 경계 영역이 오염되었음 */
#define HEAP_ERR__POLLUTED_TOP		-21

/** 데이터 하위 경계 영역이 오염되었음 */
#define HEAP_ERR__POLLUTED_BOTTOM	-22

/** 헤더 경계 영역이 오염되었음 */
#define HEAP_ERR__POLLUTED_HEADER	-23

/** 지원하지 않는 기능임 */
#define HEAP_ERR__UNSUPPORTED		-24

typedef	struct __heap_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _heap_tip_t;

/** 힙 포인터 형 정의 */
typedef	_heap_tip_t *	heap_pt;

/**
 * 힙 컴포넌트를 초기화하는 함수
 *
 * 초기화 작업들 중 재진입(reenterance)을 지원하기 위한 부분은 수행하지 않습니다.<br>
 * 재진입(reenterance)을 지원하려면 ubiclib_heap_comp_init_reent 함수를 추가로 호출해 주어야 합니다.<br>
 * 이 함수는 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.<br>
 *
 * @param	addr		메모리 block 주소
 *
 * @param	size		메모리 block 크기
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			HEAP_ERR__UNSUPPORTED: 지원하지 않음<br>
 */
int ubiclib_heap_comp_init(unsigned int addr, unsigned int size);

/**
 * 힙 컴포넌트 초기화 작업들 중 재진입(reenterance)을 지원하기 위한 부분을 수행하는 함수
 *
 * 이 함수는 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			HEAP_ERR__UNSUPPORTED: 지원하지 않음<br>
 */
int ubiclib_heap_comp_init_reent(void);

/**
 * 힙을 생성하는 함수
 *
 * @param	heap_p		생성한 힙의 주소를 저장할 포인터의 주소
 *
 * @param	addr		메모리 block 주소
 *
 * @param	size		메모리 block 크기
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_create(heap_pt * heap_p, unsigned int addr, unsigned int size);

/**
 * heap을 제거하는 함수
 *
 * @param	heap_p		제거할 힙의 주소가 저장된 포인터의 주소<br>
 * 						제거에 성공하면 *heap_p는 NULL이 됨
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_delete(heap_pt * heap_p);

/**
 * 메모리를 할당하는 함수
 *
 * @param	heap		대상 힙
 *
 * @param	size		할당할 메모리 크기
 *
 * @return	할당된 메모리 주소<br>
 * 			<br>
 * 			NULL: 오류
 */
void * heap_malloc(heap_pt heap, unsigned int size);

/**
 * 메모리를 할당하는 함수 (정방향으로 성장)
 *
 * @param	heap		대상 힙
 *
 * @param	size		할당할 메모리 크기
 *
 * @return	할당된 메모리 주소<br>
 * 			<br>
 * 			NULL: 오류
 */
void * heap_mallocn(heap_pt heap, unsigned int size);

/**
 * 메모리를 할당하는 함수 (역방향으로 성장)
 *
 * @param	heap		대상 힙
 *
 * @param	size		할당할 메모리 크기
 *
 * @return	할당된 메모리 주소<br>
 * 			<br>
 * 			NULL: 오류
 */
void * heap_mallocr(heap_pt heap, unsigned int size);

/**
 * 할당된 메모리를 해제하는 함수
 *
 * @param	heap		대상 힙
 *
 * @param	ptr			할당된 메모리 주소
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_free(heap_pt heap, void * ptr);

/**
 * 할당된 메모리의 경계 영역이 오염되었는지 여부를 검사하는 함수
 *
 * @param	ptr			할당된 메모리 주소
 *
 * @return	  0: 정상 상태<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			HEAP_ERR__POLLUTED_TOP		: top boundary 영역이 오염되었음<br>
 * 			HEAP_ERR__POLLUTED_BOTTOM	: bottom boundary 영역이 오염되었음<br>
 * 			HEAP_ERR__POLLUTED_HEADER	: header boundary 영역이 오염되었음<br>
 *
 * 메모리 블럭 구조<br>
 * <pre>
 * \#if (1 != EXCLUDE_HEAP_BOUNDARY_CHECK)
 *          -----------------------------------
 *          | header boundary                 |
 *          -----------------------------------
 *          | header                          |
 *          -----------------------------------
 *          | top boundary                    |
 *          -----------------------------------  <---- memory address
 *          | memory                          |
 *          |                                 |
 *          |                                 |
 *          -----------------------------------
 *          | bottom boundary                 |
 *          -----------------------------------
 * \#else
 *          -----------------------------------
 *          | header                          |
 *          -----------------------------------  <---- memory address
 *          | memory                          |
 *          |                                 |
 *          |                                 |
 *          -----------------------------------
 * \#endif
 * </pre>
 */
int heap_checkboundary(void * ptr);

/**
 * 할당된 모든 메모리의 경계 영역이 오염되었는지 여부를 검사하는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @return	  0: 정상 상태<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 *
 */
int heap_checkboundaryall(heap_pt heap);

/**
 * 할당된 메모리의 크기를 돌려주는 함수
 *
 * @param	ptr			할당된 메모리 주소
 *
 * @param	size		크기를 저장할 변수 주소
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getsize(void * ptr, unsigned int * size);

/**
 * 기본 heap에서 메모리를 할당하는 함수 (정방향 성장)
 *
 * @param	size		할당할 메모리 크기
 *
 * @return	할당된 메모리 주소<br>
 * 			<br>
 * 			NULL: 오류
 */
void * mallocn(size_t size);

/**
 * 기본 heap에서 메모리를 할당하는 함수 (역방향 성장)
 *
 * @param	size		할당할 메모리 크기
 *
 * @return	할당된 메모리 주소<br>
 * 			<br>
 * 			NULL: 오류
 */
void * mallocr(size_t size);

/**
 * heap 크기를 돌려주는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @param	size_p		heap 크기를 저장할 변수 주소
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getheapsize(heap_pt heap, unsigned int * size_p);

/**
 * heap의 할당된 메모리 총량을 돌려주는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @param	size_p		heap의 할당된 메모리 총량을 저장할 변수 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocsize(heap_pt heap, unsigned int * size_p);

/**
 * heap의 할당된 메모리 총량을 돌려주는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @param	nsize_p		정방향으로 할당된 메모리 총량을 저장할 변수 주소<br>
 * 						NULL이면 무시함<br>
 *
 * @param	rsize_p		역방향으로 할당된 메모리 총량을 저장할 변수 주소<br>
 * 						NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocsize_ext(heap_pt heap, unsigned int * nsize_p, unsigned int * rsize_p);

/**
 * heap의 할당된 메모리 총량 최고 기록을 돌려주는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @param	size_p		heap의 할당된 메모리 총량 최고 기록을 저장할 변수 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocsizemax(heap_pt heap, unsigned int * size_p);

/**
 * heap의 할당된 메모리 총량 최고 기록을 돌려주는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @param	nsize_p		정방향으로 할당된 메모리 총량 최고 기록을 저장할 변수 주소<br>
 * 						NULL이면 무시함<br>
 *
 * @param	rsize_p		역방향으로 할당된 메모리 총량 최고 기록을 저장할 변수 주소<br>
 * 						NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getallocsizemax_ext(heap_pt heap, unsigned int * nsize_p, unsigned int * rsize_p);

/**
 * heap의 할당된 메모리 블럭 총수를 돌려주는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @param	count_p		heap의 할당된 메모리 블럭 총수를 저장할 변수 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getalloccount(heap_pt heap, unsigned int * count_p);

/**
 * heap의 할당된 메모리 블럭 총수를 돌려주는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @param	ncount_p	정방향으로 할당된 메모리 블럭 총수를 저장할 변수 주소<br>
 * 						NULL이면 무시함<br>
 *
 * @param	rcount_p	역방향으로 할당된 메모리 블럭 총수를 저장할 변수 주소<br>
 * 						NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getalloccount_ext(heap_pt heap, unsigned int * ncount_p, unsigned int * rcount_p);

/**
 * heap의 할당된 메모리 블럭 총수 최고 기록을 돌려주는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @param	count_p		heap의 할당된 메모리 블럭 총수 최고 기록을 저장할 변수 주소<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getalloccountmax(heap_pt heap, unsigned int * count_p);

/**
 * heap의 할당된 메모리 블럭 총수 최고 기록을 돌려주는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @param	ncount_p	정방향으로 할당된 메모리 블럭 총수 최고 기록을 저장할 변수 주소<br>
 * 						NULL이면 무시함<br>
 *
 * @param	rcount_p	역방향으로 할당된 메모리 블럭 총수 최고 기록을 저장할 변수 주소<br>
 * 						NULL이면 무시함<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getalloccountmax_ext(heap_pt heap, unsigned int * ncount_p, unsigned int * rcount_p);

/**
 * heap의 메모리 블럭 당 오버헤드를 돌려주는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @param	overhead_p	heap의 메모리 블럭 당 오버헤드를 저장할 변수 주소
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_getblockoverhead(heap_pt heap, unsigned int * overhead_p);

/**
 * 힙 정보를 문자열 형태로 메모리 버퍼에 기록하는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @param	buf			기록할 메모리  버퍼
 *
 * @param	max			메모리 버퍼의 크기
 *
 * @return	기록된 문자열 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_sprintheapinfo(heap_pt heap, char * buf, int max);

/**
 * 힙 정보를 출력하는 함수
 *
 * @param	heap		대상 힙<br>
 * 						NULL이면 기본 heap<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int heap_printheapinfo(heap_pt heap);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBICLIB_HEAP_H_ */
