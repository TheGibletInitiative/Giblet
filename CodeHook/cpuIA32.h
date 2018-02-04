//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include "cpuIA32_Instruction.h"



namespace Giblet { namespace CodeHook { namespace x86
{
	const size_t THUNK_BUFFER_SIZE = 256;

	const size_t MACHINE_SIZE = 4;

	////////////////////////////////////////////////////////////////////////////////
	//
	//
	//
	////////////////////////////////////////////////////////////////////////////////
	//	Instruction prefix ops
	const uint8_t	INST_PREFIX_LOCK		=	0xF0;
	const uint8_t	INST_PREFIX_REPN		=	0xF2;
	const uint8_t	INST_PREFIX_REP			=	0xF3;

	const uint8_t	INST_PREFIX_OPSIZE		=	0x66;

	const uint8_t	INST_PREFIX_ADSIZE		=	0x67;

	const uint8_t	INST_PREFIX_SEGMENT_ES	=	0x26;
	const uint8_t	INST_PREFIX_SEGMENT_CS	=	0x2e;
	const uint8_t	INST_PREFIX_SEGMENT_SS	=	0x36;
	const uint8_t	INST_PREFIX_SEGMENT_DS	=	0x3e;
	const uint8_t	INST_PREFIX_SEGMENT_FS	=	0x64;
	const uint8_t	INST_PREFIX_SEGMENT_GS	=	0x65;


#pragma pack(push, 1)
	class Assembler
	{
	private:

		//	FIXME: Use of this likely results in UB!
		union
		{
			uint32_t**	m_Address;
			uint8_t*	m_Byte;
			uint8_t*	m_Value8;
			uint16_t*	m_Value16;
			uint32_t*	m_Value32;
			uint64_t*	m_Value64;
		};


		void Relative32(uint8_t opcode, const void *address);
		void Absolute32(uint8_t opcode, const void *address);
		void Immediate32(uint8_t opcode, uint32_t value);

	public:

		Assembler();
		Assembler(void *instrPtr);
		Assembler(void *instrPtr, size_t instrOffset);
		void Jump(const void *address);
		void JumpIndirect(const void *address);
		void Call(const void *address);
		void Push(const void *address);
		void AddESP(size_t count);
		void Return();

		Assembler &operator=(const void *instrPtr);

	};
#pragma pack(pop)

	enum HOOK_ERROR
	{
		HOOK_SUCCESS,
		HOOK_NOMODULE,
		HOOK_NOFUNC,
		HOOK_NORELOCATE,
		HOOK_NOREDIRECT,
		HOOK_COREFAILED,
		HOOK_OUTOFMEMORY
	};


	size_t RelocateCode(
		uint8_t *dstPtr,
		const void *srcPtr,
		uint64_t minCodeBytes,
		uint64_t maxBufferSize);

	void DecodeStream(
		const void *ptr,
		size_t maxSize);

	HOOK_ERROR InstallHook(
		const char *const moduleName,
		const char *const functionName,
		const void *const proxyFunction,
		void **const thunkBufferOut);

	HOOK_ERROR InstallHook(
		const char *const moduleName,
		const char *const functionName,
		const void *const procAddress,
		const void *const proxyFunction,
		void **const thunkBufferOut);
}}}

