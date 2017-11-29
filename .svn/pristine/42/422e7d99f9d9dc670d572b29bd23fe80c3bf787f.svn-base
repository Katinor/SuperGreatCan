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

#ifndef ITF_FLASH_H_
#define ITF_FLASH_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file flash.h
 *
 * @brief 플래시 (플래시 칩 드라이버) 컴포넌트 인터페이스
 */

#include "type.h"

/** 오류: 잠겨져 있음 */
#define FLASH_ERR__LOCK				-21

/** 오류: 잘못된 명령어 */
#define FLASH_ERR__PROGRAMMING		-22

/** 오류: 지원하지 않음 */
#define FLASH_ERR__UNSUPPORTED		-23

/** 플레시 쓰기시 데이터를 지우는 방법 선택 사항: 지우지 않음(이전 데이터와 새 데이터를 or한 값이 저장됨 */
#define FLASH_OPT__ERASE_NONE		0

/** 플레시 쓰기시 데이터를 지우는 방법 선택 사항: 섹터 전체를 지움(새 데이터 값이 저장됨) */
#define FLASH_OPT__ERASE_ALL		1

/** 플레시 쓰기시 데이터를 지우는 방법 선택 사항: 데이터가 변경되는 부분만 지움(새 데이터 값이 저장됨) */
#define FLASH_OPT__ERASE_CHANGED	2

typedef struct _flash_t {
	unsigned int			type					:  8;
	unsigned int			valid					:  1;
	unsigned int			reserved				:  7;
	unsigned int			reserved2				:  8;
	unsigned int			reserved3				:  8;

	struct {
		int (*create_fp)			(struct _flash_t ** flash_p, unsigned int baddr);
		int (*delete_fp)			(struct _flash_t ** flash_p);
		int (*writebyte_fp)			(struct _flash_t * flash, unsigned int sector, unsigned int offset, unsigned int data);
		int (*writesector_fp)		(struct _flash_t * flash, unsigned int sector, unsigned int offset, void * buf, unsigned int size, int erase, unsigned int * written_p);
		int (*readsector_fp)		(struct _flash_t * flash, unsigned int sector, unsigned int offset, void * buf, unsigned int size, unsigned int * read_p);
		int (*write_fp)				(struct _flash_t * flash, unsigned int addr, void * buf, unsigned int size, unsigned int * written_p);
		int (*erase_fp)				(struct _flash_t * flash, unsigned int sector);
		int (*eraseall_fp)			(struct _flash_t * flash);
		int (*getbaddr_fp)			(struct _flash_t * flash, unsigned int *  baddr_p);
		int (*getsize_fp)			(struct _flash_t * flash, unsigned int * size_p);
		int (*getsectorcount_fp)	(struct _flash_t * flash, unsigned int * sectorcount_p);
		int (*getsectorbaddr_fp)	(struct _flash_t * flash, unsigned int sector, unsigned int * sectorbaddr_p);
		int (*getsectorsize_fp)		(struct _flash_t * flash, unsigned int sector, unsigned int * sectorsize_p);
		int (*getsectorsizemax_fp)	(struct _flash_t * flash, unsigned int * sectorsizemax_p);
		int (*getaddr_fp)			(struct _flash_t * flash, unsigned int sector, unsigned int offset, unsigned int * addr_p);
		int (*getsector_fp)			(struct _flash_t * flash, unsigned int addr, unsigned int * sector_p, unsigned int * offset_p);
		int (*getchipid_fp)			(struct _flash_t * flash, unsigned char * chipid_p, int max);
		int (*getchipidsize_fp)		(struct _flash_t * flash);
	} op;
} flash_t;

/**
 * 플래시 포인터 형 정의
 */
typedef	flash_t * flash_pt;

/**
 * 플래시를 생성하는 함수
 *
 * @param	flash_p		생성한 플래시의 주소를 저장할 포인터의 주소<br>
 * 						<br>
 *
 * @param	baddr		플래시 베이스 주소<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_create(flash_pt * flash_p, unsigned int baddr);

/**
 * 대상 플래시를 제거하는 함수
 *
 * @param	flash_p		제거할 대산 플래시의 주소가 저장된 포인터의 주소<br>
 * 						<br>
 * 						제거에 성공하면 *flash_p는  NULL이 됨<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_delete(flash_pt * flash_p);

/**
 * 대상 플래시의 특정 섹터에 데이터 한 바이트를 기록하는 함수
 *
 * @param	flash	대상 플래시 포인터<br>
 * 					<br>
 *
 * @param	sector	기록할 섹터 번호 (0~)<br>
 * 					<br>
 *
 * @param	offset	기록할 오프셋<br>
 * 					<br>
 *
 * @param	data	기록할 데이터<br>
 * 					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			FLASH_ERR__LOCK			: 잠김 오류<br>
 * 			FLASH_ERR__PROGRAMMING	: 프로그래밍 오류<br>
 */
