/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com
  
  This file is part of the lib_ubiclib component of the Ubinos.
  
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

#include "itf_ubinos/itf/ubiclib.h"

#include "itf_ubinos/itf/ubiclib_logm.h"
#include "../ubiconfig.h"

#include "_ubiclib.h"

int ubiclib_comp_init(void) {
	int r = 0;

	logm_setlevel(LOGM_CATEGORY__ALL, LOGM_LEVEL);

	r = ubiclib_porting_comp_init();

	return r;
}

int ubiclib_comp_init_reent(void) {
	int r = 0;

	r = ubiclib_porting_comp_init_reent();

	return r;
}

