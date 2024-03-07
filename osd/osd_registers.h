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
 * osd_registers.h
 *
 *  Created on: Oct 21, 2019
 *      Author: todd
 */
// *********************************************************************************************** //
#ifndef OSD_REGISTERS_H_
#define OSD_REGISTERS_H_
// *********************************************************************************************** //
#include "portable.h"
// *********************************************************************************************** //
extern U8 registers[];
// *********************************************************************************************** //
extern void write_register_defaults(void);
extern void osd_decode_spi_registers(U8 data);
// *********************************************************************************************** //
#define REG_7456_VM0   0x00  //Video Mode 0
#define R_7456_VM0_VSS   (1<<6)  // Video Standard Select 0 = NTSC 1 = PAL
#define R_7456_VM0_SSM   (3<<4)  // Sync Select Mode (Table 1) 0x = Autosync select 10 = External 11 = Internal
#define R_7456_VM0_ENO   (1<<3)  // Enable Display of OSD Image 0 = Off 1 = On
#define R_7456_VM0_VSY   (1<<2)  // Vertical Synchronization of On-Screen Data 0 = Enable on-screen display immediately 1 = Enable on-screen display at the next VSYNC
#define R_7456_VM0_RST   (1<<1)  // Software Reset Bit
#define R_7456_VM0_VBE   (1<<0)  // Video Buffer Enable 0 = Enable 1 = Disable (VOUT is high impedance)
// *********************************************************************************************** //
#define REG_7456_VM1   0x01  // Video Mode 1
#define R_7456_VM1_BM    (1<<7)  // Background Mode
#define R_7456_VM1_BMB   (7<<4)  // Background Mode Brightness
#define R_7456_VM1_BT    (3<<2)  // Blinking Time
#define R_7456_VM1_BDC   (3<<0)  // Blinking Duty Cycle
// *********************************************************************************************** //
#define REG_7456_HOS   0x02  // Horizontal Offset
#define R_7456_HOS      (63<<0)  // Horizontal Position Offset, default 32
// *********************************************************************************************** //
#define REG_7456_VOS   0x03  // Vertical Offset
#define R_7456_VOS      (31<<0)  // Vertical Position Offset, default 16
// *********************************************************************************************** //
#define REG_7456_DMM   0x04  // Display Memory Mode
#define R_7456_DMM_OMS    (1<<6)  // Operation Mode Selection   0 = 16-bit operation mode   1 = 8-bit operation mode
#define R_7456_DMM_LBC    (1<<5)  // Local Background Control Bit
#define R_7456_DMM_BB     (1<<4)  // Blink Bit
#define R_7456_DMM_IB     (1<<3)  // Invert Bit
#define R_7456_DMM_CDM    (1<<2)  // Clear Display Memory
#define R_7456_DMM_VSC    (1<<1)  // Vertical Sync Clear
#define R_7456_DMM_AIM    (1<<0)  // Auto-Increment Mode   1 = Enabled
// *********************************************************************************************** //
#define REG_7456_DMAH  0x05  // Display Memory Address High
#define R_7456_DMAH_BSB  (1<<1)  // Byte Selection Bit 0 = Character Address 1 = Character Attribute byte
#define R_7456_DMAH_B8   (1<<0)  // Display Memory Address Bit 8
// *********************************************************************************************** //
#define REG_7456_DMAL  0x06  // Display Memory Address Low
#define R_7456_DMAL      (0xFF)
// *********************************************************************************************** //
#define REG_7456_DMDI  0x07  // Display Memory Data In
#define R_7456_DMDI_DATA (0xFF) // Write data memory
// *********************************************************************************************** //
#define REG_7456_CMM   0x08  // Character Memory Mode            (NOT SUPPORTED)
#define REG_7456_CMAH  0x09  // Character Memory Address High    (NOT SUPPORTED)
#define REG_7456_CMAL  0x0a  // Character Memory Address Low     (NOT SUPPORTED)
#define REG_7456_CMDI  0x0b  // Character Memory Data In         (NOT SUPPORTED)
#define REG_7456_OSDM  0x0c  // OSD Insertion Mux                (NOT SUPPORTED)
// *********************************************************************************************** //
// *********************************************************************************************** //
#define REG_7456_RB0   0x10  // Row 0 Brightness this one applies to all rows
#define REG_7456_RB1   0x11  // Row 0 Brightness  (NOT SUPPORTED)
#define REG_7456_RB2   0x12  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB3   0x13  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB4   0x14  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB5   0x15  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB6   0x16  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB7   0x17  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB8   0x18  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB9   0x19  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB10  0x1a  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB11  0x1b  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB12  0x1c  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB13  0x1d  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB14  0x1e  // Row 0 Brightness (NOT SUPPORTED)
#define REG_7456_RB15  0x1f  // Row 15 Brightness (NOT SUPPORTED)
#define R_7456_RB_BL     (3<<2)  // Row Black Level
#define R_7456_RB_WL     (3<<0)  // Row White Level
// *********************************************************************************************** //
#define REG_7456_OSDBL 0x6c  // OSD Black Level   (NOT SUPPORTED)
// *********************************************************************************************** //
#endif /* OSD_REGISTERS_H_ */
