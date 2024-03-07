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
 * osd_font.h
 *
 *  Created on: Sep 25, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#ifndef OSD_FONT_H_
#define OSD_FONT_H_
// *********************************************************************************************** //
#include "portable.h"
// *********************************************************************************************** //
extern const U32 font[];
extern const U32 system_font[];
extern const U32 blank[FONT_HEIGHT];
// *********************************************************************************************** //
extern U8 character_attribute[];
extern U32 const *character_address[];
// *********************************************************************************************** //
extern void osd_font_init(void);
extern void osd_cls(void);
extern U32 osd_set_position(U32 col,U32 row);
extern void osd_print_char(U32 ch);
extern void osd_print_charaddr(U32 *charaddr);
extern void osd_print_string(const char *str,U8 attribute);
extern void osd_print_at(U32 x,U32 y,char *str,U8 attribute);
extern void osd_print_number(U32 num,U32 digits);
// *********************************************************************************************** //
#endif
