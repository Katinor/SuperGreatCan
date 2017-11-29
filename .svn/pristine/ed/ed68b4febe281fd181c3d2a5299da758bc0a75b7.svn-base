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

#include "../ubiconfig.h"

#if (1 != EXCLUDE_RAND)

#include "itf_ubinos/itf/type.h"

#if (0 == UBI_INCLUDE_STDLIB)

#include <stdlib.h>

/*
 * Ref: http://www.bobwheeler.com/statistics/Password/MarsagliaPost.txt
 */

#if 	(INT_SIZE == 2)

	static unsigned int m_w = 7954;
	static unsigned int m_z = 5530;

	int rand(void) {
	    m_z = 144 * (m_z & 256) + (m_z >> 8);
	    m_w = 70  * (m_w & 256) + (m_w >> 8);

	    return abs((m_z << 8) + m_w);
	}

#elif	(INT_SIZE == 4)

	static unsigned int m_w = 521288629;
	static unsigned int m_z = 362436069;

	int rand(void) {
	    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	    m_w = 18000 * (m_w & 65535) + (m_w >> 16);

	    return abs((m_z << 16) + m_w);
	}

#else

	#error "Unsupported int size"

#endif


void srand(unsigned int seed) {
	m_w = seed;
}

#endif /* (0 == UBI_INCLUDE_STDLIB) */

#endif /* (1 != EXCLUDE_RAND) */
