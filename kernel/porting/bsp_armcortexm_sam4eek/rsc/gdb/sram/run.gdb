#
# Copyright (C) 2009 Sung Ho Park
# Contact: ubinos.org@gmail.com
#
# This file is part of the bsp_sam7x256ek component of the Ubinos.
#
# GNU General Public License Usage
# This file may be used under the terms of the GNU
# General Public License version 3.0 as published by the Free Software
# Foundation and appearing in the file license_gpl3.txt included in the
# packaging of this file. Please review the following information to
# ensure the GNU General Public License version 3.0 requirements will be
# met: http://www.gnu.org/copyleft/gpl.html.
#
# GNU Lesser General Public License Usage
# Alternatively, this file may be used under the terms of the GNU Lesser
# General Public License version 2.1 as published by the Free Software
# Foundation and appearing in the file license_lgpl.txt included in the
# packaging of this file. Please review the following information to
# ensure the GNU Lesser General Public License version 2.1 requirements
# will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
#
# Commercial Usage
# Alternatively, licensees holding valid commercial licenses may 
# use this file in accordance with the commercial license agreement 
# provided with the software or, alternatively, in accordance with the 
# terms contained in a written agreement between you and rightful owner.
#

#target remote localhost:2331

set remote memory-write-packet-size 1024
set remote memory-read-packet-size 1024

monitor reset
monitor sleep 500
monitor speed adaptive
monitor sleep 500
monitor endian little

delete
symbol-file

# Reset perpheral  (RSTC_CR)
set *0x400e1400 = 0xA5000004

load out/out.elf
symbol-file out/out.elf

monitor reg pc = __PROCESSOR_RESET_FUNCTION_ADDRESS__

break usrmain

stepi
