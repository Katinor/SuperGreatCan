/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com
  
  This file is part of the lib_ubiclib component of the Ubinos.
  
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

#ifndef _UBICLIB_H_
#define _UBICLIB_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file _ubiclib.h
 *
 * @brief ubiclib ������Ʈ ���� �������̽�
 *
 * ubiclib ������Ʈ ���� �������̽��� �����մϴ�.
 */

/**
 * ubiclib porting ������Ʈ�� �ʱ�ȭ�ϴ� �Լ�
 *
 * �ʱ�ȭ �۾��� �� ������(reenterance)�� �����ϱ� ���� �κ��� �������� �ʽ��ϴ�.
 * ������(reenterance)�� �����Ϸ��� ubiclib_port_comp_init_reent �Լ��� �߰��� ȣ���� �־�� �մϴ�.
 * �� �Լ��� ubiclib_comp_init �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @return	 0: ����,<br>
 * 			-1: ����
 */
int ubiclib_porting_comp_init(void);

/**
 * ubiclib porting ������Ʈ�� �ʱ�ȭ �۾��� �� ������(reenterance)�� �����ϱ� ���� �κ��� �����ϴ� �Լ�
 *
 * �� �Լ��� ubiclib_comp_init_reent �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @return	 0: ����,<br>
 * 			-1: ����
 */
int ubiclib_porting_comp_init_reent(void);

#ifdef	__cplusplus
}
#endif

#endif /* _UBICLIB_H_ */
