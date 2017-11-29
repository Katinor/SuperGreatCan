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

#ifndef ITF_UBICLIB_DLIST_H_
#define ITF_UBICLIB_DLIST_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file ubiclib_dlist.h
 *
 * @brief ubiclib (Ubinos C Library) ������Ʈ ���� ��ũ�� ����Ʈ �������̽�
 *
 * ubiclib ������Ʈ��  �����ϴ� ���� ��ũ�� ����Ʈ �������̽��� �����մϴ�.
 */

#include "type.h"

/** ����: �ش� ������Ʈ�� ���� */
#define DLIST_ERR__NOTEXIST		-21

/** ���� ��ũ�� ����Ʈ ������Ʈ �ڷ� ���� */
typedef	struct _dlist_elmt_t {
	struct _dlist_elmt_t *	prev;
	struct _dlist_elmt_t *	next;
	void *					list;
	void * 					data;
	void * 					data2;
} dlist_elmt_t;

/**
 * @var	struct _dlist_elmt_t dlist_elmt_t
 *
 * ���� ��ũ�� ����Ʈ ������Ʈ �� ����
 */

/** ���� ��ũ�� ����Ʈ ������Ʈ ������ �� ���� */
typedef dlist_elmt_t * dlist_elmt_pt;

/**
 * ���� ��ũ�� ����Ʈ ������Ʈ�� �����ϴ� �Լ�
 *
 * @param	data		 ������Ʈ�� ������
 *
 * @param	data2		 ������Ʈ�� ������2
 *
 * @return	������ ���� ��ũ�� ����Ʈ ������Ʈ ������<br>
 * 			<br>
 *          NULL: ����
 */
dlist_elmt_pt dlist_elmt_create(void * data, void * data2);

/**
 * ���� ��ũ�� ����Ʈ ������Ʈ�� �����ϴ� �Լ�
 *
 * @param	elmt		��� ���� ��ũ�� ����Ʈ ������Ʈ ������
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int dlist_elmt_delete(dlist_elmt_pt elmt);

/** ���� ��ũ�� ����Ʈ �ڷ� ���� */
typedef	struct _dlist_t {
	unsigned int	count;
	dlist_elmt_pt	head;
	dlist_elmt_pt	tail;
	dlist_elmt_pt	cur;
	void *			data;
} dlist_t;

/**
 * @var typedef	struct _dlist_t dlist_t
 *
 * ���� ��ũ�� ����Ʈ �� ����
 */

/** ���� ��ũ�� ����Ʈ ������ �� ���� */
typedef dlist_t * dlist_pt;

/**
 * ���� ��ũ�� ����Ʈ�� �ʱ�ȭ�ϴ� ��ũ��
 *
 * @param	dlist		��� ���� ��ũ�� ����Ʈ ������
 */
#define dlist_init(dlist)                             	                    \
{                                                                           \
	(dlist)->count	= 0;                                                    \
	(dlist)->head	= NULL;                                                 \
	(dlist)->tail	= NULL;                                                 \
	(dlist)->cur	= NULL;                                                 \
	(dlist)->data	= NULL;													\
}

/**
 * ���� ��ũ�� ����Ʈ�� �����ϴ� �Լ�
 *
 * @param	dlist_p		������ ���� ��ũ�� ����Ʈ�� �ּҸ� ������ �������� �ּ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int dlist_create(dlist_pt * dlist_p);

/**
 * ���� ��ũ�� ����Ʈ�� �����ϴ� �Լ�
 *
 * @param	dlist_p		������ ���� ��ũ�� ����Ʈ�� �ּҰ� ����� �������� �ּ�<br>
 * 						���ſ� �����ϸ� *dlist_p�� NULL�� ��
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int dlist_delete(dlist_pt * dlist_p);

/**
 * ������Ʈ�� ������ ������Ʈ �տ� �߰��ϴ� �Լ�
 *
 * @param	dlist		��� ���� ��ũ�� ����Ʈ ������
 *
 * @param	ref			�� �����Ͱ� ����Ű�� ������Ʈ �տ� �߰���<br>
 * 						NULL �̸� �� �ڿ� �߰���
 *
 * @param	elmt		�߰���  ������Ʈ
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int dlist_insertprev(dlist_pt dlist, dlist_elmt_pt ref, dlist_elmt_pt elmt);

/**
 * ������Ʈ�� ������ ������Ʈ ������ �߰��ϴ� �Լ�
 *
 * @param	dlist		��� ���� ��ũ�� ����Ʈ ������
 *
 * @param	ref			�� �����Ͱ� ����Ű�� ������Ʈ ������ �߰���<br>
 * 						NULL �̸� �� �տ� �߰���
 *
 * @param	elmt		�߰���  ������Ʈ
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int dlist_insertnext(dlist_pt dlist, dlist_elmt_pt ref, dlist_elmt_pt elmt);

/**
 * ������Ʈ�� �Ҽӵ� ���� ��ũ�� ����Ʈ���� �����ϴ� �Լ�
 *
 * @param	elmt		������ ������Ʈ
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int dlist_remove(dlist_elmt_pt elmt);


/**
 * ���� ������Ʈ ������(cur)�� �� ���� ������Ʈ �����ͷ� �����ϰ� ����� ���� ������Ʈ �����͸� �����ִ� �Լ�<br>
 * ���� ������Ʈ �����Ͱ� NULL�̰ų� ������ ������Ʈ�� ����Ű�� ���� ������Ʈ �����͸� ù��° ������Ʈ �����ͷ� ����
 *
 * @param	dlist		��� ���� ��ũ�� ����Ʈ ������
 *
 * @return	����� ���� ������Ʈ ������<br>
 * 			<br>
 * 			NULL: ��� ���� ��ũ�� ����Ʈ�� ��� ����
 */
dlist_elmt_pt dlist_getcurnext(dlist_pt dlist);

/**
 * ������ ���� ���� ù��° ������Ʈ�� ã�� �Լ�
 *
 * @param	dlist		��� ���� ��ũ�� ����Ʈ ������
 *
 * @param	elmt_p		ã�� ������Ʈ �����͸� ������ ���� �ּ�
 *
 * @param	data		�� ���� ���� ù��° ������Ʈ�� ã��
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 *          DLIST_ERR__NOTEXIST		: data�� ������ ������ ������Ʈ�� ����<br>
 */
int dlist_find(dlist_pt dlist, dlist_elmt_pt * elmt_p, void * data);

/**
 * ������ ���� ���� ù��° ������Ʈ�� ã�� �����ϴ� �Լ�
 *
 * @param	dlist		��� ���� ��ũ�� ����Ʈ ������
 *
 * @param	elmt_p		ã�� ������Ʈ �����͸� ������ ���� �ּ� (NULL�̸� ����)
 *
 * @param	data		�� ���� ���� ù��° ������Ʈ�� ã�� ������
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 *          DLIST_ERR__NOTEXIST		: data�� ������ ������ ������Ʈ�� ����<br>
 */
int dlist_findandremove(dlist_pt dlist, dlist_elmt_pt * elmt_p, void * data);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBICLIB_DLIST_H_ */
