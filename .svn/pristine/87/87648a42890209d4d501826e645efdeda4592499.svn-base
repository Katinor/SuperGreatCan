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
 * @brief stdlib (Standard Library) porting 컴포넌트 인터페이스
 *
 * stdlib porting 컴포넌트 인터페이스를 정의합니다.
 */

#include "type.h"

/**
 * stdlib porting 컴포넌트를 초기화하는 함수
 *
 * 초기화 작업들 중 재진입(reenterance)을 지원하기 위한 부분은 수행하지 않습니다.<br>
 * 재진입(reenterance)을 지원하려면 stdlib_porting_comp_init_reent 함수를 추가로 호출해 주어야 합니다.<br>
 * 이 함수는 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int stdlib_porting_comp_init(void);

/**
 * stdlib porting 컴포넌트의 초기화 작업들 중 재진입(reenterance)을 지원하기 위한 부분을 수행하는 함수
 *
 * 이 함수는 시스템 시작시  bsp 컴포넌트의 main 함수에서 자동적으로 호출됩니다.
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int stdlib_porting_comp_init_reent(void);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_STDLIB_PORTING_H_ */
