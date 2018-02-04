//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include "cpuIA32_Register.h"
#include "cpuIA32_Operand.h"


namespace Giblet { namespace CodeHook { namespace x86
{

	//	Opcode extentions used in MODRM
	namespace PO
	{

		const uint8_t REGISTER	=	0xfe;
		const uint8_t NONE		=	0xff;

		namespace x80
		{
			const uint8_t	ADD8i8		=	0x00;	//	80 /0 ib ADD r/m8,imm8 Add imm8 to r/m8
			const uint8_t	OR8i8		=	0x01;	//	80 /1 ib  OR r/m8,imm8 r/m8 OR imm8
			const uint8_t	ADC8i8		=	0x02;	//	80 /2 ib ADC r/m8, imm8 Add with carry imm8 to r/m8
			const uint8_t	SBB8i8		=	0x03;	//	80 /3 ib SBB r/m8,imm8 Subtract with borrow imm8 from r/m8
			const uint8_t	AND8i8		=	0x04;	//	80 /4 ib AND r/m8,imm8 r/m8 AND imm8
			const uint8_t	SUB8i8		=	0x05;	//	80 /5 ib SUB r/m8,imm8 Subtract imm8 from r/m8
			const uint8_t	XOR8i8		=	0x06;	//	80 /6 ib XOR r/m8,imm8 r/m8 XOR imm8
			const uint8_t	CMP8i8		=	0x07;	//	80 /7 ib CMP r/m8, imm8 Compare imm8 with r/m8
		};

		namespace x81
		{
			const uint8_t	ADDi		=	0x00;	//	81 /0 iw ADD r/m16,imm16 Add imm16 to r/m16
													//	81 /0 id ADD r/m32,imm32 Add imm32 to r/m32
			const uint8_t	ORi			=	0x01;	//	81 /1 iw  OR r/m16,imm16 r/m16 OR imm16
													//	81 /1 id  OR r/m32,imm32 r/m32 OR imm32
			const uint8_t	ADCi		=	0x02;	//	81 /2 iw ADC r/m16,imm16 Add with carry imm16 to r/m16
													//	81 /2 id ADC r/m32,imm32 Add with CF imm32 to r/m32
			const uint8_t	SBBi		=	0x03;	//	81 /3 iw SBB r/m16,imm16 Subtract with borrow imm16 from r/m16
													//	81 /3 id SBB r/m32,imm32 Subtract with borrow imm32 from r/m32
			const uint8_t	ANDi		=	0x04;	//	81 /4 iw AND r/m16,imm16 r/m16 AND imm16
													//	81 /4 id AND r/m32,imm32 r/m32 AND imm32
			const uint8_t	SUBi		=	0x05;	//	81 /5 iw SUB r/m16,imm16 Subtract imm16 from r/m16
													//	81 /5 id SUB r/m32,imm32 Subtract imm32 from r/m32
			const uint8_t	XORi		=	0x06;	//	81 /6 iw XOR r/m16,imm16 r/m16 XOR imm16
													//	81 /6 id XOR r/m32,imm32 r/m32 XOR imm32
			const uint8_t	CMPi		=	0x07;	//	81 /7 iw CMP r/m16, imm16 Compare imm16 with r/m16
													//	81 /7 id CMP r/m32,imm32 Compare imm32 with r/m32
		};

