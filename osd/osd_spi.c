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
 * osd_spi.c
 *
 *  Created on: Sep 24, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#include "main.h"
#include "osd_spi.h"
#include "osd_registers.h"
// *********************************************************************************************** //
extern SPI_HandleTypeDef hspi1;
// *********************************************************************************************** //
U8 spi_data[SPI_BUFF_SIZE] ; // __attribute__ ((section (".spi_buffer")));
// *********************************************************************************************** //
void osd_spi_start(void)
{
HAL_SPI_Receive_DMA(&hspi1,(uint8_t *)spi_data,SPI_BUFF_SIZE);
}
// *********************************************************************************************** //
void osd_spi_stop(void)
{
// unused
}
// *********************************************************************************************** //
void osd_spi_process(void)
{
static U32 x_last=0;
U32 x;
U32 *CNDTR2=(U32 *)(0x0020 + ((int)DMA1));

x=(SPI_BUFF_SIZE-*CNDTR2)&SPI_BUFF_MASK;

while (x!=x_last)
	{
	osd_decode_spi_registers(spi_data[x_last]);
	x_last++; x_last&=SPI_BUFF_MASK;
	}
}
// *********************************************************************************************** //
