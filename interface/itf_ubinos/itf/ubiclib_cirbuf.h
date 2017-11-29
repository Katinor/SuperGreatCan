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
 * @brief ubiclib (Ubinos C Library) ������Ʈ ȯ������ �������̽�
 *
 * ubiclib ������Ʈ��  �����ϴ� ȯ������ �������̽��� �����մϴ�.
 */

#include "type.h"

/** ���� ����: ���۰� �������� �� �����͸� ���� */
#define CIRBUF_OPT__NOOVERWRITE		0x0001

/** ���� ����: ���� �½�ũ ȯ�濡�� ����� �� ���� (�����ʹ� �սǵ� �� ����) */
#define CIRBUF_OPT__MTPROTECTION	0x0002

/** ȯ������ �ڷ� ���� */
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
 * ��ü �� ����
 */

/** ȯ������ ������ �� ���� */
typedef cirbuf_t * cirbuf_pt;

/**
 * ȯ�����۸� �ʱ�ȭ�ϴ� ��ũ��
 *
 * @param	cirbuf		��� ȯ������ ������
 *
 * @param	maxsize		���� �ִ� ũ��
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
 * ȯ�����۸� �����ϴ� �Լ�
 *
 * @param	cirbuf_p	������ ȯ�������� �ּҸ� ������ �������� �ּ�
 *
 * @param	maxsize		���� �ִ� ũ��
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int cirbuf_create(cirbuf_pt * cirbuf_p, unsigned int maxsize);

/**
 * ȯ�����۸� �����ϴ� �Լ� (Ȯ����)
 *
 * @param	cirbuf_p	������ ȯ�������� �ּҸ� ������ �������� �ּ�
 *
 * @param	maxsize		���� �ִ� ũ��
 *
 * @param	option		�ɼ� (CIRBUF_OPT__...)<br>
 * 						0: �⺻ ����<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int cirbuf_create_ext(cirbuf_pt * cirbuf_p, unsigned int maxsize, unsigned int option);

/**
 * ȯ�����۸� �����ϴ� �Լ�
 *
 * @param	cirbuf_p	������ ȯ�������� �ּҰ� ����� �������� �ּ�<br>
 * 						���ſ� �����ϸ� *cirbuf_p�� NULL�� ��
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int cirbuf_delete(cirbuf_pt * cirbuf_p);

/**
 * ȯ�����ۿ� �����͸� ���� �Լ�
 *
 * @param	cirbuf		��� ȯ������ ������
 *
 * @param	buf			�� �����Ͱ� ����Ǿ� �ִ� ����
 *
 * @param	size		�� ������ ũ��
 *
 * @param	written_p	�� ������ ũ�⸦ ������ ������ �ּ� (NULL�̸� ����)
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int cirbuf_write(cirbuf_pt cirbuf, unsigned char * buf, unsigned int size, unsigned int * written_p);

/**
 * ȯ�����ۿ��� �����͸� �д� �Լ�
 *
 * @param	cirbuf		��� ȯ������ ������
 *
 * @param	buf			���� �����͸� ������ ����
 *
 * @param	size		���� ������ ũ��
 *
 * @param	read_p		���� ������ ũ�⸦ ������ ������ �ּ� (NULL�̸� ����)
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int cirbuf_read(cirbuf_pt cirbuf, unsigned char * buf, unsigned int size, unsigned int * read_p);

/**
 * ȯ�������� �����͸� ��� ����� �Լ�
 *
 * @param	cirbuf		��� ȯ������ ������
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int cirbuf_clear(cirbuf_pt cirbuf);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_UBICLIB_CIRBUF_H_ */