		namespace x83
		{
			const uint8_t	ADDi8		=	0x00;	//	83 /0 ib ADD r/m16,imm8 Add sign-extended imm8 to r/m16
													//	83 /0 ib ADD r/m32,imm8 Add sign-extended imm8 to r/m32
			const uint8_t	ORi8		=	0x01;	//	83 /1 ib  OR r/m16,imm8 r/m16 OR imm8 (sign-extended)
													//	83 /1 ib  OR r/m32,imm8 r/m32 OR imm8 (sign-extended)
			const uint8_t	ADCi8		=	0x02;	//	83 /2 ib ADC r/m16,imm8 Add with CF sign-extended imm8 to r/m16
													//	83 /2 ib ADC r/m32,imm8 Add with CF sign-extended imm8 into r/m32
			const uint8_t	SBBi8		=	0x03;	//	83 /3 ib SBB r/m16,imm8 Subtract with borrow sign-extended imm8 from r/m16
													//	83 /3 ib SBB r/m32,imm8 Subtract with borrow sign-extended imm8 from r/m32
			const uint8_t	ANDi8		=	0x04;	//	83 /4 ib AND r/m16,imm8 r/m16 AND imm8 (sign-extended)
													//	83 /4 ib AND r/m32,imm8 r/m32 AND imm8 (sign-extended)
			const uint8_t	SUBi8		=	0x05;	//	83 /5 ib SUB r/m16,imm8 Subtract sign-extended imm8 from r/m16
													//	83 /5 ib SUB r/m32,imm8 Subtract sign-extended imm8 from r/m32
			const uint8_t	XORi8		=	0x06;	//	83 /6 ib XOR r/m16,imm8 r/m16 XOR imm8 (sign-extended)
													//	83 /6 ib XOR r/m32,imm8 r/m32 XOR imm8 (sign-extended)
			const uint8_t	CMPi8		=	0x07;	//	83 /7 ib CMP r/m16,imm8 Compare imm8 with r/m16
													//	83 /7 ib CMP r/m32,imm8 Compare imm8 with r/m32
		};

		namespace x8F
		{
			const uint8_t	POP			=	0x00;	//	8F /0 POP m16 Pop top of stack into m16; increment stack pointer
													//	8F /0 POP m32 Pop top of stack into m32; increment stack pointer
		};

		namespace xC6
		{
			const uint8_t	MOV_I8		=	0x00;	//	C6 / 0 MOV r/m8,imm8 Move imm8 to r/m8
		};

		namespace xC7
		{
			const uint8_t	MOV			=	0x00;	//	C7 / 0 MOV r/m16,imm16 Move imm16 to r/m16
													//	C7 / 0 MOV r/m32,imm32 Move imm32 to r/m32
		};

		namespace xC0
		{
			const uint8_t	ROR8i8		=	0x01;	//	C0 /1 ib ROR r/m8,imm8 Rotate 8 bits r/m16 right imm8 times
			const uint8_t	RCL8i8		=	0x02;	//	C0 /2 ib RCL r/m8,imm8 Rotate 9 bits (CF, r/m8) left imm8 times
			const uint8_t	RCR8i8		=	0x03;	//	C0 /3 ib RCR r/m8,imm8 Rotate 9 bits (CF, r/m8) right imm8 times
			const uint8_t	SAL8i8		=	0x04;	//	C0 /4 ib SAL r/m8,imm8 Multiply r/m8 by 2, imm8 times
													//	C0 /4 ib SHL r/m8,imm8 Multiply r/m8 by 2, imm8 times
			const uint8_t	SHR8i8		=	0x05;	//	C0 /5 ib SHR r/m8,imm8 Unsigned divide r/m8 by 2, imm8 times
			const uint8_t	SAR8i8		=	0x07;	//	C0 /7 ib SAR r/m8,imm8 Signed divide* r/m8 by 2, imm8 times
		};

		namespace xC1
		{
			const uint8_t	ROLi8		=	0x00;	//	C1 /0 ib ROL r/m16,imm8 Rotate 16 bits r/m16 left imm8 times
													//	C1 /0 ib ROL r/m32,imm8 Rotate 32 bits r/m32 left imm8 times
			const uint8_t	RORi8		=	0x01;	//	C1 /1 ib ROR r/m16,imm8 Rotate 16 bits r/m16 right imm8 times
													//	C1 /1 ib ROR r/m32,imm8 Rotate 32 bits r/m32 right imm8 times
			const uint8_t	RCLi8		=	0x02;	//	C1 /2 ib RCL r/m16,imm8 Rotate 17 bits (CF, r/m16) left imm8 times
													//	C1 /2 ib RCL r/m32,imm8 Rotate 33 bits (CF, r/m32) left imm8 times
			const uint8_t	RCRi8		=	0x03;	//	C1 /3 ib RCR r/m16,imm8 Rotate 17 bits (CF, r/m16) right imm8 times
													//	C1 /3 ib RCR r/m32,imm8 Rotate 33 bits (CF, r/m32) right imm8 times
			const uint8_t	SALi8		=	0x04;	//	C1 /4 ib SAL r/m16,imm8 Multiply r/m16 by 2, imm8 times
													//	C1 /4 ib SHL r/m16,imm8 Multiply r/m16 by 2, imm8 times
													//	C1 /4 ib SAL r/m32,imm8 Multiply r/m32 by 2, imm8 times
													//	C1 /4 ib SHL r/m32,imm8 Multiply r/m32 by 2, imm8 times
			const uint8_t	SHRi8		=	0x05;	//	C1 /5 ib SHR r/m16,imm8 Unsigned divide r/m16 by 2, imm8 times
													//	C1 /5 ib SHR r/m32,imm8 Unsigned divide r/m32 by 2, imm8 times
			const uint8_t	SARi8		=	0x07;	//	C1 /7 ib SAR r/m16,imm8 Signed divide* r/m16 by 2, imm8 times
													//	C1 /7 ib SAR r/m32,imm8 Signed divide* r/m32 by 2, imm8 times
		};

