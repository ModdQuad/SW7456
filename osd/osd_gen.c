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
 * osd_gen.c
 *
 *  Created on: Sep 24, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#include "osd_gen.h"
#include "osd_mixmux.h"
#include "osd_registers.h"
// *********************************************************************************************** //
#if OSD_SIGNAL_GENERATOR
// *********************************************************************************************** //
extern TIM_HandleTypeDef htim16;
extern DMA_HandleTypeDef hdma_tim16_ch1;
extern COMP_HandleTypeDef hcomp1;
extern COMP_HandleTypeDef hcomp2;
// *********************************************************************************************** //
#define LINE_FREQ_NTSC ((GEN_TIMER_CLOCK_FREQ/NTSC_LINE_FREQ+0.5)/2)
#define LINE_FREQ_PAL  ((GEN_TIMER_CLOCK_FREQ/PAL_LINE_FREQ+0.5)/2)
// *********************************************************************************************** //
#define SYNC  (NTSC_HSYNC_TIME*GEN_TIMER_CLOCK_FREQ+0.5)
#define HSYNC ((SYNC/2)+0.5)
#define LSYNC (SYNC*5.65)
// *********************************************************************************************** //
const U16 black_burst_pal[] __attribute__ ((aligned (4))) =
	{
	LSYNC,LSYNC,LSYNC,LSYNC,LSYNC,HSYNC,HSYNC,HSYNC,HSYNC,HSYNC,
	/*  00 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/*  25 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/*  50 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/*  75 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 100 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 125 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 150 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 175 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 200 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 225 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 250 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 275 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 300 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0, SYNC,0,
	HSYNC,HSYNC,HSYNC,HSYNC,HSYNC,//HSYNC,
	LSYNC,LSYNC,LSYNC,LSYNC,LSYNC,HSYNC,HSYNC,HSYNC,HSYNC,//HSYNC,
	/*  00 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/*  25 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/*  50 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/*  75 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 100 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 125 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 150 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 175 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 200 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 225 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 250 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 275 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
	/* 300 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0, SYNC,0,
	HSYNC,HSYNC,HSYNC,HSYNC,HSYNC,HSYNC
	};
// *********************************************************************************************** //
const U16 black_burst_ntsc[] __attribute__ ((aligned (4))) =
	{
	// Field 1
		/* 1-9 */ HSYNC,HSYNC,HSYNC,HSYNC,HSYNC,HSYNC, LSYNC,LSYNC,LSYNC,LSYNC,LSYNC,LSYNC, HSYNC,HSYNC,HSYNC,HSYNC,HSYNC,HSYNC,
		// hsync,0 * 253
		/*  10 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/*  35 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/*  60 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/*  85 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 110 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 135 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 160 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 185 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 210 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 235 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 260 */ SYNC,0,SYNC,0,SYNC,0,
		/* 263 */ SYNC,HSYNC,
	// Field 2
		/* 1 */   HSYNC,HSYNC,HSYNC,HSYNC,HSYNC, LSYNC,LSYNC,LSYNC,LSYNC,LSYNC,LSYNC,  HSYNC,HSYNC,HSYNC,HSYNC,HSYNC,HSYNC, 0,
		// hsync,0 * 253
		/*  10 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/*  35 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/*  60 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/*  85 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 110 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 135 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 160 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 185 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 210 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 235 */ SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,SYNC,0,
		/* 260 */ SYNC,0,SYNC,0,SYNC,0
	};
// *********************************************************************************************** //
U16 black_burst[1250]  __attribute__ ((aligned (4)));
// *********************************************************************************************** //
U32 setup_black_burst(void)
{
U32 x;
U32 size;

if (registers[REG_7456_VM0]&R_7456_VM0_VSS)
	{
	size=sizeof(black_burst_pal)/2;
	for (x=0;x<size;x++) black_burst[x]=black_burst_pal[x];
	TIM16->ARR=LINE_FREQ_PAL;
	}
else
	{
	size=sizeof(black_burst_ntsc)/2;
	for (x=0;x<size;x++) black_burst[x]=black_burst_ntsc[x];
	TIM16->ARR=LINE_FREQ_NTSC;
	}

return(size);
}
// *********************************************************************************************** //
U32 generator_on;
// *********************************************************************************************** //
void osd_gen_init(void)
{
HAL_TIM_Base_Init(&htim16);
generator_on=0;
}
// *********************************************************************************************** //

// *********************************************************************************************** //
void osd_gen_start(void)
{
U32 size;
if (!generator_on)
	{
	generator_on=1;
	TIM3->CCR1=0;

	HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);

	size=setup_black_burst();
	set_mixmux();

	HAL_COMP_Start(&hcomp2);
	HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
	HAL_DMA_Start(&hdma_tim16_ch1,(U32)black_burst,(U32)&TIM16->CCR1,size);
	TIM16->DIER=0x0600;

	GPIOB->ODR=mixmux[0][0];
	}
}
// *********************************************************************************************** //
void osd_gen_stop(void)
{
if (generator_on)
	{
	generator_on=0;
	HAL_COMP_Stop(&hcomp2);
	HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
	HAL_DMA_Abort(&hdma_tim16_ch1);
	set_mixmux();

	GPIOB->ODR=mixmux[0][0];
	}
}
// *********************************************************************************************** //
void osd_gen_check(void)
{

if (!generator_on)
	{
	if (TIM3->CNT>0x08000) osd_gen_start();
	}

if (generator_on)
	{
	if (TIM3->CCR1>LINE_TICK_MIN && TIM3->CCR1<LINE_TICK_MAX) osd_gen_stop();
	}
}
// *********************************************************************************************** //
#endif
// *********************************************************************************************** //
