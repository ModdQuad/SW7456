#
# This file is part of SW7456.
#
# SW7456 is free software. You can redistribute this software and/or modify this software
# under the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later version.
#
# SW7456 is distributed in the hope that they will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this software.
#
# If not, see <http://www.gnu.org/licenses/>.
#
# ----------------------------------------------------------------------------------------
        .global output_charline
# ----------------------------------------------------------------------------------------
		.extern osd_port
		.extern character_address
        .extern mixmux
		.extern character_attribute
		.extern display_start
#--------------------------------------
.syntax             unified
#--------------------------------------
//    .section .RamFunc
//	.align 8
//	.align 32
#--------------------------------------
    .section .text.first
	.align 8
#--------------------------------------
output_charline:
	push    {r4, r5, r6, r7, lr}
	mov     r4,r8
	mov     r5,r9
	mov     r6,r10
	mov     r7,r11
	push    {r4,r5,r6,r7}

// R0 = row
// R1 = line
// R2 = DISPLAY_WIDTH

	muls    r0,r2

	ldr     r6,=character_attribute
	adds	r6,r0

	lsls	r0,r0,#2
	ldr     r7,=character_address
	adds	r7,r0

	lsls    r5, r2, #2
	adds	r5,r7
	adds	r5,#4
	mov     r11,r5

	lsls    r5, r1, #2
	mov     r10,r5
#--------------------------------------
	ldr		r0,=osd_port
	ldr     r0,[r0]
	ldr     r2,=mixmux
#--------------------------------------
//					R0 = PORT
//					R1 = temp
//					R2 = mixnux address
//					R3 = character bits
//					R4 = attribute bits
#--------------------------------------
//	R5 = temp
//	R6 = character_attribute
//	R7 = character address
//	R8 = next character bits
//	R9 = next attribute
//	R10 = out_line * 4
//  R11 = last character address
#--------------------------------------
	ldrb    r4, [r6]

	ldr     r3, [r7]
	add		r3, r10
	ldr     r3, [r3]

	adds	r6 ,#1
	adds	r7 ,#4
#--------------------------------------
# some pixels are bigger than others because some pixels mothers...
#--------------------------------------
pixel_loop:
#-- Pixel 1 --
					lsls    r1, r3, #22
					lsrs    r1, r1, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
					strb    r1, [r0]
#-- Pixel 2 --
	ldr     r5, [r7]
					lsls    r1, r3, #18
					lsrs    r1, r1, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
					strb    r1, [r0]
#-- Pixel 3 --
	add		r5,r10
	nop
					lsls    r1, r3, #14
					lsrs    r1, r1, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
					strb    r1, [r0]
#-- Pixel 4 --
	ldr	    r5, [r5]
					lsls    r1, r3, #10
					lsrs    r1, r1, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
					strb    r1, [r0]
#-- Pixel 5 --
	mov r8,r5
	nop
					lsls    r1, r3, #6
					lsrs    r1, r1, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
					strb    r1, [r0]
#-- Pixel 6 --
	nop
	nop
					lsls    r1, r3, #2
					lsrs    r1, r1, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
					strb    r1, [r0]
#-- Pixel 7 --
//	nop
//	nop
	ldrb    r5, [r6]

					lsls    r1, r3, #20
					lsrs    r1, r1, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
					strb    r1, [r0]
#-- Pixel 8 --
	mov r9,r5
	nop
					lsls    r1, r3, #16
					lsrs    r1, r1, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
					strb    r1, [r0]
#-- Pixel 9 --
	nop
	nop
					lsls    r1, r3, #12
					lsrs    r1, r1, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
					strb    r1, [r0]
#-- Pixel 10 --
	adds	r6 ,#1
	nop
					lsls    r1, r3, #8
					lsrs    r1, r1, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
					strb    r1, [r0]
#-- Pixel 11 --
	adds	r7 ,#4
	nop
					lsls    r1, r3, #4
					lsrs    r1, r1, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
					strb    r1, [r0]
#-- Pixel 12 --
					lsrs    r1, r3, #30
					eors    r1, r4
					ldrb    r1, [r2,r1]
	cmp     r7,r11
	mov		r3,r8
	mov		r4,r9
					strb    r1, [r0]
	bne		pixel_loop // 2 cycles branch taken + I BUS access
#=======================================================
	ldrb    r1, [r2] // load transparent

	pop     {r4,r5,r6,r7}
	mov     r8 ,r4
	mov     r9 ,r5
	mov     r10,r6
	mov     r11,r7
	strb    r1, [r0] // output transparent

	pop     {r4, r5, r6, r7, pc}
#--------------------------------------
#--------------------------------------
