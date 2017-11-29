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
 * @brief ubiclib (Ubinos C Library) ������Ʈ �� �������̽�
 *
 * ubiclib ������Ʈ�� �����ϴ� �� �������̽��� �����մϴ�.
 */

#include "type.h"

/** ������ ���� ��� ������ �����Ǿ��� */
#define HEAP_ERR__POLLUTED_TOP		-21

/** ������ ���� ��� ������ �����Ǿ��� */
#define HEAP_ERR__POLLUTED_BOTTOM	-22

/** ��� ��� ������ �����Ǿ��� */
#define HEAP_ERR__POLLUTED_HEADER	-23

/** �������� �ʴ� ����� */
#define HEAP_ERR__UNSUPPORTED		-24

typedef	struct __heap_tip_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} _heap_tip_t;

/** �� ������ �� ���� */
typedef	_heap_tip_t *	heap_pt;

/**
 * �� ������Ʈ�� �ʱ�ȭ�ϴ� �Լ�
 *
 * �ʱ�ȭ �۾��� �� ������(reenterance)�� �����ϱ� ���� �κ��� �������� �ʽ��ϴ�.<br>
 * ������(reenterance)�� �����Ϸ��� ubiclib_heap_comp_init_reent �Լ��� �߰��� ȣ���� �־�� �մϴ�.<br>
 * �� �Լ��� �ý��� ���۽�  bsp ������Ʈ�� main �Լ����� �ڵ������� ȣ��˴ϴ�.<br>
 *
 * @param	addr		�޸� block �ּ�
 *
 * @param	size		�޸� block ũ��
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			HEAP_ERR__UNSUPPORTED: �������� ����<br>
 */
int ubiclib_heap_comp_init(unsigned int addr, unsigned int size);

/**
 * �� ������Ʈ �ʱ�ȭ �۾��� �� ������(reenterance)�� �����ϱ� ���� �κ��� �����ϴ� �Լ�
 *
 * �� �Լ��� �ý��� ���۽�  bsp ������Ʈ�� main �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			HEAP_ERR__UNSUPPORTED: �������� ����<br>
 */
int ubiclib_heap_comp_init_reent(void);

/**
 * ���� �����ϴ� �Լ�
 *
 * @param	heap_p		������ ���� �ּҸ� ������ �������� �ּ�
 *
 * @param	addr		�޸� block �ּ�
 *
 * @param	size		�޸� block ũ��
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_create(heap_pt * heap_p, unsigned int addr, unsigned int size);

/**
 * heap�� �����ϴ� �Լ�
 *
 * @param	heap_p		������ ���� �ּҰ� ����� �������� �ּ�<br>
 * 						���ſ� �����ϸ� *heap_p�� NULL�� ��
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_delete(heap_pt * heap_p);

/**
 * �޸𸮸� �Ҵ��ϴ� �Լ�
 *
 * @param	heap		��� ��
 *
 * @param	size		�Ҵ��� �޸� ũ��
 *
 * @return	�Ҵ�� �޸� �ּ�<br>
 * 			<br>
 * 			NULL: ����
 */
void * heap_malloc(heap_pt heap, unsigned int size);

/**
 * �޸𸮸� �Ҵ��ϴ� �Լ� (���������� ����)
 *
 * @param	heap		��� ��
 *
 * @param	size		�Ҵ��� �޸� ũ��
 *
 * @return	�Ҵ�� �޸� �ּ�<br>
 * 			<br>
 * 			NULL: ����
 */
void * heap_mallocn(heap_pt heap, unsigned int size);

/**
 * �޸𸮸� �Ҵ��ϴ� �Լ� (���������� ����)
 *
 * @param	heap		��� ��
 *
 * @param	size		�Ҵ��� �޸� ũ��
 *
 * @return	�Ҵ�� �޸� �ּ�<br>
 * 			<br>
 * 			NULL: ����
 */
void * heap_mallocr(heap_pt heap, unsigned int size);

/**
 * �Ҵ�� �޸𸮸� �����ϴ� �Լ�
 *
 * @param	heap		��� ��
 *
 * @param	ptr			�Ҵ�� �޸� �ּ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_free(heap_pt heap, void * ptr);

/**
 * �Ҵ�� �޸��� ��� ������ �����Ǿ����� ���θ� �˻��ϴ� �Լ�
 *
 * @param	ptr			�Ҵ�� �޸� �ּ�
 *
 * @return	  0: ���� ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			HEAP_ERR__POLLUTED_TOP		: top boundary ������ �����Ǿ���<br>
 * 			HEAP_ERR__POLLUTED_BOTTOM	: bottom boundary ������ �����Ǿ���<br>
 * 			HEAP_ERR__POLLUTED_HEADER	: header boundary ������ �����Ǿ���<br>
 *
 * �޸� �� ����<br>
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
 * �Ҵ�� ��� �޸��� ��� ������ �����Ǿ����� ���θ� �˻��ϴ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @return	  0: ���� ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 *
 */
int heap_checkboundaryall(heap_pt heap);

