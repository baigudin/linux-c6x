;
;  linux/arch/c6x/lib/memcpy.s
;
;  Port on Texas Instruments TMS320C6x architecture
;
;  Copyright (C) 2005, 2009 Texas Instruments Incorporated
;  Author: Aurelien Jacquiot (aurelien.jacquiot@jaluna.com)
;
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License version 2 as
;  published by the Free Software Foundation.
;

	.global  memcpy
	.text

memcpy:
	AND	.L1	0x1,A6,A0
 ||	AND	.S1	0x2,A6,A1
 ||	AND	.L2	0x4,A6,B0
 ||	MV	.D1	A4,A3

   [A0] LDB	.D2	*B4++,A5
   [A1]	LDB	.D2	*B4++,A7
   [A1] LDB	.D2	*B4++,A8
   [B0] LDNW	.D2	*B4++,A9
 || 	SHRU	.S2	A6,0x3,B1
	CMPEQ	.L2	0x1,B1,B2
||[!B1]	BNOP	.S2	B3,1

   [A0]	STB	.D1	A5,*A3++
   [A1]	STB	.D1	A7,*A3++
   [A1]	STB	.D1	A8,*A3++
   [B0]	STNW	.D1	A9,*A3++	; return when len < 8

	LDNDW	.D2	*B4++,A9:A8	; load first one
 ||[B2] B	.S2	B3
	NOP		4
   [B2]	STNDW	.D1	A9:A8,*A3++	; return when len = 8
 ||	SUB	.L2	B1,0x2,B1

COPY_LOOP:
	BDEC	.S2	COPY_LOOP,B1
	STNDW	.D1	A9:A8,*A3++
	LDNDW	.D2	*B4++,A9:A8
	NOP		3

	BNOP	.S2	B3,5
