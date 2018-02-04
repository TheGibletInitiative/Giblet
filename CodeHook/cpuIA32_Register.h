//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <cstdint>


////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
namespace Giblet { namespace CodeHook { namespace x86
{

	enum REGISTER_TYPE
	{
		RT_GP8,
		RT_GP16,
		RT_GP32,
		RT_GP64,
		RT_SEG,
		RT_MMX,
		RT_FP
	};


	struct Register
	{
	public:

		Register(
			uint8_t value,
			REGISTER_TYPE type,
			size_t size
#ifdef INCLUDE_TESTING
			, const char *name
#endif
			)
			:
			m_Value(value),
			m_Type(type),
			m_Size(size)
#ifdef INCLUDE_TESTING
			, m_Name(name)
#endif
		{}

		Register(const Register &src) = default;


		bool operator!=(const Register &src) const
		{
			return !(GetType() == src.GetType() && GetValue() == src.GetValue());
		}

		uint8_t GetValue() const
		{
			return m_Value;
		}

		REGISTER_TYPE GetType() const
		{
			return m_Type;
		}

#ifdef INCLUDE_TESTING
		const char *GetName() const
		{
			return m_Name;
		}
#endif

		size_t GetSize() const
		{
			return m_Size;
		}


	private:

		uint8_t			m_Value;
		REGISTER_TYPE	m_Type;
		size_t			m_Size;
#ifdef INCLUDE_TESTING
		const char		*m_Name;
#endif
	};


	namespace REGS
	{

		//	8 bit general purposes registers
		extern const Register AL;
		extern const Register CL;
		extern const Register DL;
		extern const Register BL;
		extern const Register AH;
		extern const Register CH;
		extern const Register DH;
		extern const Register BH;

		//	16 bit general purposes registers
		extern const Register AX;
		extern const Register CX;
		extern const Register DX;
		extern const Register BX;
		extern const Register SP;
		extern const Register BP;
		extern const Register SI;
		extern const Register DI;

		//	32 bit general purposes registers
		extern const Register EAX;
		extern const Register ECX;
		extern const Register EDX;
		extern const Register EBX;
		extern const Register ESP;
		extern const Register EBP;
		extern const Register ESI;
		extern const Register EDI;

		//	Segment registers
		extern const Register ES;
		extern const Register CS;
		extern const Register SS;
		extern const Register DS;
		extern const Register FS;
		extern const Register GS;
		extern const Register R1;	//	Reserved
		extern const Register R2;	//	Reserved

		extern const Register MM0;
		extern const Register MM1;
		extern const Register MM2;
		extern const Register MM3;
		extern const Register MM4;
		extern const Register MM5;
		extern const Register MM6;
		extern const Register MM7;

		extern const Register *const GP8Registers[];
		extern const Register *const GP16Registers[];
		extern const Register *const GP32Registers[];
		extern const Register *const SegmentRegisters[];
		extern const Register *const MMXRegisters[];

#ifdef INCLUDE_TESTING
		extern const char *GetNameByValue(uint8_t value, const Register *const registers[]);
#endif
	};

}}}
