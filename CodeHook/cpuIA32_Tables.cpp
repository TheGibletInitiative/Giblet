//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include "cpuIA32.h"


#ifdef INCLUDE_TESTING
#define MAKE_REGISTER(name, value, enum, size, text)	const Register name(value, enum, size, text)
#define	OTET_(ext, inst, addr_mode, postbyte, specreg, reg1, reg2, reg3, size, opname, desc)	\
	{ext, inst, addr_mode, postbyte, specreg, reg1, reg2, reg3, size, opname, desc, true}

#define	OTE_(ext, inst, addr_mode, postbyte, specreg, reg1, reg2, reg3, size, opname, desc)		\
	{ext, inst, addr_mode, postbyte, specreg, reg1, reg2, reg3, size, opname, desc, false}
#else
#define MAKE_REGISTER(name, value, enum, size, text)	const Register name(value, enum, size)
#define	OTET_(ext, inst, addr_mode, postbyte, specreg, reg1, reg2, reg3, size, opname, desc)	\
	{ext, inst, addr_mode, postbyte, specreg, reg1, reg2, reg3, size, true}

#define	OTE_(ext, inst, addr_mode, postbyte, specreg, reg1, reg2, reg3, size, opname, desc)		\
	{ext, inst, addr_mode, postbyte, specreg, reg1, reg2, reg3, size, false}
#endif

namespace Giblet { namespace CodeHook { namespace x86
{

#ifdef _DEBUG
	static int CheckOpcodes()
	{
		const OpcodeInfo	*const	*tables = opcodes32Table;

		while(*tables)
		{
			const OpcodeInfo	*table;

			table = *(tables++);

			while(AM_INV != table->GetAddressingMode())
			{

				//	If a ModRM operand exists make sure there's a postbyte indicator
				if(		false != IsModRM(table->GetOperandTypeDest())
					||	false != IsModRM(table->GetOperandTypeSrc1())
					||	false != IsModRM(table->GetOperandTypeSrc2()))
				{
					assert(PO::NONE != table->GetPostbyte());
				}

				//	If there's a postbyte indicator make sure there's a ModRM operand
				if(PO::NONE != table->GetPostbyte())
				{
					assert(
							false != IsModRM(table->GetOperandTypeDest())
						||	false != IsModRM(table->GetOperandTypeSrc1())
						||	false != IsModRM(table->GetOperandTypeSrc2())
						);
				}

				//	Check operands for addressing modes
				switch(table->GetAddressingMode())
				{
				case AM_INV:
					break;

				case AM_REL:
					assert(
						(	false == IsRelative(table->GetOperandTypeDest())
						&&	false != IsRelative(table->GetOperandTypeSrc1())
						&&	false == IsRelative(table->GetOperandTypeSrc2())
						)
						);
					break;

				case AM_ENH:
					assert(PO::NONE != table->GetPostbyte());
					assert(
						(	false != IsModRM(table->GetOperandTypeDest())
						||	false != IsModRM(table->GetOperandTypeSrc1())
						||	false != IsModRM(table->GetOperandTypeSrc2())
						)
						);
					break;

				case AM_EXT:
					assert(
						(	OT_NONE == table->GetOperandTypeDest()
						&&	OT_IMM == table->GetOperandTypeSrc1()
						&&	OT_IMM16 == table->GetOperandTypeSrc2()
						)
						);
					break;


				case AM_DIR:
					assert(
							(	false != IsModRM(table->GetOperandTypeDest())
							&&	false != IsModRM(table->GetOperandTypeSrc1())
							&&	OT_NONE == table->GetOperandTypeSrc2()
							)
						||
							(	OT_NONE == table->GetOperandTypeDest()
							&&	false != IsModRM(table->GetOperandTypeSrc1())
							&&	OT_NONE == table->GetOperandTypeSrc2()
							)
						);
					break;

				case AM_IMM:
					assert(
							(	OT_NONE == table->GetOperandTypeDest()
							&&	false != IsImmediate(table->GetOperandTypeSrc1())
							&&	OT_NONE == table->GetOperandTypeSrc2()
							)
						||
							(	false != IsRegister(table->GetOperandTypeDest())
							&&	false != IsImmediate(table->GetOperandTypeSrc1())
							&&	OT_NONE == table->GetOperandTypeSrc2()
							)
						||
							(	false != IsModRM(table->GetOperandTypeDest())
							&&	false != IsImmediate(table->GetOperandTypeSrc1())
							&&	OT_NONE == table->GetOperandTypeSrc2()
							)

						||
							(	PO::REGISTER == table->GetPostbyte()
							&&	false != IsModRM(table->GetOperandTypeDest())
							&&	false != IsModRM(table->GetOperandTypeSrc1())
							&&	false != IsImmediate(table->GetOperandTypeSrc2())
							)


							);
					break;

				case AM_INH:
					assert(
							(	OT_NONE == table->GetOperandTypeDest()
							&&	OT_NONE == table->GetOperandTypeSrc1()
							&&	OT_NONE == table->GetOperandTypeSrc2()
							)

						||	(	OT_NONE == table->GetOperandTypeDest()
							&&	true == IsRegister(table->GetOperandTypeSrc1())
							&&	OT_NONE == table->GetOperandTypeSrc2()
							)

						||	(	IsRegister(table->GetOperandTypeDest())
							&&	OT_NONE == table->GetOperandTypeSrc1()
							&&	OT_NONE == table->GetOperandTypeSrc2()
							)

						);
					break;

				default:
					//assert(0);
					break;
				}

				table++;
			}
		}
		return 0;
	}

	const int test = CheckOpcodes();
#endif




	bool IsRegister(OPERAND_TYPE type)
	{
		switch(type)
		{
		case OT_GPREG:
		case OT_GPREG8:
		case OT_SREG:
			return true;
			break;
		}
		return false;
	}


	bool IsModRM(OPERAND_TYPE type)
	{
		switch(type)
		{
		case OT_MRMr:
		case OT_MRMsr:
		case OT_MRMr8:
		case OT_MRM:
		case OT_MRM8:
			return true;
			break;
		}

		return false;
	}


	bool IsRelative(OPERAND_TYPE type)
	{
		switch(type)
		{
		case OT_REL:
		case OT_REL8:
		case OT_REL16:
		case OT_REL32:
		case OT_REL64:
			return true;
			break;
		}
		return false;
	}
	

	bool IsImmediate(OPERAND_TYPE type)
	{
		switch(type)
		{
		case OT_IMM:
		case OT_IMM8:
		case OT_IMM16:
		case OT_IMM32:
		case OT_IMM64:
			return true;
			break;
		}
		return false;
	}



	namespace REGS
	{
		//const Register NONE(0xff, RT_GP8, 0, "<???>");

