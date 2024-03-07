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
 * video_gen.c
 *
 *  Created on: Sep 24, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#include "main.h"
#include "portable.h"
#include "osd_irq.h"
#include "osd_spi.h"
#include "osd_glue.h"
#include "osd_font.h"
#include "osd_registers.h"
#include "osd_mixmux.h"
// *********************************************************************************************** //
extern COMP_HandleTypeDef hcomp1;
extern COMP_HandleTypeDef hcomp2;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern DAC_HandleTypeDef hdac1;
// *********************************************************************************************** //
U32 display_start_row=COPYRIGHT_LINES;
U32 end_line;
U32 *osd_port= (uint32_t *)&GPIOB->ODR;
OSD_STATS stats;
// *********************************************************************************************** //
#define NTSC_ROWS (14-1)
#define PAL_ROWS (16-1)
// *********************************************************************************************** //
#define NTSC_LINES (((NTSC_ROWS)*FONT_HEIGHT)-1)
#define PAL_LINES (((PAL_ROWS)*FONT_HEIGHT)-1)
// *********************************************************************************************** //
void osd_irq_init(void)
{
int x;
for(x=0;x<CHAR_BUFF_SIZE+(COPYRIGHT_LINES*DISPLAY_WIDTH);x++) {character_address[x]=blank;character_attribute[x]=0;}

display_start_row=COPYRIGHT_LINES;

HAL_TIM_IC_Init(&htim14);
HAL_TIM_IC_Start(&htim14,TIM_CHANNEL_1);

HAL_TIM_Base_Start(&htim6);

HAL_TIM_IC_Start(&htim2,TIM_CHANNEL_2);
HAL_TIM_IC_Start(&htim3,TIM_CHANNEL_1);

osd_signal|=SIG_RESET_INPUT;
}
// *********************************************************************************************** //
void osd_irq_start(void)
{
	end_line=NTSC_LINES;

HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x0FF);  // for generator

//	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x140);  // comp trigger 0x1c0 0x040
// 	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x088); // 0x0a0 - 0x70 hs
//	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x0C0);
HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x120);
HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
HAL_COMP_Start(&hcomp1);

osd_systick_pause();
}
// *********************************************************************************************** //
void osd_outirq_pause(void)
{
*NVIC->ICER=0x00001000;
}
// *********************************************************************************************** //
void osd_outirq_continue(void)
{
*NVIC->ISER=0x00001000;
}
// *********************************************************************************************** //
void osd_systick_pause(void)
{
*NVIC->ICER=0x00040000;
}
// *********************************************************************************************** //
void osd_systick_continue(void)
{
*NVIC->ISER=0x00040000;
}
// *********************************************************************************************** //
void osd_irq_stop(void)
{
	HAL_COMP_Stop(&hcomp1);
	HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
	HAL_DAC_Stop(&hdac1, DAC_CHANNEL_2);
}
// *********************************************************************************************** //
static S32 scanline=0;
static U32 field;
static U32 field_sync_count=0;
static U32 start_scanline;
// *********************************************************************************************** //
void osd_irq_short(U32 delta)
{
if (field_sync_count==0)
	{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);

	if (scanline==254) field=1;      // NTSC;
	else if (scanline==253) field=0; // NTSC;
	else if (scanline!=305) field=1;//field^=1; // unknown format

	if (field) stats.field1_line_count=scanline;
	else stats.field2_line_count=scanline;

	scanline=0;

	if (!field)
		{
		stats.cpu_ticks=stats.cpu_ticks_sum;stats.cpu_ticks_sum=0;
		stats.irq_ticks=stats.irq_ticks_sum;stats.irq_ticks_sum=0;
		}

	start_scanline=((registers[REG_7456_VOS])+1);
	}

field_sync_count++;

osd_signal&=osd_signal_clr;
osd_signal_clr=-1;
return;
}
// *********************************************************************************************** //
static U32 out_row=0;
static U32 out_line=0;
// *********************************************************************************************** //
void osd_irq_long(U32 delta)
{
extern void output_charline(U32,U32,U32);
static S32 line=0;
register S32 t;

if (scanline>start_scanline)
	{
//	t=registers[REG_7456_HOS]; while(t--) {}  // Optimizer -O0
	t=registers[REG_7456_HOS]; while(t--) {__asm__(" nop ");__asm__(" nop ");} // optimizer -O2


	if (line<end_line)
		{
		output_charline(out_row,out_line,DISPLAY_WIDTH);
		out_line++;
		if (out_line>=FONT_HEIGHT)
			{
			out_line=0;
			out_row++;
			}
		}
	else if (line==end_line)
		{
		out_row++;
		if (field) osd_signal|=(SIG_FRAME_END|SIG_FIELD_END);else osd_signal|=SIG_FIELD_END;
		}
	line++;
	}
else
	{
	//-------------------------------------------//]
	//-- Housekeeping                          --//
	//-------------------------------------------//
	if (field_sync_count) // first scanline
		{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);

		if (stats.field1_line_count==305) // pal
			{
			if (field_sync_count==12) {field=0;}
			else {field=1;}
			end_line=PAL_LINES;
			}
		else  end_line=NTSC_LINES;

		if (field) stats.field1_sync_count=field_sync_count;
		else stats.field2_sync_count=field_sync_count;
		field_sync_count=0;
		}

	if (scanline==start_scanline)
		{
		if (!field) osd_signal|=(SIG_FRAME_START|SIG_FIELD_START);
		else osd_signal|=SIG_FIELD_START;
		line=0;
		out_row=display_start_row;
		out_line=0;
		}
	}
scanline++;
}
// *********************************************************************************************** //

// *********************************************************************************************** //
__attribute__ ((section (".text.comp_irq")))  void osd_comp_irq(void)
{
	extern void osd_irq_short(U32 delta);
	extern void osd_irq_long(U32 delta);
	register S16 t;
	TIM_TypeDef *timer;

	t=0;
#if OSD_SIGNAL_GENERATOR
	if (EXTI->RPR1&0x40000) {t=TIM2->CCR2; timer=TIM2; stats.gen_cap_ticks=t;}
	else stats.gen_cap_ticks=1;
#endif
	if (EXTI->RPR1&0x20000) {t=TIM3->CCR1; timer=TIM3; stats.vid_cap_ticks=t;}
	else stats.vid_cap_ticks=1;

	stats.cpu_ticks_sum+=t;
// clear osd_signal bits
	osd_signal&=osd_signal_clr;
	osd_signal_clr=-1;

	if (t>LINE_TICK_MIN && t<LINE_TICK_MAX)	osd_irq_long(t);
	else if (t>SYNC_TICK_MIN && t<SYNC_TICK_MAX) osd_irq_short(t);
	else GPIOB->ODR=mixmux[0][0];  // insure pass though video on errors.

	EXTI->RPR1=0x60000;
	stats.irq_ticks_sum+=timer->CNT;
	return;
}
// *********************************************************************************************** //
#if 0
// *********************************************************************************************** //
// MODIFY this function in core/stm32g0xx_it.c
// *********************************************************************************************** //
void ADC1_COMP_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_COMP_IRQn 0 */
	extern void osd_comp_irq(void);
	osd_comp_irq();
#if 0
  /* USER CODE END ADC1_COMP_IRQn 0 */
  HAL_COMP_IRQHandler(&hcomp1);
  HAL_COMP_IRQHandler(&hcomp2);
  /* USER CODE BEGIN ADC1_COMP_IRQn 1 */
#endif
  /* USER CODE END ADC1_COMP_IRQn 1 */
}
// *********************************************************************************************** //
#endif
// *********************************************************************************************** //
