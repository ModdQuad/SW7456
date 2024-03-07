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
 * vid_font.c
 *
 *  Created on: Sep 25, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#include "osd_font.h"
#include "osd_irq.h"
#include "osd_registers.h"
// *********************************************************************************************** //
U32 const *character_address[CHAR_BUFF_SIZE+(DISPLAY_WIDTH*COPYRIGHT_LINES)];
U8 character_attribute[CHAR_BUFF_SIZE+(DISPLAY_WIDTH*COPYRIGHT_LINES)];
// *********************************************************************************************** //
static U32 _index_osd_=0;
// *********************************************************************************************** //
void osd_cls(void)
{
int x;

for (x=(DISPLAY_WIDTH*COPYRIGHT_LINES);x<CHAR_BUFF_SIZE+(DISPLAY_WIDTH*COPYRIGHT_LINES);)
	{
//	display_address[x]=blank;
	character_attribute[x]=0;
	character_address[x++]=blank;
	}
}
// *********************************************************************************************** //
U32 osd_set_position(U32 col,U32 row)
{
U32 t;
t=col+row*DISPLAY_WIDTH;
t&=CHAR_BUFF_MASK;
_index_osd_=t;
return(t);
}
// *********************************************************************************************** //
void osd_print_char(U32 ch)
{
U32 c;
c=ch;
if (c<32 || c>0x7f) c=0x7f;
character_address[_index_osd_++]=&system_font[(c-32)*FONT_HEIGHT];
_index_osd_&=CHAR_BUFF_MASK;
}
// *********************************************************************************************** //
void osd_print_charaddr(U32 *charaddr)
{
character_address[_index_osd_++]=charaddr;
_index_osd_&=CHAR_BUFF_MASK;
}
// *********************************************************************************************** //
void osd_print_string(const char *str,U8 attribute)
{
U32 pos=_index_osd_;
U32 c;
pos&=CHAR_BUFF_MASK;
while (*str)
	{
	c=*str++;
	if (c<32 || c>0x7f) c=0x7f;
	character_attribute[pos]=attribute;
	character_address[pos++]=&system_font[(c-32)*FONT_HEIGHT];
	pos&=CHAR_BUFF_MASK;
	}
_index_osd_=pos;
}
// *********************************************************************************************** //
void osd_print_at(U32 x,U32 y,char *str,U8 attribute)
{
osd_set_position(x,y);
osd_print_string(str,attribute);
}
// *********************************************************************************************** //
void osd_print_number(U32 num,U32 digits)
{
U32 pos=_index_osd_;
U32 t;
U32 c;

_index_osd_+=digits;
_index_osd_&=CHAR_BUFF_MASK;

if (!digits || digits>20) return;

pos+=(digits-1);
pos&=CHAR_BUFF_MASK;

if (digits>2)
	{
	for(t=2;t<digits;t++)
		{
		c='0'+(num%10);
		character_address[(pos--)]=&system_font[(c-32)*FONT_HEIGHT];
		num=num/10;
		pos&=CHAR_BUFF_MASK;
		}
	}

c='0'+(num%10);
character_address[(pos--)]=&system_font[(c-32)*FONT_HEIGHT];
if (digits==1) return;
pos&=CHAR_BUFF_MASK;


c='0'+((num/10)&15);
character_address[(pos--)]=&system_font[(c-32)*FONT_HEIGHT];
}
// *********************************************************************************************** //
void osd_print_char_number(U32 chr,U32 num,U32 digits)
{
osd_print_char(chr);
osd_print_number(num,digits);
}
// *********************************************************************************************** //
void osd_print_number_char(U32 num,U32 digits,U32 chr)
{
osd_print_number(num,digits);
osd_print_char(chr);
}
// *********************************************************************************************** //
void osd_print_char_number_char(U32 ch1,U32 num,U32 digits,U32 ch2)
{
osd_print_char(ch1);
osd_print_number_char(num,digits,ch2);
}
// *********************************************************************************************** //

// *********************************************************************************************** //
void osd_font_init(void)
{
U32 x;
for (x=0;x<CHAR_BUFF_SIZE+(DISPLAY_WIDTH*COPYRIGHT_LINES);x++)
	{
	character_attribute[x]=0;
	character_address[x]=blank;
	}
}
// *********************************************************************************************** //
const U32 blank[FONT_HEIGHT] __attribute__ ((aligned(64)))  = {
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
		};
// *********************************************************************************************** //
const U32 modd_logo[] __attribute__ ((aligned(64))) = {
#include "fonts/modd_logo.font"
};
// *********************************************************************************************** //
const U32 system_font[] __attribute__ ((aligned(64))) = {
#include "fonts/ascii-b1.font"
#include "fonts/ascii-b2.font"
#include "fonts/ascii-b3.font"
};
// *********************************************************************************************** //
const U32 font[] __attribute__ ((aligned(64))) = {
#include "fonts/default-b0.font"
#include "fonts/default-b1.font"
#include "fonts/default-b2.font"
#include "fonts/default-b3.font"
#include "fonts/default-b4.font"

#include "fonts/betaflight-b5.font"
#include "fonts/betaflight-b6.font"
#include "fonts/betaflight-b7.font"
};
// *********************************************************************************************** //

// *********************************************************************************************** //

