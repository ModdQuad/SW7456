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
 * osd_config.h
 *
 *  Created on: Sep 25, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#ifndef OSD_CONFIG_H_
#define OSD_CONFIG_H_
// *********************************************************************************************** //
#define OSD_DEBUG 0
#define OSD_SHOW_FPS 1
#define OSD_SHOW_IRQ 0
#define OSD_SHOW_STACK 0
// *********************************************************************************************** //
// may not be needed for later version of bf
#define OSD_DISABLE_CLS_FOR_BETAFLIGHT 1
// *********************************************************************************************** //
// disable generator to fit in 8K of ram
#define OSD_SIGNAL_GENERATOR 1
// *********************************************************************************************** //
#ifndef __ASSEMBLER__
// *********************************************************************************************** //
#define FONT_HEIGHT 18
#define FONT_WIDTH 12
// *********************************************************************************************** //
#define DISPLAY_WIDTH  30
#define DISPLAY_HEIGHT  32
// *********************************************************************************************** //
#define CHAR_BUFF_SIZE 512
#define CHAR_BUFF_MASK (CHAR_BUFF_SIZE-1)
// *********************************************************************************************** //
#define SPI_BUFF_SIZE 2048
#define SPI_BUFF_MASK (SPI_BUFF_SIZE-1)
// *********************************************************************************************** //
#define COPYRIGHT_LINES 5
// *********************************************************************************************** //
#define NTSC_LINE_FREQ 15734.2657
#define PAL_LINE_FREQ 15625

#define NTSC_FIELD_FREQ 59.94006
#define PAL_FIELD_FREQ 50

#define NTSC_LINES_PER_FRAME 525
#define PAL_LINES_PER_FRAME 625
// *********************************************************************************************** //
#define NTSC_FRAME_TIME       0.0000635555555555556
#define NTSC_HSYNC_TIME       0.0000047
#define NTSC_FRONT_PORCH_TIME 0.0000015
#define NTSC_BACK_PORCH_TIME  0.0000047
#define NTSC_VIDEO_TIME       0.0000526
// *********************************************************************************************** //
#define LINE_FREQ ((U32)((NTSC_LINE_FREQ + PAL_LINE_FREQ+1)/2))
#define LINE_TICK_COUNT (GEN_TIMER_CLOCK_FREQ/LINE_FREQ)
#define LINE_TICK_HYSTERESIS (LINE_TICK_COUNT/4)
#define SYNC_TICK_COUNT ((U32)(LINE_TICK_COUNT/2))
#define SYNC_TICK_HYSTERESIS ((U32)(LINE_TICK_HYSTERESIS/2))

//#define SYNC_TICKS_MIN (LINE_TICK_COUNT/4)
//#define SYNC_TICKS_MID (SYNC_TICKS_MIN*3)
//#define SYNC_TICKS_MAX (SYNC_TICKS_MIN*5)

#define SYNC_TICK_MIN (SYNC_TICK_COUNT-200)
#define SYNC_TICK_MAX (SYNC_TICK_COUNT+200)

#define LINE_TICK_MIN (LINE_TICK_COUNT-300)
#define LINE_TICK_MAX (LINE_TICK_COUNT+300)
// *********************************************************************************************** //
#define GEN_TIMER_CLOCK_FREQ 64000000
// *********************************************************************************************** //
#define TICK_CLOCK 64000000
// *********************************************************************************************** //
// *********************************************************************************************** //
#endif
#endif
