/*
 * new_estk_glcd.h
 *
 *  Created on: 2015. 7. 3.
 *      Author: jaegeun
 */

#ifndef KERNEL_DRIVER_LIB_NEW_ESTK_API_ITF_NEW_ESTK_GLCD_H_
#define KERNEL_DRIVER_LIB_NEW_ESTK_API_ITF_NEW_ESTK_GLCD_H_

#include <stdio.h>
#include <stdint.h>

#include "pio/pio.h"
#include "sam4e16e.h"
#include "ioport.h"

/** SPI MISO pin definition. */
#define SPI_MISO_GPIO         (PIO_PA12_IDX)
#define SPI_MISO_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI MOSI pin definition. */
#define SPI_MOSI_GPIO         (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS        (IOPORT_MODE_MUX_A)
/** SPI SPCK pin definition. */
#define SPI_SPCK_GPIO         (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS        (IOPORT_MODE_MUX_A)

#define	SPI_MASTER_BASE	SPI
/** @define LCD Pin assignment(The serial Data Input)*/
#define LCD_SI	SPI_MOSI_GPIO
/** @define LCD Pin assignment(The serial Clock Input) */
#define	LCD_SCL	SPI_SPCK_GPIO
/** @define LCD Pin assignment(Chip select signal) */
#define	LCD_CS	PIO_PC18_IDX
#define LCD_CS_FLAGS	(PIO_OUTPUT_1 | PIO_DEFAULT)
/** @define LCD Pin assignment(It determines whether the data bits are data or a command.) */
#define	LCD_RE	PIO_PE1_IDX
#define LCD_RE_FLAGS	(PIO_OUTPUT_0 | PIO_DEFAULT)
/** @define LCD Pin assignment(LCD Back-light control PIO) */
#define	LCD_BACKLIGHT	PIO_PE0_IDX


//spi configuration
#define LCD_SPI_CHIP_SEL		1
#define LCD_SPI_CHIP_PCS		spi_get_pcs(SPI_CHIP_SEL)

#define LCD_SPI_CLK_POLARITY 	0
#define LCD_SPI_CLK_PHASE		1
#define SPI_DLYBS			0x00
#define SPI_DLYBCT			0x00
#define SPI_BUFFER_SIZE 	32

/****************************************************************************
 * LCD position 128 x 64 define
 ***************************************************************************/

/****************************************************************************
 * Standard Character Pattern define
 ***************************************************************************/
/// standard ascii 5x7 font
/// defines ascii characters 0x20-0x7F (32-127)
extern const unsigned char Font5x7[];

/**
 glcd_init
 Initialize graphic LCD component
 @param None
 @remark
 	 TM0027-Z LED를 초기화 하는 함수이다.
 @return none
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcd_init(void);

/**
 glcd_set_col
 Setting x address
 @param col dot value for x-coordinate
 @remark
	X축 방향으로 parameter 크기만큼 도트(dot) 단위씩 이동한다.
 @return
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcd_set_col(unsigned char col);

/**
 glcd_set_page
 Setting y address
 @param page	line value for y-coordinate
 @remark
 	 Y축 방향으로  parameter 크기만큼 줄 단위씩 이동한다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcd_set_page(unsigned char page);

/**
 glcd_delay
 Delay loop for a busywait
 @param cnt Count value
 @remark
 	 Graphic LCD의 응답시간 딜레이를 위한 구문이다. 즉, LCD 드라이버로 부터 명령이 수행되면서 LCD는 약간의 응답시간이 필요하고 그 시간을 조절하는 함수이다.
 	 cnt는 하나의 명령 수행주기를 갖는다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcd_delay(unsigned int cnt);

/**
 glcd_data
 Send data
 @param ch
 @remark
 	 Graphic LCD에 데이터를 전달하는 함수이다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcd_data(unsigned int ch);

/**
 glcd_cmd
 Send command
 @param cmd
 @remark
 	 Graphic LCD에 명령어를 전달하는 함수이다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcd_cmd(unsigned int cmd);

/**
 glcd_clear
 Clear the screen
 @param None
 @remark
 	 Graphic LCD의 화면을 초기화 한다. 화면 지우기 기능이다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcd_clear(void);

/**
 glcdGotoChar
 move cursor in graphic LCD
 @param col		row char value(0 ~ 20)
 @param line	column char value(0 ~ 7)
 @remark
 	 Graphic LCD의 위치를 문자 단위로 변경한다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcdGotoChar(unsigned int col, unsigned int line);

/**
 glcdWriteChar
 Print in form of a char
 @param c char value
 @remark
 	 변환된(포맷된) 문자를 Graphic LCD에 출력하는 부분이다. 확장된 개념이 glcdPutStr이다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcdWriteChar(char c);

/**
 glcdPutStr
 print in form of a string
 @param *data
 @remark
 	 변환된(포맷된) 문자열을 Graphic LCD에 출력하는 부분이다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcdPutStr(char *data);

/**
 glcd_printf
 Print on the graphic LCD
 @param format_p To display the values
 @remark
 	 Graphic LCD에 출력하는 함수이다. Shell 에서 쓰는 "printf" 구문과 유사하다. 일련의 인수 포인터를 받아들이고
 	 각 인수에 format 이 가리키고 있는 포맷 문자열에 들어 있는 포맷 명시자를 공급한 뒤 포맷된 데이터를 출력한다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcd_printf(char * format_p, ...);

/**
 glcdSetXAddress
 Setting Row Address
 @param xAddr x-coordinate value
 @remark
 	 Graphic LCD의 X좌표를 설정하는 함수이다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcdSetXAddress(unsigned int xAddr);

/**
 glcdSetYAddress
 Setting column address
 @param yAddr y-coordinate value
 @remark
 	 Graphic LCD의 Y좌표를 설정하는 함수이다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcdSetYAddress(unsigned int yAddr);

/**
 glcdSetAddress
 @param X	x-coordinate
 @param yLine	y-coordinate
 @remark
 	 Graphic LCD의 X, Y 좌표를 설정한다. X는 가로축, Y는 세로 줄이다.
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void glcdSetAddress(unsigned int X, unsigned int yLine);

/**
 DrawImage
 @param pImage
 @remark
 	 제작중인 API
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void DrawImage(unsigned char *pImage);

/**
 Draw_BMP
 Under construction
 @param cx x-coordinate
 @param cy y-coordinate
 @param szx
 @param szy
 @param buf
 @remark
 	 제작중인 API
 @return None
 @par Example

 @code
 @endcode
 @author	SH Jun
 */
void Draw_BMP(unsigned int cx, unsigned int cy, unsigned int szx, unsigned int szy, const unsigned char *buf);




#endif /* KERNEL_DRIVER_LIB_NEW_ESTK_API_ITF_NEW_ESTK_GLCD_H_ */