		namespace xD0
		{
			const uint8_t	ROL8		=	0x00;	//	D0 /0 ROL r/m8,1 Rotate 8 bits r/m8 left once
			const uint8_t	ROR8		=	0x01;	//	D0 /1 ROR r/m8,1 Rotate 8 bits r/m8 right once
			const uint8_t	RCL8		=	0x02;	//	D0 /2 RCL r/m8,1 Rotate 9 bits (CF, r/m8) left once
			const uint8_t	RCR8		=	0x03;	//	D0 /3 RCR r/m8,1 Rotate 9 bits (CF, r/m8) right once
			const uint8_t	SAL8		=	0x04;	//	D0 /4 SAL r/m8,1 Multiply r/m8 by 2, once
													//	D0 /4 SHL r/m8,1 Multiply r/m8 by 2, once
			const uint8_t	SHR8		=	0x05;	//	D0 /5 SHR r/m8,1 Unsigned divide r/m8 by 2, once
			const uint8_t	SAR8		=	0x07;	//	D0 /7 SAR r/m8,1 Signed divide* r/m8 by 2, once
		};

		namespace xD1
		{
			const uint8_t	ROL			=	0x00;	//	D1 /0 ROL r/m16,1 Rotate 16 bits r/m16 left once
													//	D1 /0 ROL r/m32,1 Rotate 32 bits r/m32 left once
			const uint8_t	ROR			=	0x01;	//	D1 /1 ROR r/m16,1 Rotate 16 bits r/m16 right once
													//	D1 /1 ROR r/m32,1 Rotate 32 bits r/m32 right once
			const uint8_t	RCL			=	0x02;	//	D1 /2 RCL r/m16,1 Rotate 17 bits (CF, r/m16) left once
													//	D1 /2 RCL r/m32,1 Rotate 33 bits (CF, r/m32) left once
			const uint8_t	RCR			=	0x03;	//	D1 /3 RCR r/m16,1 Rotate 17 bits (CF, r/m16) right once
													//	D1 /3 RCR r/m32,1 Rotate 33 bits (CF, r/m32) right once
			const uint8_t	SAL			=	0x04;	//	D1 /4 SAL r/m16,1 Multiply r/m16 by 2, once
													//	D1 /4 SAL r/m32,1 Multiply r/m32 by 2, once
													//	D1 /4 SHL r/m16,1 Multiply r/m16 by 2, once
													//	D1 /4 SHL r/m32,1 Multiply r/m32 by 2, once
			const uint8_t	SHR			=	0x05;	//	D1 /5 SHR r/m16,1 Unsigned divide r/m16 by 2, once
													//	D1 /5 SHR r/m32,1 Unsigned divide r/m32 by 2, once
			const uint8_t	SAR			=	0x07;	//	D1 /7 SAR r/m16,1 Signed divide* r/m16 by 2, once
													//	D1 /7 SAR r/m32,1 Signed divide* r/m32 by 2, once
		};

