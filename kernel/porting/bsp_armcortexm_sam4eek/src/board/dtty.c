/*
  Copyright (C) 2011 RTLab
  Contact: ubinos.org@gmail.com

  Contributors: Dong Hoon Kim, Sung Ho Park

  This file is part of the bsp_armcortexm_sam3sek component of the Ubinos.

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

#include "../../ubiconfig.h"
// 150416 jaegeun
#include "../../itf/sam4e16e.h"

#include <stdio.h>

#ifdef UBI_COMPONENT__lib_ubik
	#include "itf_ubinos/itf/ubinos.h"
#endif

#include "itf_ubinos/itf/bsp_dtty.h"

#include "_bsp_board.h"

#define SLEEP_TICK	1

/** board selector */
// 150416 jaegeun
#define ESTK_M4		  // estk-m4 ver SAM4E select
//#define SAM4E_EK


int _g_bsp_dtty_echo	= 0;

int dtty_init(void) {
    unsigned int abcdsr;
    
	_g_bsp_dtty_echo	= 0;

// 150416 jaegeun
#ifdef SAM4E_EK
	/* Reset and disable receiver & transmitter */
	UART0->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

	/* Disable interrupts */
	PIOA->PIO_IDR = 0x00000600;

	/* Configure PIOs for DBGU */
	PIOA->PIO_PUDR = 0x00000600;
	abcdsr = PIOA->PIO_ABCDSR[0];
	PIOA->PIO_ABCDSR[0] &= (~0x00000600 & abcdsr);
	abcdsr = PIOA->PIO_ABCDSR[1];
	PIOA->PIO_ABCDSR[1] &= (~0x00000600 & abcdsr);
	PIOA->PIO_PDR = 0x00000600;

	/* Configure PMC */
	PMC->PMC_PCER0 = (1 << ID_UART0);

	/* === Configure serial link === */
	/* Define the baud rate divisor register [BRGR = MCK / (115200 * 16)] */
	UART0->UART_BRGR = 26;
	/* Configure mode */
	UART0->UART_MR = UART_MR_PAR_NO;

	/* Disable the RX and TX PDC transfer requests */
	UART0->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	/* Enable transmitter */
	UART0->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
#endif

// ESTK-M4 -> using usart0 as RS232
#ifdef ESTK_M4
	/* Reset and disable rx & tx */
	USART0->US_MR = 0;
	USART0->US_RTOR = 0;
	USART0->US_TTGR = 0;

	// reset register
	// tx | tx | rx | rx | status | RTS,DTS
	USART0->US_CR = US_CR_RSTTX | US_CR_TXDIS | US_CR_RSTRX | US_CR_RXDIS | US_CR_RSTSTA | US_CR_RTSDIS;

	/* Disable interrupts */
	PIOB->PIO_IDR = 0x00000003;

	/* Configure PIOs for DBGU */
	PIOB->PIO_PUDR = 0x00000003;
	abcdsr = PIOB->PIO_ABCDSR[0];
	PIOB->PIO_ABCDSR[0] &= (~0x00000003 & abcdsr);
	PIOB->PIO_ABCDSR[1] |= 0x00000003;
	PIOB->PIO_PDR = 0x00000003;

	/* Configure PMC */
	PMC->PMC_PCER0 = (1 << ID_USART0);

	/* Define the baud rate divisor register [BRGR = MCK / (115200 * 16)] */
	USART0->US_BRGR = 26;

	// set mode
	USART0->US_MR = US_MR_CHRL_8_BIT | US_MR_PAR_NO | US_MR_CHMODE_NORMAL | US_MR_NBSTOP_1_BIT;

	/* Disable the RX and TX PDC transfer requests */
	USART0->US_PTCR = US_PTCR_RXTDIS | US_PTCR_TXTDIS;

	/* Enable transmitter */
	USART0->US_CR = US_CR_RXEN | US_CR_TXEN;

#endif

#if 0
	/* Reset and disable receiver & transmitter */
	UART1->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

	/* Disable interrupts */
	PIOA->PIO_IDR = 0x00000600;

	/* Configure PIOs for DBGU */
	PIOA->PIO_PUDR = 0x00000060;
	abcdsr = PIOA->PIO_ABCDSR[0];
	PIOA->PIO_ABCDSR[0] &= (~0x00000060 & abcdsr);
	abcdsr = PIOA->PIO_ABCDSR[1];
	PIOA->PIO_ABCDSR[1] |= 0x00000060;
	PIOA->PIO_PDR = 0x00000060;

	/* Configure PMC */
	PMC->PMC_PCER1 = (1 << (ID_UART1-32));

	/* === Configure serial link === */
	/* Define the baud rate divisor register [BRGR = MCK / (115200 * 16)] */
	UART1->UART_BRGR = 26;
	/* Configure mode */
	UART1->UART_MR = UART_MR_PAR_NO;

	/* Disable the RX and TX PDC transfer requests */
	UART1->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

	/* Enable transmitter */
	UART1->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
#endif

	return 0;
}

