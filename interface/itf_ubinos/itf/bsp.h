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

#ifndef ITF_BSP_H_
#define ITF_BSP_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file bsp.h
 *
 * @brief BSP (Board Support Package) 컴포넌트 인터페이스
 *
 * BSP 컴포넌트 인터페이스를 정의합니다.
 */

/**
 * @example	misctest01.c
 * @example	misctest02.c
 */

#include "type.h"

/** 힙 정보가 잘못되었음 */
#define BSP_ERR__INVALID_HEAPINFO		-20

/**
 * 사용자 코드 시작 함수
 *
 * 사용자 코드의 시작점이 되는 함수 입니다.
 * 사용자가 직접 만들어주어야 하며, 시스템 시작시 BSP 컴포넌트의 main 함수에서 자동적으로 호출됩니다.
 *
 * @param	argc	매개변수 배열 argv의 길이<br>
 *					<br>
 *
 * @param	argv	매개변수 배열 (BSP 컴포넌트 작성자가 임의로 정의함)<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int usrmain(int argc, char * argv[]);

/**
 * BSP 컴포넌트를 초기화하는 함수
 *
 * 이 함수는 시스템 시작시 main 함수에서 자동적으로 호출됩니다.
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int bsp_comp_init(void);

/**
 * 기본 힙 정보를  돌려주는 함수
 *
 * @param	heapaddr_p	기본 힙 주소를 저장할 변수의 주소<br>
 *					<br>
 *
 * @param	heapsize_p	기본 힙 크기(Byte)를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1 : 오류<br>
 * 			 -n : n-1 번째 매개변수가 잘못되었음<br>
 * 			 BSP_ERR__INVALID_HEAPINFO: 기본 힙 정보가 잘못되었음<br>
 */
int bsp_getdefaultheapinfo(unsigned int * heapaddr_p,  unsigned int * heapsize_p);

/**
 * CPU 클럭 주파수(Hz)를  돌려주는 함수
 *
 * @param	freq_p	CPU 클럭 주파수(Hz)를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getcpuclockfreq(unsigned int * freq_p);

/**
 * CPU 클럭 주파수(KHz)를  돌려주는 함수
 *
 * @param	freqk_p	CPU 클럭 주파수(KHz)를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getcpuclockfreqk(unsigned int * freqk_p);

/**
 * 마스터 클럭 주파수(Hz)를  돌려주는 함수
 *
 * @param	freq_p	마스터  클럭 주파수(Hz)를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getmckfreq(unsigned int * freq_p);

/**
 * 마스터 클럭 주파수(KHz)를  돌려주는 함수
 *
 * @param	freqk_p	마스터  클럭 주파수(KHz)를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getmckfreqk(unsigned int * freqk_p);

/**
 * CPU ID를 돌려주는 함수
 *
 * @param	buf		CPU ID를 저장할 버퍼<br>
 *					<br>
 *
 * @param	max		CPU ID를 저장할 버퍼의 최대 크기<br>
 *					<br>
 *
 * @return	CPU ID 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getcpuid(unsigned char * buf, int max);

/**
 * CPU ID의 크기를 돌려주는 함수
 *
 * @return	CPU ID 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int bsp_getcpuidsize(void);

/**
 * 모든 인터럽트를 활성화하는 함수
 *
 * intr_enable 함수로 활성화된 번호의 인터럽트만 활성화함
 */
void bsp_enableintr(void);

/**
 * 모든 인터럽트를 비활성화하는 함수
 */
void bsp_disableintr(void);

/**
 * 시스템을 중지시키는 함수
 */
void bsp_abortsystem(void);

/**
 * 스택 오버플로우 검사 여유 영역 크기를 돌려주는 함수
 *
 * @return	 스택 오버플로우 검사 여유 영역
 */
unsigned int bsp_getsocheckmargin(void);

/**
 * 스택 오버플로우 검사 여유 영역 크기를 돌려주는 함수 (스택 오버플로우 검사 생략)
 *
 * @return	 스택 오버플로우 검사 여유 영역
 */
unsigned int bsp_getsocheckmargin_nosocheck(void);

