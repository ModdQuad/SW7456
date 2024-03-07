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
 * osd_glue.c
 *
 *  Created on: Sep 25, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#include "main.h"
#include "osd_glue.h"
#include "osd_mixmux.h"
#include "osd_gen.h"
#include "osd_spi.h"
#include "osd_irq.h"
#include "osd_font.h"
#include "osd_registers.h"
// *********************************************************************************************** //
// Signals
// *********************************************************************************************** //
U32 osd_signal;
U32 osd_signal_clr;
//U32 osd_signal_set;
// *********************************************************************************************** //
U32 signal_test_clear(U32 sigmask)
{
register U32 retval=osd_signal&osd_signal_clr&sigmask;
if (retval) osd_signal_clr&=~retval;
return(retval);
}
// *********************************************************************************************** //

// *********************************************************************************************** //
void debug_chars(void)
{
U32 x;

if (display_start_row) return;

#if OSD_SHOW_FPS
osd_print_at(12,2,"FPS:",0);
x=(TICK_CLOCK*20)/(stats.cpu_ticks/10);
osd_print_number(x/100,2);
osd_print_char('.');
osd_print_number(x%100,2);
#endif


#if OSD_SHOW_IRQ
osd_print_at(12,3,"IRQ:",0);
x=(stats.irq_ticks>>6)*10000/(stats.cpu_ticks>>6);
osd_print_number(x/100,2);
osd_print_char('.');
osd_print_number(x%100,2);
osd_print_char('%');
#endif

#if OSD_SHOW_STACK
{
extern U32 *_estack;
U32 *my_stack;

my_stack=(U32 *)(&_estack);
my_stack-=(FONT_HEIGHT*5);
x=29;
while (my_stack!=(U32 *)(&_estack))
	{
	character_address[x]=my_stack;
	character_attribute[x]=16;
	my_stack+=FONT_HEIGHT;
	x+=DISPLAY_WIDTH;
	}
}
#endif

#if OSD_DEBUG
osd_print_at(0,4,"Lines:",0);
osd_print_number(stats.field1_line_count,3);
osd_print_char(':');
osd_print_number(stats.field2_line_count,3);

osd_print_at(15,4,"Sync:",0);
osd_print_number(stats.field1_sync_count,3);
osd_print_char(':');
osd_print_number(stats.field2_sync_count,3);
#endif

return;
}

// *********************************************************************************************** //
void osd_delay_frames(U32 x)
{
while (x--)
	{
	while (!signal_test_clear(SIG_FRAME_END))
		{
		osd_spi_process();
		osd_gen_check();
		}
	}
}
// *********************************************************************************************** //
void display_modd_logo(U32 px,U32 py)
{
extern const U32 modd_logo[];
U32 lw=11;
U32 lh=3;
U32 x,y;
U32 idx;

for(y=0;y<lh;y++)
	{
	for(x=0;x<lw;x++)
		{
		idx=((DISPLAY_WIDTH*(py+y))+(x+px));
		character_address[idx]=&modd_logo[(y*lw+x)*FONT_HEIGHT];
		character_attribute[idx]=4;
		}
	}
}
// *********************************************************************************************** //
void display_copyright(void)
{
U32 x;
display_start_row=0;

osd_print_at(0,0,"(C) 2020,2024 by Todd Modjeski",16);
display_modd_logo(0,1);
osd_print_at(12,1,"GPL3 NO WARRANTY",0);
osd_print_at(12,3,"Built:",0);
osd_print_at(12+6,3,__DATE__,0);

x=90;
while(x--)
	{
	while (!signal_test_clear(SIG_FRAME_END))
		{
		osd_spi_process();
		osd_gen_check();
		}
	debug_chars();
	}
//osd_delay_frames(30*3);
#if OSD_DEBUG
//osd_print_at(12,3,"                  ",0);
return;
#endif

x=0;
while(x<10)
	{
	display_modd_logo((x<<1),1);
	while (!signal_test_clear(SIG_FRAME_END)) osd_spi_process();
	x++;
	}

x=0;
while(x<15)
	{
	osd_print_at(x<<1,1,"  ",0);
	osd_print_at(x<<1,2,"  ",0);
	osd_print_at(x<<1,3,"  ",0);
	while (!signal_test_clear(SIG_FRAME_END)) osd_spi_process();
	x++;
	}

for(x=1;x<=COPYRIGHT_LINES;x++)
	{
	display_start_row=x;
	while (!signal_test_clear(SIG_FRAME_END)) osd_spi_process(); // Wait for next frame
	}

return;
}
// *********************************************************************************************** //

// *********************************************************************************************** //
void osd_loop(void)
{
S32 blink_tick=0;
S32 blink_time=((registers[REG_7456_VM1]&R_7456_VM1_BT)>>1)|1;

while(1)
	{
	osd_spi_process();
	osd_gen_check();

	if (signal_test_clear(SIG_FRAME_END))
		{
		if (!display_start_row) debug_chars();
		if (!blink_time)
			{
			blink_tick++;
			switch(registers[REG_7456_VM1]&R_7456_VM1_BDC)
				{
				case 0:
					blink_mixmux(blink_tick&1);
					break;
				case 1:
					if (!blink_tick) blink_mixmux(1);
					else blink_mixmux(0);
					if (blink_tick>1) blink_tick=-1;
					break;
				case 2:
					if (!blink_tick) blink_mixmux(1);
					else blink_mixmux(0);
					if (blink_tick>2) blink_tick=-1;
					break;
				case 3:
					if (!blink_tick) blink_mixmux(0);
					else blink_mixmux(1);
					if (blink_tick>2) blink_tick=-1;
					break;
				}
			blink_time=((registers[REG_7456_VM1]&R_7456_VM1_BT)>>1)|1; // 1,3,5,7
			}
		else blink_time--;
		}
	}
}
// *********************************************************************************************** //
void osd_init(void)
{
write_register_defaults();

osd_font_init();
osd_mixmux_init();
osd_gen_init();

osd_irq_init();
osd_irq_start();
osd_spi_start();

display_copyright();
}
// *********************************************************************************************** //
#if 0
// *********************************************************************************************** //
// MODIFY the file core/main.c with the lines below
// *********************************************************************************************** //

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "osd_glue.h"
/* USER CODE END Includes */

AND

/* USER CODE BEGIN 2 */
osd_init();
osd_loop();
/* USER CODE END 2 */

/* Infinite loop */
/* USER CODE BEGIN WHILE */
while (1)

// *********************************************************************************************** //
#endif
// *********************************************************************************************** //
