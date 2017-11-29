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

#ifndef ITF_OBJTYPE_H_
#define ITF_OBJTYPE_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file objtype.h
 *
 * @brief 객체 형 정의
 */

/** NULL 객체 형 번호 */
#define OBJTYPE__NULL				0x00

/** ubik 태스크 객체 형 번호 */
#define OBJTYPE__UBIK_TASK			0x10
/** ubik 아이들 태스크 객체 형 번호 */
#define OBJTYPE__UBIK_IDLETASK		0x18

/** ubik 시그널 객체 형 번호 */
#define OBJTYPE__UBIK_SIGNAL		0x11
/** ubik 뮤텍스 객체 형 번호 */
#define OBJTYPE__UBIK_MUTEX			0x12
/** ubik 세마포어 객체 형 번호 */
#define OBJTYPE__UBIK_SEM			0x13
/** ubik 큐 객체 형 번호 */
#define OBJTYPE__UBIK_MSGQ			0x14
/** ubik 컨디션 변수 객체 형 번호 */
#define OBJTYPE__UBIK_CONDV			0x15
/** ubik 세마포어 타이머 객체 형 번호 */
#define OBJTYPE__UBIK_STIMER		0x16
/** 입출력버퍼 객체 형 번호 */
#define OBJTYPE__UBIK_IOBUF			0x17

/** 힙 객체 형 번호 */
#define OBJTYPE__UBICLIB_HEAP		0x30

/** 플래시 객체 형 번호 */
#define OBJTYPE__FLASH				0x50

/** 객체 자료 구조 */
typedef	struct _obj_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved				:  7;

	unsigned int		reserved2				: 16;
} obj_t;

/**
 * @var typedef struct _obj_t obj_t
 *
 * 객체 형 정의
 */

/** 객체 포인터 형 정의 */
typedef obj_t * obj_pt;

#ifdef	__cplusplus
}
#endif

#endif /* ITF_OBJTYPE_H_ */