/**
 * �Ҵ�� �޸��� ũ�⸦ �����ִ� �Լ�
 *
 * @param	ptr			�Ҵ�� �޸� �ּ�
 *
 * @param	size		ũ�⸦ ������ ���� �ּ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_getsize(void * ptr, unsigned int * size);

/**
 * �⺻ heap���� �޸𸮸� �Ҵ��ϴ� �Լ� (������ ����)
 *
 * @param	size		�Ҵ��� �޸� ũ��
 *
 * @return	�Ҵ�� �޸� �ּ�<br>
 * 			<br>
 * 			NULL: ����
 */
void * mallocn(size_t size);

/**
 * �⺻ heap���� �޸𸮸� �Ҵ��ϴ� �Լ� (������ ����)
 *
 * @param	size		�Ҵ��� �޸� ũ��
 *
 * @return	�Ҵ�� �޸� �ּ�<br>
 * 			<br>
 * 			NULL: ����
 */
void * mallocr(size_t size);

/**
 * heap ũ�⸦ �����ִ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @param	size_p		heap ũ�⸦ ������ ���� �ּ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_getheapsize(heap_pt heap, unsigned int * size_p);

/**
 * heap�� �Ҵ�� �޸� �ѷ��� �����ִ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @param	size_p		heap�� �Ҵ�� �޸� �ѷ��� ������ ���� �ּ�<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_getallocsize(heap_pt heap, unsigned int * size_p);

/**
 * heap�� �Ҵ�� �޸� �ѷ��� �����ִ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @param	nsize_p		���������� �Ҵ�� �޸� �ѷ��� ������ ���� �ּ�<br>
 * 						NULL�̸� ������<br>
 *
 * @param	rsize_p		���������� �Ҵ�� �޸� �ѷ��� ������ ���� �ּ�<br>
 * 						NULL�̸� ������<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_getallocsize_ext(heap_pt heap, unsigned int * nsize_p, unsigned int * rsize_p);

/**
 * heap�� �Ҵ�� �޸� �ѷ� �ְ� ����� �����ִ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @param	size_p		heap�� �Ҵ�� �޸� �ѷ� �ְ� ����� ������ ���� �ּ�<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_getallocsizemax(heap_pt heap, unsigned int * size_p);

/**
 * heap�� �Ҵ�� �޸� �ѷ� �ְ� ����� �����ִ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @param	nsize_p		���������� �Ҵ�� �޸� �ѷ� �ְ� ����� ������ ���� �ּ�<br>
 * 						NULL�̸� ������<br>
 *
 * @param	rsize_p		���������� �Ҵ�� �޸� �ѷ� �ְ� ����� ������ ���� �ּ�<br>
 * 						NULL�̸� ������<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_getallocsizemax_ext(heap_pt heap, unsigned int * nsize_p, unsigned int * rsize_p);

/**
 * heap�� �Ҵ�� �޸� �� �Ѽ��� �����ִ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @param	count_p		heap�� �Ҵ�� �޸� �� �Ѽ��� ������ ���� �ּ�<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_getalloccount(heap_pt heap, unsigned int * count_p);

/**
 * heap�� �Ҵ�� �޸� �� �Ѽ��� �����ִ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @param	ncount_p	���������� �Ҵ�� �޸� �� �Ѽ��� ������ ���� �ּ�<br>
 * 						NULL�̸� ������<br>
 *
 * @param	rcount_p	���������� �Ҵ�� �޸� �� �Ѽ��� ������ ���� �ּ�<br>
 * 						NULL�̸� ������<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_getalloccount_ext(heap_pt heap, unsigned int * ncount_p, unsigned int * rcount_p);

/**
 * heap�� �Ҵ�� �޸� �� �Ѽ� �ְ� ����� �����ִ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @param	count_p		heap�� �Ҵ�� �޸� �� �Ѽ� �ְ� ����� ������ ���� �ּ�<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_getalloccountmax(heap_pt heap, unsigned int * count_p);

/**
 * heap�� �Ҵ�� �޸� �� �Ѽ� �ְ� ����� �����ִ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @param	ncount_p	���������� �Ҵ�� �޸� �� �Ѽ� �ְ� ����� ������ ���� �ּ�<br>
 * 						NULL�̸� ������<br>
 *
 * @param	rcount_p	���������� �Ҵ�� �޸� �� �Ѽ� �ְ� ����� ������ ���� �ּ�<br>
 * 						NULL�̸� ������<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_getalloccountmax_ext(heap_pt heap, unsigned int * ncount_p, unsigned int * rcount_p);

/**
 * heap�� �޸� �� �� ������带 �����ִ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @param	overhead_p	heap�� �޸� �� �� ������带 ������ ���� �ּ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_getblockoverhead(heap_pt heap, unsigned int * overhead_p);

/**
 * �� ������ ���ڿ� ���·� �޸� ���ۿ� ����ϴ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @param	buf			����� �޸�  ����
 *
 * @param	max			�޸� ������ ũ��
 *
 * @return	��ϵ� ���ڿ� ũ��<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_sprintheapinfo(heap_pt heap, char * buf, int max);

/**
 * �� ������ ����ϴ� �Լ�
 *
 * @param	heap		��� ��<br>
 * 						NULL�̸� �⺻ heap<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int heap_printheapinfo(heap_pt heap);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBICLIB_HEAP_H_ */
