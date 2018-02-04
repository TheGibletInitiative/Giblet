//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include "cpuIA32_Register.h"
#include "cpuIA32_Postbyte.h"
#include "cpuIA32_Translator.h"


namespace Giblet { namespace CodeHook { namespace x86
{

	enum OPERAND_TYPE
	{
		OT_GPREG	=	0x00,		//	Explicit gp register specified in the opcode info
		OT_GPREG8	=	0x01,		//	Explicit gp register specified in the opcode info
		OT_SREG		=	0x02,		//	Explicit segment register specified in the opcode info

		OT_MRMr		=	0x10,		//	Implicit register specified in mod r/m postbyte
		OT_MRMsr	=	0x11,
		OT_MRMr8	=	0x12,		//	Implicit register specified in mod r/m postbyte
		OT_MRM		=	0x13,		//	Mod/RM using machine word spec
		OT_MRM8		=	0x14,		//	Mod/RM explicit 8bit values

		OT_IMM		=	0x20,		//	Immediate value. Size = machine word
		OT_IMM8		=	0x21,		//	Immediate value. Size = 8 bits
		OT_IMM16	=	0x22,		//	Immediate value. Size = 16 bits
		OT_IMM32	=	0x23,		//	Immediate value. Size = 32 bits
		OT_IMM64	=	0x24,		//	Immediate value. Size = 64 bits

		OT_REL		=	0x25,		//	Relative value. Size = machine word
		OT_REL8		=	0x26,		//	Relative value. Size = 8 bits
		OT_REL16	=	0x27,		//	Relative value. Size = 16 bits
		OT_REL32	=	0x28,		//	Relative value. Size = 32 bits
		OT_REL64	=	0x29,		//	Relative value. Size = 64 bits

		OT_NONE		=	0xff,		//	None
		OT_PEND		=	0xfe		//	Explicit register specified in the opcode info
	};


	class Operand
	{
	public:

		Operand()
			:
			m_Type(OT_NONE),
			m_Size(0),
			m_IntValue(0)
		{}

		void create(OPERAND_TYPE type)
		{
			m_Type = type;
			m_Size = 0;
			m_IntValue = 0;
		}

		void setValuedouble(OPERAND_TYPE type, double value)
		{
			m_Type = type;
			m_Size = sizeof(value);
			m_FloatValue = value;
		}

		void setValue(OPERAND_TYPE type, float value)
		{
			m_Type = type;
			m_Size = sizeof(value);
			m_FloatValue = value;
		}

		template<typename _TYPE> void setValue(OPERAND_TYPE type, _TYPE value)
		{
			m_Type = type;
			m_Size = sizeof(value);
			m_IntValue = value;
		}

		OPERAND_TYPE getType() const
		{
			return m_Type;
		}

		uint64_t getValue() const
		{
			return m_IntValue;
		}

		int64_t getSignedValue() const
		{

			switch(m_Size)
			{
			case 1:
				return (__int8)m_IntValue;

			case 2:
				return (__int16)m_IntValue;

			case 4:
				return (__int32)m_IntValue;

			case 8:
				return (__int64)m_IntValue;
			}

			throw std::runtime_error("unknown value size");
		}

		size_t getSize() const
		{
			return m_Size;
		}


	private:

		OPERAND_TYPE	m_Type; 
		size_t			m_Size;
		union
		{
			uint64_t		m_IntValue;
			double		m_FloatValue;
		};
	};


	bool IsRegister(OPERAND_TYPE type);
	bool IsModRM(OPERAND_TYPE type);
	bool IsRelative(OPERAND_TYPE type);
	bool IsImmediate(OPERAND_TYPE type);

	size_t SizeOfOperandByType(OPERAND_TYPE type);

}}}
