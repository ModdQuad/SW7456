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
 * osd_mixmux.c
 *
 *  Created on: Nov 21, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#include "osd_mixmux.h"
#include "osd_registers.h"
// *********************************************************************************************** //
U8 mixmux[8][4];
// *********************************************************************************************** //
#define MMI_LOCAL_BKGND 4
#define MMI_BLINK 2
#define MMI_INVERT 1
// *********************************************************************************************** //
#define MMCLR_TRANS 0
#define MMCLR_BLACK 1
#define MMCLR_GREY  2
#define MMCLR_WHITE 3
// *********************************************************************************************** //
void set_mixmux(void)
{
extern U32 generator_on;
const U8 enable=1+8;
U8 blk,wht,gry,trn;
S32 x;
x=(registers[REG_7456_VM1]&R_7456_VM1_BMB);
x+=0x050;
gry=x|enable;

x=registers[REG_7456_RB0]&R_7456_RB_WL;
x=3-x;
x=(0x0c+x)<<4;
wht=x|enable; // 0xF0-0xC0 default 0xE0

x=(registers[REG_7456_RB0]&R_7456_RB_BL)>>2;
x+=3;
x<<=4;
blk=x|enable; // 0xF0-0xC0 default 0xE0

#if OSD_SIGNAL_GENERATOR
if (generator_on)
	{
	trn=1;
	gry-=0x30;
	wht-=0x30;
	blk-=0x30;
	}
else trn=TRANS;
#else
trn=TRANS;
#endif

for(x=0;x<8;x++)
	{
	if (x&MMI_LOCAL_BKGND) mixmux[x][MMCLR_TRANS]=gry; // local background
	else mixmux[x][MMCLR_TRANS]=trn;

	mixmux[x][MMCLR_GREY]=gry;
	if (x&MMI_INVERT) // invert
		{
		mixmux[x][MMCLR_BLACK]=wht;
		mixmux[x][MMCLR_WHITE]=blk;
		}
	else
		{
		mixmux[x][MMCLR_BLACK]=blk;
		mixmux[x][MMCLR_WHITE]=wht;
		}
	}
}
// *********************************************************************************************** //
void blink_mixmux(S32 on)
{
S32 x;

if (on)
	{
	U8 black=mixmux[0][MMCLR_BLACK];
	U8 white=mixmux[0][MMCLR_WHITE];

	for (x=2;x<8;x++)
		{
		if (x&MMI_BLINK)
			{
			if (x&MMI_INVERT) // invert
				{
				mixmux[x][MMCLR_BLACK]=white;
				mixmux[x][MMCLR_WHITE]=black;
				}
			else
				{
				mixmux[x][MMCLR_BLACK]=black;
				mixmux[x][MMCLR_WHITE]=white;
				}
			}
		}
	}
else
	{
	U8 trans=mixmux[0][MMCLR_TRANS];
	U8 grey=mixmux[4][MMCLR_TRANS];

	for (x=2;x<8;x++)
		{
		if (x&MMI_BLINK)
			{
			if (x&MMI_LOCAL_BKGND)
				{
				mixmux[x][MMCLR_BLACK]=grey;
				mixmux[x][MMCLR_WHITE]=grey;
				}
			else
				{
				mixmux[x][MMCLR_BLACK]=trans;
				mixmux[x][MMCLR_WHITE]=trans;
				}
			}
		}
	}
}
// *********************************************************************************************** //
void osd_mixmux_init(void)
{
set_mixmux();
}
// *********************************************************************************************** //