		namespace xD2
		{
			const uint8_t	ROL8cl		=	0x00;	//	D2 /0 ROL r/m8,CL Rotate 8 bits r/m8 left CL times
			const uint8_t	ROR8cl		=	0x01;	//	D2 /1 ROR r/m8,CL Rotate 8 bits r/m8 right CL times
			const uint8_t	RCL8cl		=	0x02;	//	D2 /2 RCL r/m8,CL Rotate 9 bits (CF, r/m8) left CL times
			const uint8_t	RCR8cl		=	0x03;	//	D2 /3 RCR r/m8,CL Rotate 9 bits (CF, r/m8) right CL times
			const uint8_t	SAL8cl		=	0x04;	//	D2 /4 SAL r/m8,CL Multiply r/m8 by 2, CL times
													//	D2 /4 SHL r/m8,CL Multiply r/m8 by 2, CL times
			const uint8_t	SHR8cl		=	0x05;	//	D2 /5 SHR r/m8,CL Unsigned divide r/m8 by 2, CL times
			const uint8_t	SAR8cl		=	0x07;	//	D2 /7 SAR r/m8,CL Signed divide* r/m8 by 2, CL times
		};

		namespace xD3
		{
			const uint8_t	ROLcl		=	0x00;	//	D3 /0 ROL r/m16,CL Rotate 16 bits r/m16 left CL times
													//	D3 /0 ROL r/m32,CL Rotate 32 bits r/m32 left CL times
			const uint8_t	RORcl		=	0x01;	//	D3 /1 ROR r/m16,CL Rotate 16 bits r/m16 right CL times
													//	D3 /1 ROR r/m32,CL Rotate 32 bits r/m32 right CL times
			const uint8_t	RCLcl		=	0x02;	//	D3 /2 RCL r/m16,CL Rotate 17 bits (CF, r/m16) left CL times
													//	D3 /2 RCL r/m32,CL Rotate 33 bits (CF, r/m32) left CL times
			const uint8_t	RCRcl		=	0x03;	//	D3 /3 RCR r/m16,CL Rotate 17 bits (CF, r/m16) right CL times
													//	D3 /3 RCR r/m32,CL Rotate 33 bits (CF, r/m32) right CL times
			const uint8_t	SALcl		=	0x04;	//	D3 /4 SAL r/m16,CL Multiply r/m16 by 2, CL times
													//	D3 /4 SAL r/m32,CL Multiply r/m32 by 2, CL times
													//	D3 /4 SHL r/m16,CL Multiply r/m16 by 2, CL times
													//	D3 /4 SHL r/m32,CL Multiply r/m32 by 2, CL times
			const uint8_t	SHRcl		=	0x05;	//	D3 /5 SHR r/m16,CL Unsigned divide r/m16 by 2, CL times
													//	D3 /5 SHR r/m32,CL Unsigned divide r/m32 by 2, CL times
			const uint8_t	SARcl		=	0x07;	//	D3 /7 SAR r/m16,CL Signed divide* r/m16 by 2, CL times
													//	D3 /7 SAR r/m32,CL Signed divide* r/m32 by 2, CL times
		};

		namespace xF6
		{
			const uint8_t	TEST8i8		=	0x00;	//	F6 /0 ib TEST r/m8,imm8 AND imm8 with r/m8; set SF, ZF, PF according to result
			const uint8_t	NOT8		=	0x02;	//	F6 /2 NOT r/m8 Reverse each bit of r/m8
			const uint8_t	NEG8		=	0x03;	//	F6 /3 NEG r/m8 Two's complement negate r/m8
			const uint8_t	MUL8		=	0x04;	//	F6 /4 MUL r/m8 Unsigned multiply (AX ¬ AL * r/m8)
			const uint8_t	IMUL8		=	0x05;	//	F6 /5 IMUL r/m8 AX¬ AL * r/m byte
			const uint8_t	DIV8		=	0x06;	//	F6 /6 DIV r/m8 Unsigned divide AX by r/m8; AL ¬ Quotient, AH ¬ Remainder
			const uint8_t	IDIV8		=	0x07;	//	F6 /7 IDIV r/m8 Signed divide AX (where AH must contain signextension
													//		of AL) by r/m byte. (Results: AL=Quotient, AH=Remainder)
		};