int dtty_enable(void) {
	return -10;
}

#if (1 != EXCLUDE_MISC)

int dtty_disable(void) {
	return -10;
}

#endif /* (1 != EXCLUDE_MISC) */

int dtty_putc(int ch) {
#ifdef SAM4E_EK
	for (;;)
	{
		if(UART0->UART_SR & UART_SR_TXEMPTY) {
			break;
		}
	}

	UART0->UART_THR = (ch & 0xFF);
#endif

#ifdef ESTK_M4
	for (;;)
	{
		if(USART0->US_CSR & US_CSR_TXRDY) {
			break;
		}
	}

	USART0->US_THR = (ch & 0xFF);
#endif

	return 0;
}

#if (1 != EXCLUDE_MISC)

int dtty_getc(char * ch_p) {
	unsigned int i;

	if (NULL == ch_p) {
		return -2;
	}

#ifdef UBI_COMPONENT__lib_ubik
	if(ubik_isactive()) {
		for (i=0; ; i++)
		{
#ifdef SAM4E_EK
			if(UART0->UART_SR & UART_SR_RXRDY) {
				break;
			}
#endif
#ifdef ESTK_M4
			if(USART0->US_CSR & US_CSR_RXRDY) {
				break;
			}
#endif
			if (255 <= i) {
				task_sleep(SLEEP_TICK);
				i = 0;
			}
		}
	}
#else
	for (i=0; ; i++)
	{
		if(UART0->UART_SR & UART_SR_RXRDY) {
			break;
		}
	}
#endif

#ifdef SAM4E_EK
	*ch_p = (char) ((UART0->UART_RHR) & 0xFF);
#endif

#ifdef ESTK_M4
	*ch_p = (char) ((USART0->US_RHR) & 0xFF);
#endif

	if (0 != _g_bsp_dtty_echo) {
		dtty_putc(*ch_p);
	}

	return 0;
}

#endif /* (1 != EXCLUDE_MISC) */

int dtty_puts(const char * str, int max) {
	int i = 0;

	if (NULL == str) {
		return -2;
	}

	if (0 > max) {
		return -3;
	}

	for (i=0; i<max; i++) {
		if ('\0' == *str) {
			break;
		}
		dtty_putc(*str);
		str++;
	}

	return i;
}

#if (1 != EXCLUDE_MISC)

int dtty_gets(char * str, int max) {
	int i;
	int r;

	if (NULL == str) {
		return -2;
	}

	if (0 > max) {
		return -3;
	}

	for (i=0; i<max; i++) {
		r = dtty_getc(&str[i]);
		if (0 != r || '\0' == str[i] || '\n' == str[i] || '\r' == str[i]) {
			break;
		}
	}
	if (0 != i && max == i) {
		i--;
	}
	str[i] = '\0';

	return i;
}

int dtty_kbhit(void) {
	if (UART0->UART_SR & UART_SR_RXRDY) {
		return 1;
	} else {
		return 0;
	}
}

#endif /* (1 != EXCLUDE_MISC) */

int dtty_setecho(int echo) {
	_g_bsp_dtty_echo = echo;

	return 0;
}

