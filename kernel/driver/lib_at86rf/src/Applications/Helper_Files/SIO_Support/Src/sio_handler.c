/**
 * @file sio_handler.c
 *
 * @brief Implements the SIO (UART and USB) write and read functionalities
 *
 * $Id: sio_handler.c,v 1.4 2010-08-05 05:41:48 slb Exp $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "../../../../../ubiconfig.h"

#include "../../../../../../itf_ubinos/hdr/bsp_dtty.h"

/* === Includes ============================================================ */

#ifdef SIO_HUB
#include <stdio.h>
#include "../../../../PAL/Inc/pal.h"
#include "../Inc/sio_handler.h"

/* === Macros ============================================================== */


/* === Globals ============================================================= */


/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */

#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
retval_t _pal_sio_init(uint8_t sio_unit) {
	return 0;
}

uint8_t _pal_sio_tx(uint8_t sio_unit, uint8_t *data, uint8_t length) {
	int r;

	r = dtty_puts( (const char *) data, (int) length );

	return (uint8_t) r;
}

uint8_t _pal_sio_rx(uint8_t sio_unit, uint8_t *data, uint8_t max_length) {
	int i;
	int r;

	for (i=0; i<max_length; i++) {
		r = dtty_getc((char *)(&data[i]));
		if (0 != r) {
			break;
		}
	}

	return i;
}

#define pal_sio_init(sio_unit)				_pal_sio_init(sio_unit)
#define pal_sio_tx(channel, buf, size) 		_pal_sio_tx(channel, buf, size)
#define pal_sio_rx(channel, buf, size)		_pal_sio_rx(channel, buf, size)
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */

#if ((defined __ICCAVR__) || (defined __ICCARM__))
int _sio_putchar(char data)
#else
int _sio_putchar(char data, FILE *dummy)
#endif
{
    uint8_t c = data;

    if (c == '\n')
    {
        c = '\r';

        while (0 == pal_sio_tx(SIO_CHANNEL, &c, 1))
        {
#ifdef USB0
            /*
             * For USB we need to call the USB handler via pal_task()
             * within this while loop.
             */
            pal_task();
#endif
        }

        c = data;
    }

    while (0 == pal_sio_tx(SIO_CHANNEL, &c, 1))
    {
#ifdef USB0
        /*
         * For USB we need to call the USB handler via pal_task()
         * within this while loop.
         */
        pal_task();
#endif
    }

    return (0);
}



#if ((defined __ICCAVR__) || (defined __ICCARM__))
int _sio_getchar(void)
#else
int _sio_getchar(FILE *dummy)
#endif
{
    uint8_t c;

    while (0 == pal_sio_rx(SIO_CHANNEL, &c, 1))
    {
#ifdef USB0
        /*
         * For USB we need to call the USB handler via pal_task()
         * within this while loop.
         */
        pal_task();
#endif
    }

    return c;
}



#if ((defined __ICCAVR__) || (defined __ICCARM__))
int _sio_getchar_nowait(void)
#else
int _sio_getchar_nowait(FILE *dummy)
#endif
{
    uint8_t c;

#ifdef USB0
    /*
     * In case this is used in a while loop with USB,
     * we need to call the USB handler via pal_task().
     */
    pal_task();
#endif

#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
    int back = pal_sio_rx(SIO_CHANNEL, &c, 1);
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
    int back = 0;
    if(dtty_kbhit()) {
    	back = pal_sio_rx(SIO_CHANNEL, &c, 1);
    }
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */

    if (back == 1)
    {
        return c;
    }
    else
    {
        return (-1);
    }
}

#endif  /* SIO_HUB */

/* EOF */
