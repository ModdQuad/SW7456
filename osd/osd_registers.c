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
 * osd_registers.c
 *
 *  Created on: Oct 21, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#include "osd_registers.h"
#include "osd_mixmux.h"
#include "osd_spi.h"
#include "osd_irq.h"
#include "osd_font.h"
// *********************************************************************************************** //
#define __DEBUG__ 0
// *********************************************************************************************** //
#if __DEBUG__
U8 spi_debug[512];
S32 sdi;

void spi_debug_trigger(U8 addr,U8 data)
{
if (sdi>20) sdi=0;
spi_debug[sdi++]=0xF0;
spi_debug[sdi++]=addr;
spi_debug[sdi++]=data;
spi_debug[sdi++]=0xF0;
}
#else
void spi_debug_trigger(U8 addr,U8 data) {}
#endif
// *********************************************************************************************** //
const U8 register_default[] = {
	0x00,0x47,0x20,0x10,0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x1B,0x00,0x00,0x00, // 0x00 - 0x0F
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01, // 0x10 - 0x1F
	};
// *********************************************************************************************** //
// *********************************************************************************************** //
U8 registers[32];
// *********************************************************************************************** //
// *********************************************************************************************** //
static U32 _index_;
static const S32 offset=(COPYRIGHT_LINES*DISPLAY_WIDTH);
// *********************************************************************************************** //
// *********************************************************************************************** //
void write_register_defaults(void)
{
U32 x;
for (x=0;x<32;x++) registers[x]=register_default[x];
}
// *********************************************************************************************** //
static void _decode_outattr(U32 c)
{
if (registers[REG_7456_VM1]&R_7456_VM1_BM) c|=0x080;
character_attribute[offset+_index_]=((c>>3)&0x1C);
}
// *********************************************************************************************** //
static void _decode_outchar(U32 c)
{
character_address[offset+_index_]=&font[FONT_HEIGHT*c];
if (registers[REG_7456_VM1]&R_7456_VM1_BM) character_attribute[offset+_index_]|=0x10;
}
// *********************************************************************************************** //
// *********************************************************************************************** //
void osd_decode_spi_registers(U8 data)
{
static U8 spi_state=0;
static U8 spi_addr=0;
U8 last;

#if __DEBUG__
if (sdi<512) spi_debug[sdi++]=data;
#endif

switch (spi_state)
	{
	case 0: // wait for address
		spi_addr=data;
		if (spi_addr&0x80) spi_state=2; // unsupported read
		else spi_state=1;
		break;

	case 1:
		spi_state=0;
		if (spi_addr<0x20)
			{
			last=registers[spi_addr];
			registers[spi_addr]=data;

			switch(spi_addr)
				{
				case REG_7456_DMDI:
					if (data==0xFF && (registers[REG_7456_DMM]&R_7456_DMM_AIM)) registers[REG_7456_DMM]&=(~(R_7456_DMM_AIM));
					else
						{
						if (registers[REG_7456_DMM]&R_7456_DMM_OMS)
							{
							if (registers[REG_7456_DMAH]&R_7456_DMAH_BSB) _decode_outattr(data);
							else _decode_outchar(data);
							}
						else {_decode_outchar(data); _decode_outattr((registers[REG_7456_DMM]&0x038)<<2);}

						if (registers[REG_7456_DMM]&R_7456_DMM_AIM) {_index_++; _index_&=CHAR_BUFF_MASK;}
						}
					break;

				case REG_7456_VM0:
					if ((last^data)&R_7456_VM0_SSM) ; // always auto select, should fix but lazy.

//					if ((last^data)&R_7456_VM0_ENO) osd_outirq_continue(); // may work, untested
//					else osd_outirq_pause();

					if (data&R_7456_VM0_RST)
						{
//						osd_outirq_pause();
						osd_cls();
						write_register_defaults();
						set_mixmux();
						}
					break;

				case REG_7456_VM1:
					if ((last^data)&R_7456_VM1_BMB) set_mixmux();
					break;

				case REG_7456_HOS:
					registers[REG_7456_HOS]=data&R_7456_HOS;
					break;

				case REG_7456_VOS:
					registers[REG_7456_VOS]=data&R_7456_VOS;
					break;

				case REG_7456_DMAH: //Display Memory Address high
					_index_=((data&1)<<8)|(_index_&0x00FF);
					break;

				case REG_7456_DMAL: //Display Memory Address Low
					_index_=(_index_&0x0100)|data;
					break;

				case REG_7456_RB0: // Row 0 is used to set the brightness for ALL rows, individual row brightness not supported.
					if (last^data)	set_mixmux();
					break;

				case REG_7456_DMM: //Display Memory mode
					// there are issues with flickering using betaflight and the CLS bit.
					// the timing of when the clear screen occurs differs from the actual part
#if OSD_DISABLE_CLS_FOR_BETAFLIGHT

#else
					if (data&R_7456_DMM_CDM)
						{
						registers[REG_7456_DMM]&=(~R_7456_DMM_CDM);
						osd_cls();
						}
#endif
					break;
				}
			}
		break;

	case 2:
		// Ignore 7456 SPI reads, they can not be supported.
		// SPI always reads 0x00 or 0xFF based on the pull-up configuration.
		// a zero should follow a read command, used to resync.
		if (data==0x00) spi_state=0;

		spi_debug_trigger(spi_addr,data);
		break;

	default:
		spi_state=0;
	}
}
// *********************************************************************************************** //
// *********************************************************************************************** //
