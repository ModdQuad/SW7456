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

#ifndef OSD_OSD_H_
#define OSD_OSD_H_
// *********************************************************************************************** //
#include "portable.h"
// *********************************************************************************************** //
typedef struct
	{
	U32 field1_sync_count;
	U32 field2_sync_count;

	U32 field1_line_count;
	U32 field2_line_count;

	U32 irq_ticks_sum;
	U32 irq_ticks;
	U32 cpu_ticks_sum;
	U32 cpu_ticks;

	U32 vid_cap_ticks;
	U32 gen_cap_ticks;
	} OSD_STATS;
// *********************************************************************************************** //
extern void osd_irq_init(void);
extern void osd_irq_start(void);
extern void osd_irq_stop(void);

extern void osd_outirq_pause(void);
extern void osd_outirq_continue(void);

extern void osd_systick_pause(void);
extern void osd_systick_continue(void);
// *********************************************************************************************** //
extern U32 display_start_row;
extern OSD_STATS stats;
// *********************************************************************************************** //
#endif
