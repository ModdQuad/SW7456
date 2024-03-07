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
 * osd_mixer.h
 *
 *  Created on: Nov 21, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#ifndef OSD_MIXMUX_H_
#define OSD_MIXMUX_H_
// *********************************************************************************************** //
#include "portable.h"
// *********************************************************************************************** //
extern U8 mixmux[8][4];
// *********************************************************************************************** //
extern void osd_mixmux_init(void);
extern void blink_mixmux(S32 on);
extern void set_mixmux(void);
// *********************************************************************************************** //
#define SMOO 0x08
#define ENABLE 0x01
#define TADD SMOO|ENABLE
#define TRANS 0x20+SMOO
#define GEN 1
// *********************************************************************************************** //
#define DEEPBLACK 0x20+TADD
#define BLACK 0x30+TADD
#define GREY9 0x40+TADD
#define GREY8 0x50+TADD
#define GREY7 0x60+TADD
#define GREY6 0x70+TADD
#define GREY5 0x80+TADD
#define GREY4 0x90+TADD
#define GREY3 0xA0+TADD
#define GREY2 0xB0+TADD
#define GREY1 0xC0+TADD
#define GREY0 0xD0+TADD
#define WHITE 0xE0+TADD
// *********************************************************************************************** //
#endif