		namespace xF7
		{
			const uint8_t	TEST		=	0x00;	//	F7 /0 iw TEST r/m16,imm16 AND imm16 with r/m16; set SF, ZF, PF according to result
													//	F7 /0 id TEST r/m32,imm32 AND imm32 with r/m32; set SF, ZF, PF according to result
			const uint8_t	NOT			=	0x02;	//	F7 /2 NOT r/m16 Reverse each bit of r/m16
													//	F7 /2 NOT r/m32 Reverse each bit of r/m32
			const uint8_t	NEG			=	0x03;	//	F7 /3 NEG r/m16 Two's complement negate r/m16
													//	F7 /3 NEG r/m32 Two's complement negate r/m32
			const uint8_t	MUL			=	0x04;	//	F7 /4 MUL r/m16 Unsigned multiply (DX:AX ¬ AX * r/m16)
													//	F7 /4 MUL r/m32 Unsigned multiply (EDX:EAX ¬ EAX * r/m32)
			const uint8_t	IMUL		=	0x05;	//	F7 /5 IMUL r/m16 DX:AX ¬ AX * r/m word
													//	F7 /5 IMUL r/m32 EDX:EAX ¬ EAX * r/m doubleword
			const uint8_t	DIV			=	0x06;	//	F7 /6 DIV r/m16 Unsigned divide DX:AX by r/m16; AX ¬ Quotient, DX ¬ Remainder
													//	F7 /6 DIV r/m32 Unsigned divide EDX:EAX by r/m32 doubleword;
													//		EAX ¬ Quotient, EDX ¬ Remainder
			const uint8_t	IDIV		=	0x07;	//	F7 /7 IDIV r/m16 Signed divide DX:AX (where DX must contain signextension
													//		of AX) by r/m word. (Results: AX=Quotient, DX=Remainder)
													//	F7 /7 IDIV r/m32 Signed divide EDX:EAX (where EDX must contain
													//		sign-extension of EAX) by r/m doubleword.
													//		(Results: EAX=Quotient, EDX=Remainder)
		};

		namespace xFE
		{
			const uint8_t	DEC8		=	0x01;	//	FE /1 DEC r/m8 Decrement r/m8 by 1
		};

		namespace xFF
		{
			const uint8_t	INC			=	0x00;	//	FF /0 INC r/m16 Increment r/m word by 1
													//	FF /0 INC r/m32 Increment r/m doubleword by 1
			const uint8_t	DEC			=	0x01;	//	FF /1 DEC r/m16 Decrement r/m16 by 1
													//	FF /1 DEC r/m32 Decrement r/m32 by 1
			const uint8_t	NCALL		=	0x02;	//	FF /2 CALL r/m16 Call near, absolute indirect, address given in r/m16
													//	FF /2 CALL r/m32 Call near, absolute indirect, address given in r/m32
			const uint8_t	FCALL		=	0x03;	//	FF /3 CALL m16:16 Call far, absolute indirect, address given in m16:16
													//	FF /3 CALL m16:32 Call far, absolute indirect, address given in m16:32
			const uint8_t	JUMPN		=	0x04;	//	FF /4 JMP r/m16 Jump near, absolute indirect, address given in r/m16
													//	FF /4 JMP r/m32 Jump near, absolute indirect, address given in r/m32
			const uint8_t	JUMPF		=	0x05;	//	FF /5 JMP m16:16 Jump far, absolute indirect, address given in m16:16
													//	FF /5 JMP m16:32 Jump far, absolute indirect, address given in m16:32
			const uint8_t	PUSH		=	0x06;	//	FF /6 PUSH r/m16 Push r/m16
													//	FF /6 PUSH r/m32 Push r/m32
		};

		namespace x0F71
		{
			const uint8_t	PSRLW		=	0x02;	//	0F 71 /2 ib PSRLW mm, imm8 Shift words in mm right by imm8.
			const uint8_t	PSRAW		=	0x04;	//	0F 71 /4 ib PSRAW mm, imm8 Shift words in mm right by imm8 while shifting in sign bits
			const uint8_t	PSLLW		=	0x06;	//	0F 71 /6 ib PSLLW mm, imm8 Shift words in mm left by imm8, while shifting in zeros.
		};

		namespace x0F72
		{
			const uint8_t	PSRLD		=	0x02;	//	0F 72 /2 ib PSRLD mm, imm8 Shift doublewords in mm right by imm8.
			const uint8_t	PSRAD		=	0x04;	//	0F 72 /4 ib PSRAD mm, imm8 Shift doublewords in mm right by imm8 while shifting in sign bits
			const uint8_t	PSLLD		=	0x06;	//	0F 72 /6 ib PSLLD mm, imm8 Shift doublewords in mm by imm8, while shifting in zeros.
		};

