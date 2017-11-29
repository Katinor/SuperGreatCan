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
 * @brief BSP (Board Support Package) ������Ʈ �������̽�
 *
 * BSP ������Ʈ �������̽��� �����մϴ�.
 */

/**
 * @example	misctest01.c
 * @example	misctest02.c
 */

#include "type.h"

/** �� ������ �߸��Ǿ��� */
#define BSP_ERR__INVALID_HEAPINFO		-20

/**
 * ����� �ڵ� ���� �Լ�
 *
 * ����� �ڵ��� �������� �Ǵ� �Լ� �Դϴ�.
 * ����ڰ� ���� ������־�� �ϸ�, �ý��� ���۽� BSP ������Ʈ�� main �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @param	argc	�Ű����� �迭 argv�� ����<br>
 *					<br>
 *
 * @param	argv	�Ű����� �迭 (BSP ������Ʈ �ۼ��ڰ� ���Ƿ� ������)<br>
 *					<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int usrmain(int argc, char * argv[]);

/**
 * BSP ������Ʈ�� �ʱ�ȭ�ϴ� �Լ�
 *
 * �� �Լ��� �ý��� ���۽� main �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int bsp_comp_init(void);

/**
 * �⺻ �� ������  �����ִ� �Լ�
 *
 * @param	heapaddr_p	�⺻ �� �ּҸ� ������ ������ �ּ�<br>
 *					<br>
 *
 * @param	heapsize_p	�⺻ �� ũ��(Byte)�� ������ ������ �ּ�<br>
 *					<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1 : ����<br>
 * 			 -n : n-1 ��° �Ű������� �߸��Ǿ���<br>
 * 			 BSP_ERR__INVALID_HEAPINFO: �⺻ �� ������ �߸��Ǿ���<br>
 */
int bsp_getdefaultheapinfo(unsigned int * heapaddr_p,  unsigned int * heapsize_p);

/**
 * CPU Ŭ�� ���ļ�(Hz)��  �����ִ� �Լ�
 *
 * @param	freq_p	CPU Ŭ�� ���ļ�(Hz)�� ������ ������ �ּ�<br>
 *					<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int bsp_getcpuclockfreq(unsigned int * freq_p);

/**
 * CPU Ŭ�� ���ļ�(KHz)��  �����ִ� �Լ�
 *
 * @param	freqk_p	CPU Ŭ�� ���ļ�(KHz)�� ������ ������ �ּ�<br>
 *					<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int bsp_getcpuclockfreqk(unsigned int * freqk_p);

/**
 * ������ Ŭ�� ���ļ�(Hz)��  �����ִ� �Լ�
 *
 * @param	freq_p	������  Ŭ�� ���ļ�(Hz)�� ������ ������ �ּ�<br>
 *					<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int bsp_getmckfreq(unsigned int * freq_p);

/**
 * ������ Ŭ�� ���ļ�(KHz)��  �����ִ� �Լ�
 *
 * @param	freqk_p	������  Ŭ�� ���ļ�(KHz)�� ������ ������ �ּ�<br>
 *					<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int bsp_getmckfreqk(unsigned int * freqk_p);

/**
 * CPU ID�� �����ִ� �Լ�
 *
 * @param	buf		CPU ID�� ������ ����<br>
 *					<br>
 *
 * @param	max		CPU ID�� ������ ������ �ִ� ũ��<br>
 *					<br>
 *
 * @return	CPU ID ũ��<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int bsp_getcpuid(unsigned char * buf, int max);

/**
 * CPU ID�� ũ�⸦ �����ִ� �Լ�
 *
 * @return	CPU ID ũ��<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int bsp_getcpuidsize(void);

/**
 * ��� ���ͷ�Ʈ�� Ȱ��ȭ�ϴ� �Լ�
 *
 * intr_enable �Լ��� Ȱ��ȭ�� ��ȣ�� ���ͷ�Ʈ�� Ȱ��ȭ��
 */
void bsp_enableintr(void);

/**
 * ��� ���ͷ�Ʈ�� ��Ȱ��ȭ�ϴ� �Լ�
 */
void bsp_disableintr(void);

/**
 * �ý����� ������Ű�� �Լ�
 */
void bsp_abortsystem(void);

/**
 * ���� �����÷ο� �˻� ���� ���� ũ�⸦ �����ִ� �Լ�
 *
 * @return	 ���� �����÷ο� �˻� ���� ����
 */
unsigned int bsp_getsocheckmargin(void);

/**
 * ���� �����÷ο� �˻� ���� ���� ũ�⸦ �����ִ� �Լ� (���� �����÷ο� �˻� ����)
 *
 * @return	 ���� �����÷ο� �˻� ���� ����
 */
unsigned int bsp_getsocheckmargin_nosocheck(void);

