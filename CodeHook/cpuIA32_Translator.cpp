//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include "cpuIA32.h"
#include "cpuIA32_Instruction.h"


namespace Giblet { namespace CodeHook { namespace x86
{

	size_t SizeOfOperandByType(OPERAND_TYPE type)
	{
		switch(type)
		{
		case OT_GPREG:		//	FIXME
		case OT_GPREG8:		//	FIXME
		case OT_SREG:		//	FIXME
			assert(0);
			break;

		case OT_MRM:	return MACHINE_SIZE;
		case OT_MRM8:	return 1;
		case OT_MRMr:	break;	//	FIXME
		case OT_IMM:	return MACHINE_SIZE;
		case OT_IMM8:	return 1;
		case OT_IMM16:	return 2;
		case OT_IMM32:	return 4;
		case OT_IMM64:	return 8;
		case OT_REL:	return MACHINE_SIZE;
		case OT_REL8:	return 1;
		case OT_REL16:	return 2;
		case OT_REL32:	return 4;
		case OT_REL64:	return 8;

		case OT_NONE:
		case OT_PEND:
		default:
			assert(0);
			break;
		}

		return 0;
	}


#ifdef INCLUDE_TESTING
	const char *REGS::GetNameByValue(uint8_t value, const Register *const registers[])
	{
		while(*registers)
		{
			const Register *reg;

			reg = *(registers++);
			if(reg->GetValue() == value)
			{
				return reg->GetName();
			}
		}

		return "<???>";
	}
#endif

}}}
