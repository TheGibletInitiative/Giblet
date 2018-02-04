//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include "cpuIA32.h"


namespace Giblet { namespace CodeHook { namespace x86
{

	Assembler::Assembler()
		: m_Address(NULL)
	{}


	Assembler::Assembler(void *instrPtr)
	{
		*this = instrPtr;
	}


	Assembler::Assembler(void *instrPtr, size_t instrOffset)
	{
		*this = (static_cast<uint8_t*>(instrPtr) + instrOffset);
	}


	Assembler &Assembler::operator=(const void *instrPtr)
	{
		m_Address = reinterpret_cast<uint32_t**>(const_cast<void*>(instrPtr));

		return *this;
	}


	void Assembler::Relative32(uint8_t opcode, const void *address)
	{
		*(m_Byte++) = opcode;
		*(m_Address++) = (size_t*)(((char*)address) - (((char*)m_Address) + sizeof(*m_Address)));;
	}


	void Assembler::Absolute32(uint8_t opcode, const void *address)
	{
		*(m_Byte++) = opcode;
		*(m_Address++) = (size_t*)address;
	}


	void Assembler::Immediate32(uint8_t opcode, uint32_t value)
	{
		*(m_Byte++) = opcode;
		*(m_Value32++) = value;
	}


	void Assembler::Jump(const void *address)
	{
		Relative32(0xe9, address);
	}


	void Assembler::JumpIndirect(const void *address)
	{
		*(m_Byte++) = 0xff;

		Absolute32(0x25, address);
	}


	void Assembler::Call(const void *address)
	{
		Relative32(0xe8, address);
	}


	void Assembler::Push(const void *address)
	{
		Absolute32(0x68, address);
	}


	void Assembler::AddESP(size_t count)
	{
		*(m_Byte++) = 0x81;

		Immediate32(0xc4, count);
	}


	void Assembler::Return()
	{
		*(m_Byte++) = 0xc3;
	}

}}}