/**
 * ���� ���� ũ�⸦ �����ִ� �Լ�
 *
 * @param	type	���� ���� ����<br>
 *					<br>
 * 					0: system
 *					<br>
 *
 * @param	size_p	���� ���� ũ�⸦ ������ ������ �ּ�<br>
 *					<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int bsp_getstacksize(int type, unsigned int * stacksize_p);

/**
 * ���� ���� ũ�⸦ �����ִ� �Լ� (���� �����÷ο� �˻� ����)
 *
 * @param	type	���� ���� ����<br>
 *					<br>
 * 					0: system
 *					<br>
 *
 * @param	size_p	���� ���� ũ�⸦ ������ ������ �ּ�<br>
 *					<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int bsp_getstacksize_nosocheck(int type, unsigned int * stacksize_p);

/**
 * ���� ��뷮 �ְ� ����� �����ִ� �Լ�
 *
 * @param	type			���� ���� ����<br>
 *							<br>
 * 							0: system
 *							<br>
 *
 * @param	maxstackusage_p	���� ��뷮 �ְ� ����� ������ ������ �ּ�<br>
 *							<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int bsp_getmaxstackusage(int type, unsigned int * maxstackusage_p);

/**
 * ���� ��뷮 �ְ� ����� �����ִ� �Լ� (���� �����÷ο� �˻� ����)
 *
 * @param	type			���� ���� ����<br>
 *							<br>
 * 							0: system
 *							<br>
 *
 * @param	maxstackusage_p	���� ��뷮 �ְ� ����� ������ ������ �ּ�<br>
 *							<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int bsp_getmaxstackusage_nosocheck(int type, unsigned int * maxstackusage_p);

/**
 * CPU�� ������ ä�� ��ٸ��� �Լ�
 *
 * @param	count	��ٸ� �ð� (���� Ƚ��)<br>
 *					<br>
 */
void bsp_busywait(unsigned int count);

/**
 * CPU�� ������ ä�� ��ٸ��� �Լ� (õ���� ���� ����)
 *
 * @param	timems	��ٸ� �ð� (õ���� ����)<br>
 *					<br>
 */
void bsp_busywaitms(unsigned int timems);

/**
 * õ���� ���ʸ� busy wait �ϱ� ���� �ʿ��� ���� Ƚ���� �����ִ� �Լ�
 *
 * @return	 õ���� ���ʸ� busy wait �ϱ� ���� �ʿ��� ���� Ƚ��<br>
 */
unsigned int bsp_getbusywaitcountperms(void);

/**
 * �ð��� busy wait count�� ��ȯ�ϴ� �Լ�
 *
 * @param	timems	�ð�(õ���� ����)<br>
 * 					<br>
 *
 * @return	busy wait count<br>
 */
unsigned int bsp_timemstobwc(unsigned int timems);

/**
 * busy wait count�� �ð����� ��ȯ�ϴ� �Լ�
 *
 * @param	count	busy wait count<br>
 * 					<br>
 *
 * @return	�ð�(õ���� ����)<br>
 */
unsigned int bsp_bwctotimems(unsigned int count);

/* 2015. 06. 05. RTLAB_GYLee */
/* add us busywaiting*/
/**
 * CPU�� ������ ä�� ��ٸ��� �Լ� (�ʸ����� ���� ����)
 *
 * @param	timems	��ٸ� �ð� (�ʸ����� ����)<br>
 *					<br>
 */
void bsp_busywaitus(unsigned int timems);

/**
 * �ʸ����� ���ʸ� busy wait �ϱ� ���� �ʿ��� ���� Ƚ���� �����ִ� �Լ�
 *
 * @return	 �ʸ����Ǹ� busy wait �ϱ� ���� �ʿ��� ���� Ƚ��<br>
 */
unsigned int bsp_getbusywaitcountperus(void);

/**
 * �ð��� busy wait count�� ��ȯ�ϴ� �Լ�
 *
 * @param	timems	�ð�(�ʸ����� ����)<br>
 * 					<br>
 *
 * @return	busy wait count<br>
 */
unsigned int bsp_timeustobwc(unsigned int timems);

/**
 * busy wait count�� �ð����� ��ȯ�ϴ� �Լ�
 *
 * @param	count	busy wait count<br>
 * 					<br>
 *
 * @return	�ð�(�ʸ����� ����)<br>
 */
unsigned int bsp_bwctotimeus(unsigned int count);
/**
 * �޸� �� ������ ���ڿ� ���·� �޸� ���ۿ� ����ϴ� �Լ�
 *
 * @param	buf		����� �޸�  ����<br>
 *					<br>
 *
 * @param	max		�޸� ������ ũ��<br>
 *
 * @return	��ϵ� ���ڿ� ũ��<br>
 * 			<br>
 * 			 -1: ����<br>
 * 			 -n: n-1 ��° �Ű������� �߸��Ǿ���<br>
 */
int bsp_sprintmemmap(char * buf, int max);

/**
 * �޸� �� ������ ����ϴ� �Լ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int bsp_printmemmap(void);

/**
 * ù��° �½�ũ�� �����ϱ� ���� �ʿ��� �غ� �۾��� �����ϴ� �Լ�
 *
 * Ŀ�ο� ���� �ڵ������� ȣ��Ǹ�, ����ڰ� ���Ƿ� ȣ���ؼ��� �� ��
 */
void _bsp_initfirsttask(void);

#ifdef	__cplusplus
}
#endif

#endif /* ITF_BSP_H_ */
