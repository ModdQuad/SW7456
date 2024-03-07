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
 * osd_glue.h
 *
 *  Created on: Sep 25, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#ifndef OSD_GLUE_H_
#define OSD_GLUE_H_
// *********************************************************************************************** //
#include "portable.h"
// *********************************************************************************************** //
extern void osd_init(void);
extern void osd_loop(void);
// *********************************************************************************************** //
// Signals
// *********************************************************************************************** //
extern U32 osd_signal;
extern U32 osd_signal_clr;
//extern U32 osd_signal_set;
// *********************************************************************************************** //
#define SIG_FRAME_START (1<<0)
#define SIG_FRAME_END   (1<<1)
#define SIG_FIELD_START (1<<2)
#define SIG_FIELD_END   (1<<3)
#define SIG_RESET_INPUT (1<<24)
// *********************************************************************************************** //
#endif