		//	8 bit general purposes registers
		MAKE_REGISTER(AL, 0x00, RT_GP8, 1, "AL");
		MAKE_REGISTER(CL, 0x01, RT_GP8, 1, "CL");
		MAKE_REGISTER(DL, 0x02, RT_GP8, 1, "DL");
		MAKE_REGISTER(BL, 0x03, RT_GP8, 1, "BL");
		MAKE_REGISTER(AH, 0x04, RT_GP8, 1, "AH");
		MAKE_REGISTER(CH, 0x05, RT_GP8, 1, "CH");
		MAKE_REGISTER(DH, 0x06, RT_GP8, 1, "DH");
		MAKE_REGISTER(BH, 0x07, RT_GP8, 1, "BH");

		//	16 bit general purposes registers
		MAKE_REGISTER(AX, 0x00, RT_GP16, 2, "AX");
		MAKE_REGISTER(CX, 0x01, RT_GP16, 2, "CX");
		MAKE_REGISTER(DX, 0x02, RT_GP16, 2, "DX");
		MAKE_REGISTER(BX, 0x03, RT_GP16, 2, "BX");
		MAKE_REGISTER(SP, 0x04, RT_GP16, 2, "SP");
		MAKE_REGISTER(BP, 0x05, RT_GP16, 2, "BP");
		MAKE_REGISTER(SI, 0x06, RT_GP16, 2, "SI");
		MAKE_REGISTER(DI, 0x07, RT_GP16, 2, "DI");

		//	32 bit general purposes registers
		MAKE_REGISTER(EAX, 0x00, RT_GP32, 4, "EAX");
		MAKE_REGISTER(ECX, 0x01, RT_GP32, 4, "ECX");
		MAKE_REGISTER(EDX, 0x02, RT_GP32, 4, "EDX");
		MAKE_REGISTER(EBX, 0x03, RT_GP32, 4, "EBX");
		MAKE_REGISTER(ESP, 0x04, RT_GP32, 4, "ESP");
		MAKE_REGISTER(EBP, 0x05, RT_GP32, 4, "EBP");
		MAKE_REGISTER(ESI, 0x06, RT_GP32, 4, "ESI");
		MAKE_REGISTER(EDI, 0x07, RT_GP32, 4, "EDI");

		//	Segment registers
		MAKE_REGISTER(ES, 0x00, RT_SEG, 2, "ES");
		MAKE_REGISTER(CS, 0x01, RT_SEG, 2, "CS");
		MAKE_REGISTER(SS, 0x02, RT_SEG, 2, "SS");
		MAKE_REGISTER(DS, 0x03, RT_SEG, 2, "DS");
		MAKE_REGISTER(FS, 0x04, RT_SEG, 2, "FS");
		MAKE_REGISTER(GS, 0x05, RT_SEG, 2, "GS");
		MAKE_REGISTER(R1, 0x06, RT_SEG, 2, "R1");
		MAKE_REGISTER(R2, 0x07, RT_SEG, 2, "R2");

		MAKE_REGISTER(MM0, 0x00, RT_MMX, 4, "MM32$I0");
		MAKE_REGISTER(MM1, 0x01, RT_MMX, 4, "MM32$I0");
		MAKE_REGISTER(MM2, 0x02, RT_MMX, 4, "MM32$I0");
		MAKE_REGISTER(MM3, 0x03, RT_MMX, 4, "MM32$I0");
		MAKE_REGISTER(MM4, 0x04, RT_MMX, 4, "MM32$I0");
		MAKE_REGISTER(MM5, 0x05, RT_MMX, 4, "MM32$I0");
		MAKE_REGISTER(MM6, 0x06, RT_MMX, 4, "MM32$I0");
		MAKE_REGISTER(MM7, 0x07, RT_MMX, 4, "MM32$I0");

		
		const Register *const SegmentRegisters[]	=	{&ES, &CS, &SS, &DS, &FS, &GS, &R1, &R2, NULL};
		const Register *const GP8Registers[]		=	{&AL, &CL, &DL, &BL, &AH, &CH, &DH, &BH, NULL};
		const Register *const GP16Registers[]		=	{&AX, &CX, &DX, &BX, &SP, &BP, &SI, &DI, NULL};
		const Register *const GP32Registers[]		=	{&EAX, &ECX, &EDX, &EBX, &ESP, &EBP, &ESI, &EDI, NULL};
		const Register *const MMXRegisters[]		=	{&MM0, &MM1, &MM2, &MM3, &MM4, &MM5, &MM6, &MM7, NULL};
		const Register *const *const GPRegisters[]	=
		{
			GP8Registers,
			GP16Registers,
			GP32Registers,
			NULL
		};

	}