int flash_writebyte(flash_pt flash, unsigned int sector, unsigned int offset, unsigned int data);

/**
 * 대상 플래시의 특정 섹터에 데이터를 쓰는 함수
 *
 * @param	flash		대상 플래시 포인터<br>
 * 						<br>
 *
 * @param	sector		쓸 섹터 번호 (0~)<br>
 * 						<br>
 *
 * @param	offset		쓸 오프셋<br>
 * 						<br>
 *
 * @param	buf			쓸 데이터가 들어있는 버퍼<br>
 * 						<br>
 *
 * @param	size		쓸 데이터 크기<br>
 * 						<br>
 *
 * @param	erase		 데이터를 지우는 방법 선택(FLASH_OPT__ERASE_...)<br>
 * 						<br>
 *
 * @param	written_p	쓰여진 데이터 크기를 저장할 변수의 주소<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			FLASH_ERR__LOCK			: 잠김 오류<br>
 * 			FLASH_ERR__PROGRAMMING	: 프로그래밍 오류<br>
 */
int flash_writesector(flash_pt flash, unsigned int sector, unsigned int offset, void * buf, unsigned int size, int erase, unsigned int * written_p);

/**
 * 대상 플래시의 특정 섹터에서 데이터를 읽어오는 함수
 *
 * @param	flash	대상 플래시 포인터<br>
 * 					<br>
 *
 * @param	sector	읽어올 섹터 번호 (0~)<br>
 * 					<br>
 *
 * @param	offset	읽어올 오프셋<br>
 * 					<br>
 *
 * @param	buf		읽어온 데이터를 저장할 버퍼<br>
 * 					<br>
 *
 * @param	size	읽어올 데이터 크기<br>
 * 					<br>
 *
 * @param	read_p	읽어온 데이터 크기를 저장할 변수의 주소<br>
 * 					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			FLASH_ERR__LOCK			: 잠김 오류<br>
 * 			FLASH_ERR__PROGRAMMING	: 프로그래밍 오류<br>
 */
int flash_readsector(flash_pt flash, unsigned int sector, unsigned int offset, void * buf, unsigned int size, unsigned int * read_p);

/**
 * 대상 플래시의 특정 주소에 데이터를 기록하는 함수
 *
 * @param	flash		대상 플래시 포인터<br>
 * 						<br>
 *
 * @param	addr		기록할 목적지 주소<br>
 * 						<br>
 *
 * @param	buf			기록할 데이터가 들어있는 버퍼<br>
 * 						<br>
 *
 * @param	size		기록할 데이터 크기<br>
 * 						<br>
 *
 * @param	written_p	기록한 데이터 크기를 저장할 변수의 주소 (NULL이면 무시)<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			FLASH_ERR__LOCK			: 잠김 오류<br>
 * 			FLASH_ERR__PROGRAMMING	: 프로그래밍 오류<br>
 */
int flash_write(flash_pt flash, unsigned int addr, void * buf, unsigned int size, unsigned int * written_p);

/**
 * 대상 플래시의 특정 섹터를 지우는 함수
 *
 * @param	flash	대상 플래시 포인터<br>
 * 					<br>
 *
 * @param	sector	지울 섹터 번호 (0~)<br>
 * 					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			FLASH_ERR__LOCK			: 잠김 오류<br>
 * 			FLASH_ERR__PROGRAMMING	: 프로그래밍 오류<br>
 */
int flash_erase(flash_pt flash, unsigned int sector);

/**
 * 대상 플래시의 모든 섹터를 지우는 함수
 *
 * @param	flash	대상 플래시 포인터<br>
 * 					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 * 			FLASH_ERR__LOCK			: 잠김 오류<br>
 * 			FLASH_ERR__PROGRAMMING	: 프로그래밍 오류<br>
 */
int flash_eraseall(flash_pt flash);

