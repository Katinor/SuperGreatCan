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
 * @brief	ubiclib (Ubinos C Library) ������Ʈ �α� �޼��� �������̽�
 *
 * ubiclib ������Ʈ��  �����ϴ� �α� �޼��� �������̽��� �����մϴ�.
 */

#include "type.h"

/** ��� ī�װ� ���� */
#define LOGM_CATEGORY__ALL			0
#define LOGM_CATEGORY__DEFAULT		1
#define LOGM_CATEGORY__HEAP			2
#define LOGM_CATEGORY__USER00		3
#define LOGM_CATEGORY__USER01		4
#define LOGM_CATEGORY__USER02		5
#define LOGM_CATEGORY__END			6

/** � �޽����� ������� ���� */
#define LOGM_LEVEL__NONE			0
/** �׻� ��ϵǾ�� �ϴ� �޽��� */
#define LOGM_LEVEL__ALWAYS			1
/** �ɰ��� ������ ������ ���� �޽��� */
#define LOGM_LEVEL__FATAL			2
/** �Ϲ����� ������ ���� �޽��� */
#define LOGM_LEVEL__ERROR			3
/** ��� �޽��� */
#define LOGM_LEVEL__WARNING			4
/** ���� ���� ���� */
#define LOGM_LEVEL__INFO			5
/** ������ �ʿ��� ��Ÿ ���� */
#define LOGM_LEVEL__DEBUG			6
#define LOGM_LEVEL__END				7

#define LOGM_CATEGORY 				LOGM_CATEGORY__DEFAULT
#define LOGM_LEVEL	 				LOGM_LEVEL__WARNING
#define LOGM_TAG					NULL

#if (UBI_BUILDMODE__DEBUG == UBI_BUILDMODE)

/**
 * ����� �޽��� ������ �����ϴ� �Լ�
 *
 * @param	category �޽��� �з� ��ȣ (0�� ��� �з�)
 *
 * @param	level    ����� �޽��� ���� (������ ���� �̻�(���ų� ���� ��)�� �޽����� ��ϵ�)
 *
 * @return	������ ����� �޽��� ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int logm_setlevel(int category, int level);

/**
 * �����Ǿ� �ִ� ����� �޽��� ������ �����ִ� �Լ�
 *
 * @param	category �޽��� �з� ��ȣ
 *
 * @return	�����Ǿ� �ִ� ����� �޽��� ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int logm_getlevel(int category);

/**
 * �޽����� ����ϴ� �Լ�
 *
 * @param category	�޽��� �з� ��ȣ
 *
 * @param level		�޽��� ����
 *
 * @param format	�޽��� ����
 *
 * @param ...		�޽����� ���Ե� �����͵�
 *
 * @return			����� �޽��� ����<br>
 * 					<br>
 *                 -1: ����<br>
 */
int logm_printf(int category, int level, const char * tag, const char * format, ...);

/**
 * ���ۿ� ���ڿ��� ����ϴ� �Լ�
 *
 * @param str		���ڿ��� ����� ����
 *
 * @param num		���ڿ� �ִ� ����
 *
 * @param format	���ڿ� ����
 *
 * @param ...		���ڿ��� ���Ե� �����͵�
 *
 * @return			����� ���ڿ� ����<br>
 * 					<br>
 *                 -1: ����<br>
 */
int snprintf(char * str, size_t num, const char * format, ...);

#if !(defined(__SOCHECK__) && defined(__NOSOCHECK__))

/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__ALWAYS ����) */
#define logma(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__ALWAYS,  LOGM_TAG, format, ## args)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__FATAL ����) */
#define logmf(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__FATAL,   LOGM_TAG, format, ## args)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__ERROR ����) */
#define logme(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__ERROR,   LOGM_TAG, format, ## args)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__WARNING ����) */
#define logmw(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__WARNING, LOGM_TAG, format, ## args)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__INFO ����) */
#define logmi(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__INFO,    LOGM_TAG, format, ## args)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__DEBUG ����) */
#define logmd(format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL__DEBUG,   LOGM_TAG, format, ## args)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL �⺻�� ����) */
#define logm (format, args...) logm_printf(LOGM_CATEGORY, LOGM_LEVEL,          LOGM_TAG, format, ## args)

#else

/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__ALWAYS ����) */
#define logma(format, args...) dtty_puts_nosocheck(format, 120)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__FATAL ����) */
#define logmf(format, args...) dtty_puts_nosocheck(format, 120)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__ERROR ����) */
#define logme(format, args...) dtty_puts_nosocheck(format, 120)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__WARNING ����) */
#define logmw(format, args...)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__INFO ����) */
#define logmi(format, args...)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL__DEBUG ����) */
#define logmd(format, args...)
/** �޽����� ����ϴ� ��ũ�� (LOGM_LEVEL �⺻�� ����) */
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
