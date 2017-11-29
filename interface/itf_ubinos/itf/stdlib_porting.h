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

#ifndef ITF_STDLIB_PORTING_H_
#define ITF_STDLIB_PORTING_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file stdlib_port.h
 *
 * @brief stdlib (Standard Library) porting ������Ʈ �������̽�
 *
 * stdlib porting ������Ʈ �������̽��� �����մϴ�.
 */

#include "type.h"

/**
 * stdlib porting ������Ʈ�� �ʱ�ȭ�ϴ� �Լ�
 *
 * �ʱ�ȭ �۾��� �� ������(reenterance)�� �����ϱ� ���� �κ��� �������� �ʽ��ϴ�.<br>
 * ������(reenterance)�� �����Ϸ��� stdlib_porting_comp_init_reent �Լ��� �߰��� ȣ���� �־�� �մϴ�.<br>
 * �� �Լ��� �ý��� ���۽�  bsp ������Ʈ�� main �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int stdlib_porting_comp_init(void);

/**
 * stdlib porting ������Ʈ�� �ʱ�ȭ �۾��� �� ������(reenterance)�� �����ϱ� ���� �κ��� �����ϴ� �Լ�
 *
 * �� �Լ��� �ý��� ���۽�  bsp ������Ʈ�� main �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int stdlib_porting_comp_init_reent(void);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_STDLIB_PORTING_H_ */