/**
 * 대상 플래시의 베이스 주소를 돌려주는  함수
 *
 * @param	flash	대상 플래시 포인터<br>
 * 					<br>
 *
 * @param	baddr_p	베이스 주소를 저장할 변수의 주소<br>
 * 					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_getbaddr(flash_pt flash, unsigned int * baddr_p);

/**
 * 대상 플래시의 크기를 돌려주는 함수
 *
 * @param	flash	대상 플래시 포인터<br>
 * 					<br>
 *
 * @param	size_p	플래시의 크기를 저장할 변수의 주소<br>
 * 					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_getsize(flash_pt flash, unsigned int * size_p);

/**
 * 대상 플래시의 섹터 수를 돌려주는 함수
 *
 * @param	flash			대상 플래시 포인터<br>
 * 							<br>
 *
 * @param	sectorcount_p	플래시의 섹터 수를 저장할 변수의 주소<br>
 * 							<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_getsectorcount(flash_pt flash, unsigned int * sectorcount_p);

/**
 * 대상 플래시의 특정 섹터의 베이스 주소를 돌려주는 함수
 *
 * @param	flash			대상 플래시 포인터<br>
 * 							<br>
 *
 * @param	sector			섹터 번호 (0~)<br>
 * 							<br>
 *
 * @param	sectorbaddr_p	섹터의 베이스 주소를 저장할 변수의 주소<br>
 * 							<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_getsectorbaddr(flash_pt flash, unsigned int sector, unsigned int * sectorbaddr_p);

/**
 * 대상 플래시의 특정 섹터의 크기를 돌려주는 함수
 *
 * @param	flash			대상 플래시 포인터<br>
 * 							<br>
 *
 * @param	sector			섹터 번호 (0~)<br>
 * 							<br>
 *
 * @param	sectorsize_p	섹터의 크기를 저장할 변수의 주소<br>
 * 							<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_getsectorsize(flash_pt flash, unsigned int sector, unsigned int * sectorsize_p);

/**
 * 대상 플래시의 섹터들 중 가장 큰 것의 크기를 돌려주는 함수
 *
 * @param	flash			대상 플래시 포인터<br>
 * 							<br>
 *
 * @param	sectorsizemax_p	섹터들 중 가장 큰 것의 크기를 저장할 변수의 주소<br>
 * 							<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_getsectorsizemax(flash_pt flash, unsigned int * sectorsizemax_p);

/**
 * 대상 플래시의 섹터 번호와 오프셋으로 주소를 찾는 함수
 *
 * @param	flash	대상 플래시 포인터<br>
 * 					<br>
 *
 * @param	sector	섹터 번호 (0~)<br>
 * 					<br>
 *
 * @param	offset	오프셋<br>
 * 					<br>
 *
 * @param	addr_p	섹터 번호와 오프셋에 해당하는 주소를 저장할 변수의 주소<br>
 * 					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_getaddr(flash_pt flash, unsigned int sector, unsigned int offset, unsigned int * addr_p);

/**
 * 주소로 대상 플래시의 섹터 번호와 오프셋을 찾는 함수
 *
 * @param	flash		대상 플래시 포인터<br>
 * 						<br>
 *
 * @param	addr		주소<br>
 * 						<br>
 *
 * @param	sector_p	섹터 번호를 저장할 변수의 주소<br>
 * 						<br>
 *
 * @param	offset_p	오프셋을 저장할 변수의 주소 (NULL이면 무시)<br>
 * 						<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_getsector(flash_pt flash, unsigned int addr, unsigned int * sector_p, unsigned int * offset_p);

/**
 * 대상 플래시의 칩 아이디를 돌려주는 함수
 *
 * @param	flash	대상 플래시 포인터<br>
 * 					<br>
 *
 * @param	buf		칩 아이디를 저장할 버퍼<br>
 * 					<br>
 *
 * @param	max		칩 아이디를 저장할 버퍼의 최대 크기<br>
 * 					<br>
 *
 * @return	칩 아이디 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_getchipid(flash_pt flash, unsigned char * buf, int max);

/**
 * 대상 플래시의 칩 아이디 크기를 돌려주는 함수
 *
 * @param	flash	대상 플래시 포인터<br>
 * 					<br>
 *
 * @return	칩 아이디 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int flash_getchipidsize(flash_pt flash);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_FLASH_H_ */
