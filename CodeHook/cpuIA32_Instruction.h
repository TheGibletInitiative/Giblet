//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include "cpuIA32_Opcode.h"


namespace Giblet { namespace CodeHook { namespace x86
{

	//	Instruction representation
	struct Instruction
	{
	private:

		const OpcodeInfo *FindOpcode(OpcodeIterator &opIter) const;
		size_t ProcessPrefix(OpcodeIterator &opIter);
		void ProcessOperand(OpcodeIterator &opIter, Operand &operand, OPERAND_TYPE type);
		void ProcessPostfix(OpcodeIterator &opIter);

#ifdef INCLUDE_TESTING
		char *DumpOperand(char *ptr, const Operand &operand) const;
		char *DumpOperandEx(char *ptr, const Operand &operand, size_t &currentCount) const;
#endif

	public:

		Instruction();

		void Reset();

		bool Decode(OpcodeIterator &opIter);
		uint8_t *Emit(uint8_t *outPtr);

		void Relocate(uint64_t address);

		uint64_t getSize() const
		{
			return m_CodeSize;
		}

#ifdef INCLUDE_TESTING
		void Dump() const;
#endif


	protected:

		//	Maxium number of prefix bytes
		static const size_t MAX_PREFIX_BYTES = 4;

		const OpcodeInfo*	m_Info;	//	Pointer to an opcode descriptor
		//OpcodeIterator		m_InstPtr;	//	Starting memory position of the instruction
		uint64_t			m_Address;
		uint64_t			m_CodeSize;	//	Number of bytes occupied by the instruction

		size_t				m_PrefixByteCount;	//	Number of prefix bytes present
		uint8_t				m_PrefixBytes[MAX_PREFIX_BYTES];	//	Prefix bytes

		PostByte			m_Postbyte;		//	ModRM postbyte (optional)
		Operand				m_Displacement;	//	displacement indicated in ModRM
		Operand				m_OperandDst;	//	destination operand
		Operand				m_OperandSrc1;	//	source value 1 operand
		Operand				m_OperandSrc2;	//	source value 2 operand (contains segment #)
	};

}}}
