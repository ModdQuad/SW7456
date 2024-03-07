/*
 * This file is part of SW7456.
 *
 * SW7456 is free software. You can redistribute this software and/or modify this software
 * under the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * SW7456 is distributed in the hope that they will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */
// *********************************************************************************************** //
/*
 * osd_gen.h
 *
 *  Created on: Sep 24, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#ifndef OSD_GEN_H_
#define OSD_GEN_H_
// *********************************************************************************************** //
#include "portable.h"
// *********************************************************************************************** //
#if OSD_SIGNAL_GENERATOR
extern void osd_gen_check(void);
extern void osd_gen_init(void);
extern U32 generator_on;
#else
static void osd_gen_check(void) {}
static void osd_gen_init(void) {}
#endif
// *********************************************************************************************** //
#endif
