/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com
  
  This file is part of the bsp_sam3s4ek component of the Ubinos.
  
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


OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")

OUTPUT_ARCH(arm)

ENTRY(_processor_reset)


MEMORY 
{
	/* Internal SRAM  0x20000000 ~ 0x2001FFFF (128Kbyte) */
	int_sram (W!RX)	: ORIGIN = 0x20000000, LENGTH = 128K
	/* Internal FLASH 0x00400000 ~ 0x004FFFFF (1024Kbyte) */
	int_flash (RX)	: ORIGIN = 0x00400000, LENGTH = 1024K
}

__int_sram_start__			= 0x20000000;
__int_sram_end__			= 0x2001FFFF;
__int_flash_start__			= 0x00400000;
__int_flash_end__			= 0x004FFFFF;


__stack_top_temp__			= __int_sram_end__; 	/* temporary stack for lowlevel_init */
__stack_limit_temp__		= __int_sram_start__;
__stack_top__				= __int_sram_end__;

__exc_vectors_dst_start__	= __exc_vectors_start__;
__exc_vectors_src_start__	= LOADADDR(.exc_vectors);
__exc_vectors_src_end__		= LOADADDR(.exc_vectors) + SIZEOF(.exc_vectors);

__ram_section_dst_start__	= __data_start__;
__ram_section_src_start__	= LOADADDR(.data);
__ram_section_src_end__		= LOADADDR(.data) + SIZEOF(.data);


SECTIONS
{
	.exc_vectors :
	{
		. =  ALIGN(4);
		__exc_vectors_start__ = .;
		KEEP(*(.exc_vectors*))
		. =  ALIGN(4);
		__exc_vectors_end__ = .;
	} >int_flash

	.startup :
	{
		. =  ALIGN(4);
		__startup_start__ = .;
		KEEP(*(.startup*))
		. =  ALIGN(4);
		__startup_end__ = .;
	} >int_flash

	.text : 
	{
		. =  ALIGN(4);
		__text_start__ = .;
		*(.text*)
		. =  ALIGN(4);
		*(.glue_7)			/* arm to thumb glue section */
		. =  ALIGN(4);
		*(.glue_7t)			/* thumb to arm glue section */
		. =  ALIGN(4);
		*(.vfp11_veneer)	/* vfp11 erratum veneer section */
		. =  ALIGN(4);
		*(.v4_bx)			/* arm bx glue section */
		. =  ALIGN(4);
		__text_end__ = .;
	} >int_flash

	.rodata : 
	{
		. =  ALIGN(4);
		__rodata_start__ = .;
		*(.rodata*)
		. =  ALIGN(4);
		__rodata_end__ = .;
	} >int_flash

	.data : 
	{
		. =  ALIGN(4);
		__data_start__ = .;
		*(.data*)
		. =  ALIGN(4);
		__data_end_boundary__ = .;
		. = . + 4;
		__data_end__ = .;
	} >int_sram AT>int_flash

	.bss : 
	{
		. =  ALIGN(4);
		__bss_start__ = .;
		*(.bss*)
		. =  ALIGN(4);
		__bss_end_boundary__ = .;
		. = . + 4;
		__bss_end__ = .;
	} >int_sram

	.common : 
	{
		. =  ALIGN(4);
		__common_start__ = .;
		*(COMMON*)
		. =  ALIGN(4);
		__common_end_boundary__ = .;
		. = . + 4;
		__common_end__ = .;
	} >int_sram

	. =  ALIGN(4);
	__end__ = .;
	_end = .;
}