		namespace x0F73
		{
			const uint8_t	PSRLQ		=	0x02;	//	0F 73 /2 ib PSRLQ mm, imm8 Shift mm right by imm8 while shifting in zeros.
			const uint8_t	PSLLQ		=	0x06;	//	0F 73 /6 ib PSLLQ mm, imm8 Shift mm left by Imm8, while shifting in zeros.
		};

		namespace x0FBA
		{
			const uint8_t	BTi8		=	0x04;	//	0F BA /4 ib BT r/m16,imm8 Store selected bit in CF flag
													//	0F BA /4 ib BT r/m32,imm8 Store selected bit in CF flag
			const uint8_t	BTSi8		=	0x05;	//	0F BA /5 ib BTS r/m16,imm8 Store selected bit in CF flag and set
													//	0F BA /5 ib BTS r/m32,imm8 Store selected bit in CF flag and set
			const uint8_t	BTRi8		=	0x06;	//	0F BA /6 ib BTR r/m16,imm8 Store selected bit in CF flag and clear
													//	0F BA /6 ib BTR r/m32,imm8 Store selected bit in CF flag and clear
			const uint8_t	BTCi8		=	0x07;	//	0F BA /7 ib BTC r/m16,imm8 Store selected bit in CF flag and complement
													//	0F BA /7 ib BTC r/m32,imm8 Store selected bit in CF flag and complement
		};

		namespace xFC7
		{
			const uint8_t	CMPXCHG8B	=	0x01;	//	0F C7 /1 m64 CMPXCHG8B m64 Compare EDX:EAX with m64.
													//	If equal, set ZF and load ECX:EBX into m64.
													//	Else, clear ZF and load m64 intoEDX:EAX.
		};

		namespace xD8
		{
			const uint8_t	FADD32		=	0x00;	//	D8 /0 FADD m32 real Add m32real to ST(0) and store result in ST(0)
			const uint8_t	FCOM32		=	0x02;	//	D8 /2 FCOM m32real Compare ST(0) with m32real.
			const uint8_t	FCOMP32		=	0x03;	//	D8 /3 FCOMP m32real Compare ST(0) with m32real and pop register stack.
			const uint8_t	FDIV32		=	0x06;	//	D8 /6 FDIV m32real Divide ST(0) by m32real and store result in ST(0)
			const uint8_t	FDIVR32		=	0x07;	//	D8 /7 FDIVR m32real Divide m32real by ST(0) and store result in ST(0)
		};

		namespace xDA
		{
			const uint8_t	FIADD32		=	0x00;	//	DA /0 FIADD m32int Add m32int to ST(0) and store result in ST(0)
			const uint8_t	FICOM32		=	0x02;	//	DA /2 FICOM m32int Compare ST(0) with m32int
			const uint8_t	FICOMP32	=	0x03;	//	DA /3 FICOMP m32int Compare ST(0) with m32int and pop stack register
			const uint8_t	FIDIV32		=	0x06;	//	DA /6 FIDIV m32int Divide ST(0) by m32int and store result in ST(0)
			const uint8_t	FIDIVR32	=	0x07;	//	DA /7 FIDIVR m32int Divide m32int by ST(0) and store result in ST(0)
		};

		namespace xDB
		{
			const uint8_t	FILD32		=	0x00;	//	DB /0 FILD m32int Push m32int onto the FPU register stack.
			const uint8_t	FIST32		=	0x02;	//	DB /2 FIST m32int Store ST(0) in m32int
			const uint8_t	FISTP32		=	0x03;	//	DB /3 FISTP m32int Store ST(0) in m32int and pop register stack
		};

		namespace xDC
		{
			const uint8_t	FADD64		=	0x00;	//	DC /0 FADD m64real Add m64real to ST(0) and store result in ST(0)
			const uint8_t	FCOM64		=	0x02;	//	DC /2 FCOM m64real Compare ST(0) with m64real.
			const uint8_t	FCOMP64		=	0x03;	//	DC /3 FCOMP m64real Compare ST(0) with m64real and pop register stack.
			const uint8_t	FDIV64		=	0x06;	//	DC /6 FDIV m64real Divide ST(0) by m64real and store result in ST(0)
			const uint8_t	FDIVR64		=	0x07;	//	DC /7 FDIVR m64real Divide m64real by ST(0) and store result in ST(0)
		};