	const OpcodeInfo opcodesTableGeneral[] =
	{
		//   op1	op2		addr_mode	postbyte			exp reg		dst_type	src1		src2		sz	op			desc

		OTE_(0x00,	0xc3,	AM_INH,		PO::NONE,			NULL,		OT_NONE,	OT_NONE,	OT_NONE,	4,	"RET",		"NEAR"),
		OTE_(0x00,	0xcb,	AM_INH,		PO::NONE,			NULL,		OT_NONE,	OT_NONE,	OT_NONE,	4,	"RETF",		"FAR"),
		OTE_(0x00,	0xc2,	AM_IMM,		PO::NONE,			NULL,		OT_NONE,	OT_IMM16,	OT_NONE,	2,	"RET",		"NEAR imm16"),
		OTE_(0x00,	0xca,	AM_IMM,		PO::NONE,			NULL,		OT_NONE,	OT_IMM16,	OT_NONE,	2,	"RETF",		"FAR imm16"),

		OTE_(0x00,	0xe2,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"LOOP",		"E2 cb LOOP rel8 Decrement count; jump short if count ¹ 0"),
		OTE_(0x00,	0xe1,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"LOOPZ",	"E1 cb LOOPE rel8 Decrement count; jump short if count ¹ 0 and ZF=1"),
		OTE_(0x00,	0xe0,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"LOOPNZ",	"E0 cb LOOPNE rel8 Decrement count; jump short if count ¹ 0 and ZF=0"),
		OTE_(0x00,	0x99,	AM_INH,		PO::NONE,			NULL,		OT_NONE,	OT_NONE,	OT_NONE,	4,	"CDQ",		"EDX:EAX ¬ sign-extend of EAX"),

		//	TO CHECK
		OTE_(0x00,	0x37,	AM_INH,		PO::NONE,			NULL,		OT_NONE,	OT_NONE,	OT_NONE,	4,	"AAA",		"ASCII adjust AL after addition"),
		OTE_(0x00,	0xD5,	AM_IMM,		PO::NONE,			NULL,		OT_NONE,	OT_IMM8,	OT_NONE,	1,	"AAD",		"Adjust AX before division to number base imm8"),
		OTE_(0x00,	0xD4,	AM_IMM,		PO::NONE,			NULL,		OT_NONE,	OT_IMM8,	OT_NONE,	1,	"AAM",		"Adjust AX after multiply to number base imm8"),
		OTE_(0x00,	0x3F,	AM_INH,		PO::NONE,			NULL,		OT_NONE,	OT_NONE,	OT_NONE,	4,	"AAS",		"ASCII adjust AL after subtraction"),
	
	
		//	0F BC BSF r32,r/m32 Bit scan forward on r/m32
		//	0F BD BSR r32,r/m32 Bit scan reverse on r/m32
		//	0F C8+rd BSWAP r32 Reverses the byte order of a 32-bit register.
		OTE_(0x00,	0x8D,	AM_ENH,		PO::REGISTER,		NULL,		OT_MRMr,	OT_MRM,		OT_NONE,	4,	"LEA",		"/r LEA r32,m Store effective address for m in register r32"),

		OTET_(0x00,	0xcc,	AM_INH,		PO::NONE,			NULL,		OT_NONE,	OT_NONE,	OT_NONE,	0,	"INT3",		""),

		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTableADC[] =
	{
		OTE_(0x00,	0x14,	AM_IMM,		PO::NONE,			&REGS::AL,	OT_GPREG8,	OT_IMM8,	OT_NONE,	1,	"ADC",	"ib ADC AL, imm8 Add with carry imm8 to AL"),
		OTE_(0x00,	0x15,	AM_IMM,		PO::NONE,			&REGS::EAX,	OT_GPREG,	OT_IMM,		OT_NONE,	4,	"ADC",	"id ADC EAX, imm32 Add with carry imm32 to EAX"),
		OTE_(0x00,	0x80,	AM_IMM,		PO::x80::ADC8i8,	NULL,		OT_MRM8,	OT_IMM8,	OT_NONE,	1,	"ADC",	"/2 ib ADC r/m8, imm8 Add with carry imm8 to r/m8"),
		OTE_(0x00,	0x81,	AM_IMM,		PO::x81::ADCi,		NULL,		OT_MRM,		OT_IMM,		OT_NONE,	4,	"ADC",	"/2 id ADC r/m32,imm32 Add with CF imm32 to r/m32"),
		OTE_(0x00,	0x83,	AM_IMM,		PO::x83::ADCi8,		NULL,		OT_MRM,		OT_IMM8,	OT_NONE,	1,	"ADC",	"/2 ib ADC r/m32,imm8 Add with CF sign-extended imm8 into r/m32"),
		OTE_(0x00,	0x10,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM8,	OT_MRMr8,	OT_NONE,	1,	"ADC",	"/r ADC r/m8,r8 Add with carry byte register to r/m8"),
		OTE_(0x00,	0x11,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM,		OT_MRMr,	OT_NONE,	4,	"ADC",	"/r ADC r/m32,r32 Add with CF r32 to r/m32"),
		OTE_(0x00,	0x12,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr8,	OT_MRM8,	OT_NONE,	1,	"ADC",	"/r ADC r8,r/m8 Add with carry r/m8 to byte register"),
		OTE_(0x00,	0x13,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr,	OT_MRM,		OT_NONE,	4,	"ADC",	"/r ADC r32,r/m32 Add with CF r/m32 to r32"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTableADD[] =
	{
		OTE_(0x00,	0x04,	AM_IMM,		PO::NONE,			&REGS::AL,	OT_GPREG8,	OT_IMM8,	OT_NONE,	1,	"ADD",	"ib ADD AL, imm8 Add imm8 to AL"),
		OTE_(0x00,	0x05,	AM_IMM,		PO::NONE,			&REGS::EAX,	OT_GPREG,	OT_IMM,		OT_NONE,	4,	"ADD",	"id ADD EAX, imm32 Add imm32 to EAX"),
		OTE_(0x00,	0x80,	AM_IMM,		PO::x80::ADD8i8,	NULL,		OT_MRM8,	OT_IMM8,	OT_NONE,	1,	"ADD",	"/0 ib ADD r/m8,imm8 Add imm8 to r/m8"),
		OTE_(0x00,	0x81,	AM_IMM,		PO::x81::ADDi,		NULL,		OT_MRM,		OT_IMM,		OT_NONE,	4,	"ADD",	"/0 id ADD r/m32,imm32 Add imm32 to r/m32"),
		OTE_(0x00,	0x83,	AM_IMM,		PO::x83::ADDi8,		NULL,		OT_MRM,		OT_IMM8,	OT_NONE,	1,	"ADD",	"/0 ib ADD r/m32,imm8 Add sign-extended imm8 to r/m32"),
		OTE_(0x00,	0x00,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM8,	OT_MRMr8,	OT_NONE,	1,	"ADD",	"/r ADD r/m8,r8 Add r8 to r/m8"),
		OTE_(0x00,	0x01,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM,		OT_MRMr,	OT_NONE,	4,	"ADD",	"/r ADD r/m32,r32 Add r32 to r/m32"),
		OTE_(0x00,	0x02,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr8,	OT_MRM8,	OT_NONE,	1,	"ADD",	"/r ADD r8,r/m8 Add r/m8 to r8"),
		OTE_(0x00,	0x03,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr,	OT_MRM,		OT_NONE,	4,	"ADD",	"/r ADD r32,r/m32 Add r/m32 to r32"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTableMUL[] =
	{
		OTE_(0x00,	0xF6,	AM_ENH,		PO::xF6::IMUL8,		&REGS::AL,	OT_MRM8,	OT_IMM8,	OT_NONE,	1,	"IMUL",	"/5 IMUL r/m8 AX¬ AL * r/m byte"),
		OTE_(0x00,	0xF7,	AM_ENH,		PO::xF7::IMUL,		&REGS::EAX,	OT_MRM,		OT_IMM,		OT_NONE,	4,	"IMUL",	"/5 IMUL r/m32 EDX:EAX ¬ EAX * r/m doubleword"),
		OTE_(0x0F,	0xAF,	AM_ENH,		PO::REGISTER,		NULL,		OT_MRMr,	OT_MRM,		OT_NONE,	4,	"IMUL",	"/r IMUL r32,r/m32 doubleword register ¬ doubleword register * r/m doubleword"),
		OTE_(0x00,	0x6B,	AM_ENH,		PO::REGISTER,		NULL,		OT_MRMr,	OT_MRM,		OT_IMM8,	1,	"IMUL",	"/r ib IMUL r32,r/m32,imm8 doubleword register ¬ r/m32 * sign-extended immediate byte"),
		OTE_(0x00,	0x6B,	AM_IMM,		PO::REGISTER,		NULL,		OT_MRMr,	OT_IMM8,	OT_NONE,	1,	"IMUL",	"/r ib IMUL r32,imm8 doubleword register ¬ doubleword register * sign-extended immediate byte"),
		OTE_(0x00,	0x69,	AM_IMM,		PO::REGISTER,		NULL,		OT_MRMr,	OT_MRM,		OT_IMM,		4,	"IMUL",	"/r id IMUL r32,r/m32,imm32 doubleword register ¬ r/m32 * immediate doubleword"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTableDIV[] =
	{
		OTE_(0x00,	0xF6,	AM_ENH,		PO::xF6::IDIV8,		NULL,		OT_NONE,	OT_MRM8,	OT_NONE,	1,	"IDIV",	"/7 IDIV r/m8 Signed divide AX (where AH must contain signextension of AL) by r/m byte. (Results: AL=Quotient, AH=Remainder)"),
		OTE_(0x00,	0xF7,	AM_ENH,		PO::xF7::IDIV,		NULL,		OT_NONE,	OT_MRM,		OT_NONE,	4,	"IDIV",	"/7 IDIV r/m32 Signed divide EDX:EAX (where EDX must contain sign-extension of EAX) by r/m doubleword. (Results: EAX=Quotient, EDX=Remainder)"),
		{0x00,	0x00,	AM_INV}
	};


	//	Test
	const OpcodeInfo opcodesTableAND[] =
	{
		OTE_(0x00,	0x24,	AM_IMM,		PO::NONE,			&REGS::AL,	OT_GPREG8,	OT_IMM8,	OT_NONE,	1,	"AND",	"ib AND AL, imm8 AL AND imm8"),
		OTE_(0x00,	0x25,	AM_IMM,		PO::NONE,			&REGS::EAX,	OT_GPREG,	OT_IMM,		OT_NONE,	4,	"AND",	"id AND EAX, imm32 EAX AND imm32"),
		OTE_(0x00,	0x80,	AM_IMM,		PO::x80::AND8i8,	NULL,		OT_MRM8,	OT_IMM8,	OT_NONE,	1,	"AND",	"/4 ib AND r/m8,imm8 r/m8 AND imm8"),
		OTE_(0x00,	0x81,	AM_IMM,		PO::x83::ANDi8,		NULL,		OT_MRM,		OT_IMM8,	OT_NONE,	1,	"AND",	"/4 id AND r/m32,imm32 r/m32 AND imm32"),
		OTE_(0x00,	0x83,	AM_IMM,		PO::x83::ANDi8,		NULL,		OT_MRM,		OT_IMM8,	OT_NONE,	1,	"AND",	"/4 ib AND r/m32,imm8 r/m32 AND imm8 (sign-extended)"),
		OTE_(0x00,	0x20,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM8,	OT_MRMr8,	OT_NONE,	1,	"AND",	"/r AND r/m8,r8 r/m8 AND r8"),
		OTE_(0x00,	0x21,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM,		OT_MRMr,	OT_NONE,	4,	"AND",	"/ r AND r/m32,r32 r/m32 AND r32"),
		OTE_(0x00,	0x22,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr8,	OT_MRM8,	OT_NONE,	1,	"AND",	"/ r AND r8,r/m8 r8 AND r/m8"),
		OTE_(0x00,	0x23,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr,	OT_MRM,		OT_NONE,	4,	"AND",	"/ r AND r32,r/m32 r32 AND r/m32"),
		{0x00,	0x00,	AM_INV}
	};


	//	Test
	const OpcodeInfo opcodesTableBT[] =
	{
		OTE_(0x0F,	0xA3,	AM_ENH,		PO::REGISTER,		NULL,		OT_MRM,		OT_MRMr,	OT_NONE,	4,	"BT",	"/r BT r/m32,r32 Store selected bit in CF flag"),
		OTE_(0x0F,	0xBA,	AM_IMM,		PO::x0FBA::BTi8,	NULL,		OT_MRM,		OT_IMM8,	OT_NONE,	1,	"BT",	"/4 ib BT r/m32,imm8 Store selected bit in CF flag"),
		{0x00,	0x00,	AM_INV}
	};


	//	Test
	const OpcodeInfo opcodesTableBTC[] =
	{
		OTE_(0x0F,	0xBB,	AM_ENH,		PO::REGISTER,		NULL,		OT_MRM,		OT_MRMr,	OT_NONE,	4,	"BTC",	"BTC r/m32,r32 Store selected bit in CF flag and complement"),
		OTE_(0x0F,	0xBA,	AM_IMM,		PO::x0FBA::BTCi8,	NULL,		OT_MRM,		OT_IMM8,	OT_NONE,	1,	"BTC",	"/7 ib BTC r/m32,imm8 Store selected bit in CF flag and complement"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTableCALL[] =
	{
		OTE_(0x00,	0xE8,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"CALL",	"cd CALL rel32 Call near, relative, displacement relative to next instruction"),
		OTE_(0x00,	0xFF,	AM_ENH,		PO::xFF::NCALL,		NULL,		OT_NONE,	OT_MRM,		OT_NONE,	4,	"CALL",	"/2 CALL r/m32 Call near, absolute indirect, address given in r/m32"),
		OTE_(0x00,	0x9A,	AM_EXT,		PO::NONE,			NULL,		OT_NONE,	OT_IMM,		OT_IMM16,	4,	"CALL",	"cp CALL ptr16:32 Call far, absolute, address given in operand"),
		OTE_(0x00,	0xFF,	AM_ENH,		PO::xFF::FCALL,		NULL,		OT_NONE,	OT_MRM,		OT_NONE,	4,	"CALL",	"/3 CALL m16:32 Call far, absolute indirect, address given in m16:32"),
		{0x00,	0x00,	AM_INV}
	};


	//	Test
	const OpcodeInfo opcodesTableCMP[] =
	{
		OTE_(0x00,	0x3C,	AM_IMM,		PO::NONE,			&REGS::AL,	OT_GPREG8,	OT_IMM8,	OT_NONE,	1,	"CMP",	"ib CMP AL, imm8 Compare imm8 with AL"),
		OTE_(0x00,	0x3D,	AM_IMM,		PO::NONE,			&REGS::EAX,	OT_GPREG,	OT_IMM,		OT_NONE,	4,	"CMP",	"id CMP EAX, imm32 Compare imm32 with EAX"),
		OTE_(0x00,	0x80,	AM_IMM,		PO::x80::CMP8i8,	NULL,		OT_MRM8,	OT_IMM8,	OT_NONE,	1,	"CMP",	"/7 ib CMP r/m8, imm8 Compare imm8 with r/m8"),
		OTE_(0x00,	0x81,	AM_IMM,		PO::x81::CMPi,		NULL,		OT_MRM,		OT_IMM,		OT_NONE,	4,	"CMP",	"/7 id CMP r/m32,imm32 Compare imm32 with r/m32"),
		OTE_(0x00,	0x83,	AM_IMM,		PO::x83::CMPi8,		NULL,		OT_MRM,		OT_IMM8,	OT_NONE,	1,	"CMP",	"/7 ib CMP r/m32,imm8 Compare imm8 with r/m32"),
		OTE_(0x00,	0x38,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM8,	OT_MRMr8,	OT_NONE,	1,	"CMP",	"/r CMP r/m8,r8 Compare r8 with r/m8"),
		OTE_(0x00,	0x39,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM,		OT_MRMr,	OT_NONE,	4,	"CMP",	"/r CMP r/m32,r32 Compare r32 with r/m32"),
		OTE_(0x00,	0x3A,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr8,	OT_MRM8,	OT_NONE,	1,	"CMP",	"/r CMP r8,r/m8 Compare r/m8 with r8"),
		OTE_(0x00,	0x3B,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr,	OT_MRM,		OT_NONE,	4,	"CMP",	"/r CMP r32,r/m32 Compare r/m32 with r32"),
		OTE_(0x00,	0xA6,	AM_INH,		PO::NONE,			NULL,		OT_NONE,	OT_NONE,	OT_NONE,	1,	"CMPS",	"m8, m8 Compares byte at address DS:(E)SI with byte at address			ES:(E)DI and sets the status flags accordingly"),
		OTE_(0x00,	0xA7,	AM_INH,		PO::NONE,			NULL,		OT_NONE,	OT_NONE,	OT_NONE,	4,	"CMPS",	"m32, m32 Compares doubleword at address DS:(E)SI with doubleword at address ES:(E)DI and sets the status flags accordingly"),
		{0x00,	0x00,	AM_INV}
	};


	//	test
	const OpcodeInfo opcodesTableJcc[] =
	{
		OTE_(0x00,	0x70,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JO",			"cb JO rel8 Jump short if overflow (OF=1)"),
		OTE_(0x00,	0x71,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JNO",			"cb JNO rel8 Jump short if not overflow (OF=0)"),
		OTE_(0x00,	0x72,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JB/JC/JNAE",	"cb JB rel8 Jump short if below (CF=1)"),
		OTE_(0x00,	0x73,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JAE/JNB/JNC",	"cb JAE rel8 Jump short if above or equal (CF=0)"),
		OTE_(0x00,	0x74,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JE/JZ",		"cb JE rel8 Jump short if equal (ZF=1)"),
		OTE_(0x00,	0x75,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JNE/JNZ",		"cb JNE rel8 Jump short if not equal (ZF=0)"),
		OTE_(0x00,	0x76,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JBE/JNA",		"cb JBE rel8 Jump short if below or equal (CF=1 or ZF=1)"),
		OTE_(0x00,	0x77,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JA/JNBE",		"cb JA rel8 Jump short if above (CF=0 and ZF=0)"),
		OTE_(0x00,	0x78,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JS",			"cb JS rel8 Jump short if sign (SF=1)"),
		OTE_(0x00,	0x79,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JNS",			"cb JNS rel8 Jump short if not sign (SF=0)"),
		OTE_(0x00,	0x7A,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JP/JPE",		"cb JP rel8 Jump short if parity (PF=1)"),
		OTE_(0x00,	0x7B,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JNP/JPO",		"cb JNP rel8 Jump short if not parity (PF=0)"),
		OTE_(0x00,	0x7C,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JL/JNGE",		"cb JL rel8 Jump short if less (SF<>OF)"),
		OTE_(0x00,	0x7D,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JGE/JNL",		"cb JGE rel8 Jump short if greater or equal (SF=OF)"),
		OTE_(0x00,	0x7E,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JLE/JNG",		"cb JLE rel8 Jump short if less or equal (ZF=1 or SF<>OF)"),
		OTE_(0x00,	0x7F,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JG/JNLE",		"cb JNLE rel8 Jump short if not less or equal (ZF=0 and SF=OF)"),
		OTE_(0x00,	0xE3,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JCXZ/JECXZ",	"cb JCXZ rel8 Jump short if CX register is 0"),
		OTE_(0x0F,	0x80,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JO",			"cw/cd JO rel16/32 Jump near if overflow (OF=1)"),
		OTE_(0x0F,	0x81,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JNO",			"cw/cd JNO rel16/32 Jump near if not overflow (OF=0)"),
		OTE_(0x0F,	0x82,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JB/JC/JNAE",	"cw/cd JB/JC/JNAE rel16/32 Jump near if below (CF=1)"),
		OTE_(0x0F,	0x83,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JAE/JNB/JNC",	"cw/cd JAE/JNB/JNC rel16/32 Jump near if above or equal (CF=0)"),
		OTE_(0x0F,	0x84,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JE/JZ",		"cw/cd JE/JZ rel16/32 Jump near if equal (ZF=1)"),
		OTE_(0x0F,	0x85,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JNE/JNZ",		"cw/cd JNE/JNZ rel16/32 Jump near if not equal (ZF=0)"),
		OTE_(0x0F,	0x86,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JBE/JNA",		"cw/cd JBE/JNA rel16/32 Jump near if below or equal (CF=1 or ZF=1)"),
		OTE_(0x0F,	0x87,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JA/JNBE",		"cw/cd JA/JNBE rel16/32 Jump near if above (CF=0 and ZF=0)"),
		OTE_(0x0F,	0x88,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JS",			"cw/cd JS rel16/32 Jump near if sign (SF=1)"),
		OTE_(0x0F,	0x89,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JNS",			"cw/cd JNS rel16/32 Jump near if not sign (SF=0)"),
		OTE_(0x0F,	0x8A,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JP/JPE",		"cw/cd JP/JPE rel16/32 Jump near if parity (PF=1)"),
		OTE_(0x0F,	0x8B,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JNP/JPO",		"cw/cd JNP/JPO rel16/32 Jump near if not parity (PF=0)"),
		OTE_(0x0F,	0x8C,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JL/JNGE",		"cw/cd JL/JNGE rel16/32 Jump near if less (SF<>OF)"),
		OTE_(0x0F,	0x8D,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JGE/JNL",		"cw/cd JGE/JNL rel16/32 Jump near if greater or equal (SF=OF)"),
		OTE_(0x0F,	0x8E,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JLE/JNG",		"cw/cd JLE/JNG rel16/32 Jump near if less or equal (ZF=1 or SF<>OF)"),
		OTE_(0x0F,	0x8F,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JG/JNLE",		"cw/cd JG/JNLE rel16/32 Jump near if greater (ZF=0 and SF=OF)"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTableJMP[] =
	{
		OTE_(0x00,	0xEB,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL8,	OT_NONE,	1,	"JMP",	"rel8     Jump short, relative"),	//, displacement relative to next instruction
		OTE_(0x00,	0xE9,	AM_REL,		PO::NONE,			NULL,		OT_NONE,	OT_REL,		OT_NONE,	4,	"JMP",	"rel32 	  Jump near, relative"),	//, displacement relative to next instruction
		OTE_(0x00,	0xFF,	AM_ENH,		PO::xFF::JUMPN,		NULL,		OT_NONE,	OT_MRM,		OT_NONE,	4,	"JMP",	"r/m32    Jump near, absolute indirect"),
		OTE_(0x00,	0xEA,	AM_EXT,		PO::NONE,			NULL,		OT_NONE,	OT_IMM,		OT_IMM16,	4,	"JMP",	"ptr16:32 Jump far, absolute, address given in operand"),
		OTE_(0x00,	0xFF,	AM_DIR,		PO::xFF::JUMPF,		NULL,		OT_NONE,	OT_MRM,		OT_NONE,	4,	"JMP",	"m16:32   Jump far, absolute indirect, address in m16:32"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTableMOV[] =
	{
		OTE_(0x00,	0x88,	AM_ENH,		PO::REGISTER,		NULL,		OT_MRM8,	OT_MRMr8,	OT_NONE,	1,	"MOV",	"/r r/m8,r8 Move r8 to r/m8"),
		OTE_(0x00,	0x89,	AM_ENH,		PO::REGISTER,		NULL,		OT_MRM,		OT_MRMr,	OT_NONE,	4,	"MOV",	"/r r/m32,r32 Move r32 to r/m32"),
		OTE_(0x00,	0x8A,	AM_ENH,		PO::REGISTER,		NULL,		OT_MRMr8,	OT_MRM8,	OT_NONE,	1,	"MOV",	"/r r8,r/m8 Move r/m8 to r8"),
		OTE_(0x00,	0x8B,	AM_ENH,		PO::REGISTER,		NULL,		OT_MRMr,	OT_MRM,		OT_NONE,	4,	"MOV",	"/r r32,r/m32 Move r/m32 to r32"),
		OTE_(0x00,	0x8C,	AM_ENH,		PO::REGISTER,		NULL,		OT_MRM,		OT_MRMsr,	OT_NONE,	0,	"MOV",	"/r r/m16,Sreg** Move segment register to r/m16"),
		OTE_(0x00,	0x8E,	AM_ENH,		PO::REGISTER,		NULL,		OT_MRMsr,	OT_MRM,		OT_NONE,	0,	"MOV",	"/r Sreg,r/m16** Move r/m16 to segment register"),
		OTE_(0x00,	0xA0,	AM_IMM,		PO::NONE,			&REGS::AL,	OT_GPREG8,	OT_IMM,		OT_NONE,	4,	"MOV",	"AL, moffs8* Move byte at ( seg:offset) to AL"),
		OTE_(0x00,	0xA1,	AM_IMM,		PO::NONE,			&REGS::EAX,	OT_GPREG,	OT_IMM,		OT_NONE,	4,	"MOV",	"EAX, moffs32* Move doubleword at ( seg:offset) to EAX"),
		OTE_(0x00,	0xA2,	AM_IMM,		PO::NONE,			&REGS::AL,	OT_GPREG8,	OT_IMM,		OT_NONE,	0,	"MOV",	"moffs8*,AL Move AL to ( seg:offset)"),
		OTE_(0x00,	0xA3,	AM_IMM,		PO::NONE,			&REGS::EAX,	OT_GPREG,	OT_IMM,		OT_NONE,	0,	"MOV",	"moffs32*,EAX Move EAX to ( seg:offset)"),
		OTE_(0x00,	0xB0,	AM_IMM,		PO::NONE,			&REGS::AL,	OT_GPREG8,	OT_IMM8,	OT_NONE,	0,	"MOV",	"r8,imm8 Move imm8 to r8"),
		OTE_(0x00,	0xB1,	AM_IMM,		PO::NONE,			&REGS::CL,	OT_GPREG8,	OT_IMM8,	OT_NONE,	0,	"MOV",	"r8,imm8 Move imm8 to r8"),
		OTE_(0x00,	0xB2,	AM_IMM,		PO::NONE,			&REGS::DL,	OT_GPREG8,	OT_IMM8,	OT_NONE,	0,	"MOV",	"r8,imm8 Move imm8 to r8"),
		OTE_(0x00,	0xB3,	AM_IMM,		PO::NONE,			&REGS::BL,	OT_GPREG8,	OT_IMM8,	OT_NONE,	0,	"MOV",	"r8,imm8 Move imm8 to r8"),
		OTE_(0x00,	0xB4,	AM_IMM,		PO::NONE,			&REGS::AH,	OT_GPREG8,	OT_IMM8,	OT_NONE,	0,	"MOV",	"r8,imm8 Move imm8 to r8"),
		OTE_(0x00,	0xB5,	AM_IMM,		PO::NONE,			&REGS::CH,	OT_GPREG8,	OT_IMM8,	OT_NONE,	0,	"MOV",	"r8,imm8 Move imm8 to r8"),
		OTE_(0x00,	0xB6,	AM_IMM,		PO::NONE,			&REGS::DH,	OT_GPREG8,	OT_IMM8,	OT_NONE,	0,	"MOV",	"r8,imm8 Move imm8 to r8"),
		OTE_(0x00,	0xB7,	AM_IMM,		PO::NONE,			&REGS::BH,	OT_GPREG8,	OT_IMM8,	OT_NONE,	0,	"MOV",	"r8,imm8 Move imm8 to r8"),
		OTE_(0x00,	0xB8,	AM_IMM,		PO::NONE,			&REGS::EAX,	OT_GPREG8,	OT_IMM,		OT_NONE,	0,	"MOV",	"r32,imm32 Move imm32 to r32"),
		OTE_(0x00,	0xB9,	AM_IMM,		PO::NONE,			&REGS::ECX,	OT_GPREG8,	OT_IMM,		OT_NONE,	0,	"MOV",	"r32,imm32 Move imm32 to r32"),
		OTE_(0x00,	0xBA,	AM_IMM,		PO::NONE,			&REGS::EDX,	OT_GPREG8,	OT_IMM,		OT_NONE,	0,	"MOV",	"r32,imm32 Move imm32 to r32"),
		OTE_(0x00,	0xBB,	AM_IMM,		PO::NONE,			&REGS::EBX,	OT_GPREG8,	OT_IMM,		OT_NONE,	0,	"MOV",	"r32,imm32 Move imm32 to r32"),
		OTE_(0x00,	0xBC,	AM_IMM,		PO::NONE,			&REGS::ESP,	OT_GPREG8,	OT_IMM,		OT_NONE,	0,	"MOV",	"r32,imm32 Move imm32 to r32"),
		OTE_(0x00,	0xBD,	AM_IMM,		PO::NONE,			&REGS::EBP,	OT_GPREG8,	OT_IMM,		OT_NONE,	0,	"MOV",	"r32,imm32 Move imm32 to r32"),
		OTE_(0x00,	0xBE,	AM_IMM,		PO::NONE,			&REGS::ESI,	OT_GPREG8,	OT_IMM,		OT_NONE,	0,	"MOV",	"r32,imm32 Move imm32 to r32"),
		OTE_(0x00,	0xBF,	AM_IMM,		PO::NONE,			&REGS::EDI,	OT_GPREG8,	OT_IMM,		OT_NONE,	0,	"MOV",	"r32,imm32 Move imm32 to r32"),
	//	_OTE(0x00,	0xC6,	XAM_NONE,	PO::NONE,			NULL,		OT_PEND,	OT_PEND,	OT_NONE,	0,	"MOV",	"/0 r/m8,imm8 Move imm8 to r/m8"),
	//	_OTE(0x00,	0xC7,	AM_ENH,		PO::NONE,			NULL,		OT_PEND,	OT_PEND,	OT_NONE,	0,	"MOV",	"/0 r/m32,imm32 Move imm32 to r/m32"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTablePUSH[] =
	{
		OTE_(0x00,	0xFF,	AM_ENH,		PO::xFF::PUSH,		NULL,		OT_NONE,	OT_MRM,		OT_NONE,	4,	"PUSH",	"r/m16 r/m32"),
		OTE_(0x00,	0x50,	AM_INH,		PO::NONE,			&REGS::EAX,	OT_NONE,	OT_GPREG,	OT_NONE,	4,	"PUSH",	"r16:AX/r32:EAX"),
		OTE_(0x00,	0x51,	AM_INH,		PO::NONE,			&REGS::ECX,	OT_NONE,	OT_GPREG,	OT_NONE,	4,	"PUSH",	"r16:CX/r32:ECX"),
		OTE_(0x00,	0x52,	AM_INH,		PO::NONE,			&REGS::EDX,	OT_NONE,	OT_GPREG,	OT_NONE,	4,	"PUSH",	"r16:DX/r32:EDX"),
		OTE_(0x00,	0x53,	AM_INH,		PO::NONE,			&REGS::EBX,	OT_NONE,	OT_GPREG,	OT_NONE,	4,	"PUSH",	"r16:BX/r32:EBX"),
		OTE_(0x00,	0x54,	AM_INH,		PO::NONE,			&REGS::ESP,	OT_NONE,	OT_GPREG,	OT_NONE,	4,	"PUSH",	"r16:SP/r32:ESP"),
		OTE_(0x00,	0x55,	AM_INH,		PO::NONE,			&REGS::EBP,	OT_NONE,	OT_GPREG,	OT_NONE,	4,	"PUSH",	"r16:BP/r32:EBP"),
		OTE_(0x00,	0x56,	AM_INH,		PO::NONE,			&REGS::ESI,	OT_NONE,	OT_GPREG,	OT_NONE,	4,	"PUSH",	"r16:SI/r32:ESI"),
		OTE_(0x00,	0x57,	AM_INH,		PO::NONE,			&REGS::EDI,	OT_NONE,	OT_GPREG,	OT_NONE,	4,	"PUSH",	"r16:DI/r32:EDI"),
		OTE_(0x00,	0x6A,	AM_IMM,		PO::NONE,			NULL,		OT_NONE,	OT_IMM8,	OT_NONE,	1,	"PUSH",	"imm8"),
		OTE_(0x00,	0x68,	AM_IMM,		PO::NONE,			NULL,		OT_NONE,	OT_IMM,		OT_NONE,	4,	"PUSH",	"imm16/imm32"),
		OTE_(0x00,	0x0E,	AM_INH,		PO::NONE,			&REGS::CS,	OT_NONE,	OT_SREG,	OT_NONE,	2,	"PUSH",	"r16:CS"),
		OTE_(0x00,	0x16,	AM_INH,		PO::NONE,			&REGS::SS,	OT_NONE,	OT_SREG,	OT_NONE,	2,	"PUSH",	"r16:SS"),
		OTE_(0x00,	0x1E,	AM_INH,		PO::NONE,			&REGS::DS,	OT_NONE,	OT_SREG,	OT_NONE,	2,	"PUSH",	"r16:DS"),
		OTE_(0x00,	0x06,	AM_INH,		PO::NONE,			&REGS::ES,	OT_NONE,	OT_SREG,	OT_NONE,	2,	"PUSH",	"r16:ES"),
		OTE_(0x0F,	0xA0,	AM_INH,		PO::NONE,			&REGS::FS,	OT_NONE,	OT_SREG,	OT_NONE,	2,	"PUSH",	"r16:FS"),
		OTE_(0x0F,	0xA8,	AM_INH,		PO::NONE,			&REGS::GS,	OT_NONE,	OT_SREG,	OT_NONE,	2,	"PUSH",	"r16:GS"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTablePOP[] =
	{
		OTE_(0x00,	0x8F,	AM_ENH,		PO::x8F::POP,		NULL,		OT_MRM,		OT_NONE,	OT_NONE,	4,	"POP",	"r/m16 r/m32"),
		OTE_(0x00,	0x58,	AM_INH,		PO::NONE,			&REGS::EAX,	OT_GPREG,	OT_NONE,	OT_NONE,	4,	"POP",	"r16:AX/r32:EAX"),
		OTE_(0x00,	0x59,	AM_INH,		PO::NONE,			&REGS::ECX,	OT_GPREG,	OT_NONE,	OT_NONE,	4,	"POP",	"r16:CX/r32:ECX"),
		OTE_(0x00,	0x5A,	AM_INH,		PO::NONE,			&REGS::EDX,	OT_GPREG,	OT_NONE,	OT_NONE,	4,	"POP",	"r16:DX/r32:EDX"),
		OTE_(0x00,	0x5B,	AM_INH,		PO::NONE,			&REGS::EBX,	OT_GPREG,	OT_NONE,	OT_NONE,	4,	"POP",	"r16:BX/r32:EBX"),
		OTE_(0x00,	0x5C,	AM_INH,		PO::NONE,			&REGS::ESP,	OT_GPREG,	OT_NONE,	OT_NONE,	4,	"POP",	"r16:SP/r32:ESP"),
		OTE_(0x00,	0x5D,	AM_INH,		PO::NONE,			&REGS::EBP,	OT_GPREG,	OT_NONE,	OT_NONE,	4,	"POP",	"r16:BP/r32:EBP"),
		OTE_(0x00,	0x5E,	AM_INH,		PO::NONE,			&REGS::ESI,	OT_GPREG,	OT_NONE,	OT_NONE,	4,	"POP",	"r16:SI/r32:ESI"),
		OTE_(0x00,	0x5F,	AM_INH,		PO::NONE,			&REGS::EDI,	OT_GPREG,	OT_NONE,	OT_NONE,	4,	"POP",	"r16:DI/r32:EDI"),
		OTE_(0x00,	0x1F,	AM_INH,		PO::NONE,			&REGS::DS,	OT_SREG,	OT_NONE,	OT_NONE,	2,	"POP",	"r16:DS"),
		OTE_(0x00,	0x07,	AM_INH,		PO::NONE,			&REGS::ES,	OT_SREG,	OT_NONE,	OT_NONE,	2,	"POP",	"r16:ES"),
		OTE_(0x00,	0x17,	AM_INH,		PO::NONE,			&REGS::SS,	OT_SREG,	OT_NONE,	OT_NONE,	2,	"POP",	"r16:SS"),
		OTE_(0x0F,	0xA1,	AM_INH,		PO::NONE,			&REGS::FS,	OT_SREG,	OT_NONE,	OT_NONE,	2,	"POP",	"r16:FS"),
		OTE_(0x0F,	0xA9,	AM_INH,		PO::NONE,			&REGS::GS,	OT_SREG,	OT_NONE,	OT_NONE,	2,	"POP",	"r16:GS"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTableSTOS[] =
	{
		OTE_(0x00,	0xAA,	AM_INH,		PO::NONE,			&REGS::EDI,	OT_NONE,	OT_NONE,	OT_NONE,	1,	"STOS",		"m8 Store AL at address ES:(E)DI"),
		OTE_(0x00,	0xAB,	AM_INH,		PO::NONE,			&REGS::EDI,	OT_NONE,	OT_NONE,	OT_NONE,	4,	"STOS",		"m32 Store EAX at address ES:(E)DI"),
		OTE_(0x00,	0xAA,	AM_INH,		PO::NONE,			&REGS::EDI,	OT_NONE,	OT_NONE,	OT_NONE,	1,	"STOSB",	"Store AL at address ES:(E)DI"),
		OTE_(0x00,	0xAB,	AM_INH,		PO::NONE,			&REGS::EDI,	OT_NONE,	OT_NONE,	OT_NONE,	4,	"STOSD",	"Store EAX at address ES:(E)DI"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTableSUB[] =
	{
		OTE_(0x00,	0x2C,	AM_IMM,		PO::NONE,			&REGS::AL,	OT_GPREG8,	OT_IMM8,	OT_NONE,	1,	"SUB",	"ib SUB AL, imm8 Subtract imm8 from AL"),
		OTE_(0x00,	0x2D,	AM_IMM,		PO::NONE,			&REGS::EAX,	OT_GPREG,	OT_IMM,		OT_NONE,	4,	"SUB",	"id SUB EAX, imm32 Subtract imm32 from EAX"),
		OTE_(0x00,	0x80,	AM_IMM,		PO::x80::SUB8i8,	NULL,		OT_MRM8,	OT_IMM8,	OT_NONE,	1,	"SUB",	"/5 ib SUB r/m8,imm8 Subtract imm8 from r/m8"),
		OTE_(0x00,	0x81,	AM_IMM,		PO::x81::SUBi,		NULL,		OT_MRM,		OT_IMM,		OT_NONE,	4,	"SUB",	"/5 id SUB r/m32,imm32 Subtract imm32 from r/m32"),
		OTE_(0x00,	0x83,	AM_IMM,		PO::x83::SUBi8,		NULL,		OT_MRM,		OT_IMM8,	OT_NONE,	1,	"SUB",	"/5 ib SUB r/m32,imm8 Subtract sign-extended imm8 from r/m32"),
		OTE_(0x00,	0x28,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM8,	OT_MRMr8,	OT_NONE,	1,	"SUB",	"/r SUB r/m8,r8 Subtract r8 from r/m8"),
		OTE_(0x00,	0x29,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM,		OT_MRMr,	OT_NONE,	4,	"SUB",	"/r SUB r/m32,r32 Subtract r32 from r/m32"),
		OTE_(0x00,	0x2A,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr8,	OT_MRM8,	OT_NONE,	1,	"SUB",	"/r SUB r8,r/m8 Subtract r/m8 from r8"),
		OTE_(0x00,	0x2B,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr,	OT_MRM,		OT_NONE,	4,	"SUB",	"/r SUB r32,r/m32 Subtract r/m32 from r32"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo opcodesTableXOR[] =
	{
		OTE_(0x00,	0x34,	AM_IMM,		PO::NONE,			&REGS::AL,	OT_GPREG8,	OT_IMM8,	OT_NONE,	1,	"XOR",	"ib XOR AL, imm8 AL XOR imm8"),
		OTE_(0x00,	0x35,	AM_IMM,		PO::NONE,			&REGS::EAX,	OT_GPREG,	OT_IMM,		OT_NONE,	4,	"XOR",	"id XOR EAX, imm32 EAX XOR imm32"),
		OTE_(0x00,	0x80,	AM_IMM,		PO::x80::XOR8i8,	NULL,		OT_MRM8,	OT_IMM8,	OT_NONE,	1,	"XOR",	"/6 ib XOR r/m8,imm8 r/m8 XOR imm8"),
		OTE_(0x00,	0x81,	AM_IMM,		PO::x83::XORi8,		NULL,		OT_MRM,		OT_IMM8,	OT_NONE,	1,	"XOR",	"/6 id XOR r/m32,imm32 r/m32 XOR imm32"),
		OTE_(0x00,	0x83,	AM_IMM,		PO::x83::XORi8,		NULL,		OT_MRM,		OT_IMM8,	OT_NONE,	1,	"XOR",	"/6 ib XOR r/m32,imm8 r/m32 XOR imm8 (sign-extended)"),
		OTE_(0x00,	0x30,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM8,	OT_MRMr8,	OT_NONE,	1,	"XOR",	"/r XOR r/m8,r8 r/m8 XOR r8"),
		OTE_(0x00,	0x31,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRM,		OT_MRMr,	OT_NONE,	4,	"XOR",	"/r XOR r/m32,r32 r/m32 XOR r32"),
		OTE_(0x00,	0x32,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr8,	OT_MRM8,	OT_NONE,	1,	"XOR",	"/r XOR r8,r/m8 r8 XOR r/m8"),
		OTE_(0x00,	0x33,	AM_DIR,		PO::REGISTER,		NULL,		OT_MRMr,	OT_MRM,		OT_NONE,	4,	"XOR",	"/r XOR r32,r/m32 r8 XOR r/m8"),
		{0x00,	0x00,	AM_INV}
	};


	const OpcodeInfo *const opcodes32Table[] =
	{
		opcodesTableGeneral,
		opcodesTableADD,
		opcodesTableAND,
		opcodesTableCMP,
		opcodesTableCALL,
		opcodesTableDIV,
		opcodesTableJcc,
		opcodesTableJMP,
		opcodesTableMOV,
		opcodesTableMUL,
		opcodesTablePOP,
		opcodesTablePUSH,
		opcodesTableSUB,
		opcodesTableXOR,
		opcodesTableSTOS,
		NULL
	};

}}}