/**
 * 스택 영역 크기를 돌려주는 함수
 *
 * @param	type	스택 영역 종류<br>
 *					<br>
 * 					0: system
 *					<br>
 *
 * @param	size_p	스택 영역 크기를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getstacksize(int type, unsigned int * stacksize_p);

/**
 * 스택 영역 크기를 돌려주는 함수 (스택 오버플로우 검사 생략)
 *
 * @param	type	스택 영역 종류<br>
 *					<br>
 * 					0: system
 *					<br>
 *
 * @param	size_p	스택 영역 크기를 저장할 변수의 주소<br>
 *					<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getstacksize_nosocheck(int type, unsigned int * stacksize_p);

/**
 * 스택 사용량 최고 기록을 돌려주는 함수
 *
 * @param	type			스택 영역 종류<br>
 *							<br>
 * 							0: system
 *							<br>
 *
 * @param	maxstackusage_p	스택 사용량 최고 기록을 저장할 변수의 주소<br>
 *							<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getmaxstackusage(int type, unsigned int * maxstackusage_p);

/**
 * 스택 사용량 최고 기록을 돌려주는 함수 (스택 오버플로우 검사 생략)
 *
 * @param	type			스택 영역 종류<br>
 *							<br>
 * 							0: system
 *							<br>
 *
 * @param	maxstackusage_p	스택 사용량 최고 기록을 저장할 변수의 주소<br>
 *							<br>
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_getmaxstackusage_nosocheck(int type, unsigned int * maxstackusage_p);

/**
 * CPU를 점유한 채로 기다리는 함수
 *
 * @param	count	기다릴 시간 (루프 횟수)<br>
 *					<br>
 */
void bsp_busywait(unsigned int count);

/**
 * CPU를 점유한 채로 기다리는 함수 (천분의 일초 단위)
 *
 * @param	timems	기다릴 시간 (천분의 일초)<br>
 *					<br>
 */
void bsp_busywaitms(unsigned int timems);

/**
 * 천분의 일초를 busy wait 하기 위해 필요한 루프 횟수를 돌려주는 함수
 *
 * @return	 천분의 일초를 busy wait 하기 위해 필요한 루프 횟수<br>
 */
unsigned int bsp_getbusywaitcountperms(void);

/**
 * 시간을 busy wait count로 변환하는 함수
 *
 * @param	timems	시간(천분의 일초)<br>
 * 					<br>
 *
 * @return	busy wait count<br>
 */
unsigned int bsp_timemstobwc(unsigned int timems);

/**
 * busy wait count를 시간으로 변환하는 함수
 *
 * @param	count	busy wait count<br>
 * 					<br>
 *
 * @return	시간(천분의 일초)<br>
 */
unsigned int bsp_bwctotimems(unsigned int count);

/* 2015. 06. 05. RTLAB_GYLee */
/* add us busywaiting*/
/**
 * CPU를 점유한 채로 기다리는 함수 (십만분의 일초 단위)
 *
 * @param	timems	기다릴 시간 (십만분의 일초)<br>
 *					<br>
 */
void bsp_busywaitus(unsigned int timems);

/**
 * 십만분의 일초를 busy wait 하기 위해 필요한 루프 횟수를 돌려주는 함수
 *
 * @return	 십만분의를 busy wait 하기 위해 필요한 루프 횟수<br>
 */
unsigned int bsp_getbusywaitcountperus(void);

/**
 * 시간을 busy wait count로 변환하는 함수
 *
 * @param	timems	시간(십만분의 일초)<br>
 * 					<br>
 *
 * @return	busy wait count<br>
 */
unsigned int bsp_timeustobwc(unsigned int timems);

/**
 * busy wait count를 시간으로 변환하는 함수
 *
 * @param	count	busy wait count<br>
 * 					<br>
 *
 * @return	시간(십만분의 일초)<br>
 */
unsigned int bsp_bwctotimeus(unsigned int count);
/**
 * 메모리 맵 정보를 문자열 형태로 메모리 버퍼에 기록하는 함수
 *
 * @param	buf		기록할 메모리  버퍼<br>
 *					<br>
 *
 * @param	max		메모리 버퍼의 크기<br>
 *
 * @return	기록된 문자열 크기<br>
 * 			<br>
 * 			 -1: 오류<br>
 * 			 -n: n-1 번째 매개변수가 잘못되었음<br>
 */
int bsp_sprintmemmap(char * buf, int max);

/**
 * 메모리 맵 정보를 출력하는 함수
 *
 * @return	  0: 성공<br>
 * 			<br>
 * 			 -1: 오류<br>
 */
int bsp_printmemmap(void);

/**
 * 첫번째 태스크를 시작하기 위해 필요한 준비 작업을 수행하는 함수
 *
 * 커널에 의해 자동적으로 호출되며, 사용자가 임의로 호출해서는 안 됨
 */
void _bsp_initfirsttask(void);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_BSP_H_ */