		namespace xDE
		{
			const uint8_t	FIADD16		=	0x00;	//	DE /0 FIADD m16int Add m16int to ST(0) and store result in ST(0)
			const uint8_t	FICOM16		=	0x02;	//	DE /2 FICOM m16int Compare ST(0) with m16int
			const uint8_t	FICOMP16	=	0x03;	//	DE /3 FICOMP m16int Compare ST(0) with m16int and pop stack register
			const uint8_t	FIDIV16		=	0x06;	//	DE /6 FIDIV m16int Divide ST(0) by m64int and store result in ST(0)
			const uint8_t	FIDIVR16	=	0x07;	//	DE /7 FIDIVR m16int Divide m64int by ST(0) and store result in ST(0)
		};

		namespace xDF
		{
			const uint8_t	FILD16		=	0x00;	//	DF /0 FILD m16int Push m16int onto the FPU register stack.
			const uint8_t	FIST16		=	0x02;	//	DF /2 FIST m16int Store ST(0) in m16int
			const uint8_t	FISTP16		=	0x03;	//	DF /3 FISTP m16int Store ST(0) in m16int and pop register stack
			const uint8_t	FBLD80		=	0x04;	//	DF /4 FBLD m80 dec Convert BCD value to real and push onto the FPU stack.
			const uint8_t	FILD64		=	0x05;	//	DF /5 FILD m64int Push m64int onto the FPU register stack.
			const uint8_t	FBSTP80		=	0x06;	//	DF /6 FBSTP m80bcd Store ST(0) in m80bcd and pop ST(0).
			const uint8_t	FISTP64		=	0x07;	//	DF /7 FISTP m64int Store ST(0) in m64int and pop register stack
		};

	};


	enum ADDR_MODE
	{
		AM_INV,		//	Invalid addressing mode
		AM_INH,		//	Inherent (i.e. no operands)
		AM_DIR,		//	Direct memory access
		AM_EXT,		//	Extended addressing (seg:addr)
		AM_REL,		//	Relative
		AM_IMM,		//	Immediate value
		AM_ENH		//	followed by ModRM [SIB]
	};


	class OpcodeInfo
	{
	public:

		uint8_t GetPrefix() const
		{
			return m_Prefix;
		}

		uint8_t GetOpcode() const
		{
			return m_Opcode;
		}

		ADDR_MODE GetAddressingMode() const
		{
			return m_AddressingMode;
		}

		uint8_t GetPostbyte() const
		{
			return m_Postbyte;
		}

		const Register *GetExplicitRegister() const
		{
			return m_Register;
		}

		OPERAND_TYPE GetOperandTypeDest() const
		{
			return m_OperandTypeDst;
		}

		OPERAND_TYPE GetOperandTypeSrc1() const
		{
			return m_OperandTypeSrc1;
		}

		OPERAND_TYPE GetOperandTypeSrc2() const
		{
			return m_OperandTypeSrc2;
		}

		size_t GetOperandSize() const
		{
			return m_OperandSize;
		}

#ifdef INCLUDE_TESTING
		const char *GetOperandName() const
		{
			return m_OpName;
		}

		const char *GetOperandDesc() const
		{
			return m_Desc;
		}
#endif

		bool GetIsTerminator() const
		{
			return m_IsTerminator;
		}


	public:

		uint8_t			m_Prefix;
		uint8_t			m_Opcode;
		ADDR_MODE		m_AddressingMode;
		uint8_t			m_Postbyte;
		const Register *m_Register;
		OPERAND_TYPE	m_OperandTypeDst;
		OPERAND_TYPE	m_OperandTypeSrc1;
		OPERAND_TYPE	m_OperandTypeSrc2;
		size_t			m_OperandSize;
#ifdef INCLUDE_TESTING
		const char *	m_OpName;
		const char *	m_Desc;
#endif
		bool			m_IsTerminator;
	};


	extern const OpcodeInfo *const opcodes32Table[];

}}}
