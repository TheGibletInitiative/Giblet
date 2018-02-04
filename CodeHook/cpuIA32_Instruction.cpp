//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include "cpuIA32.h"
#include <cstdio>


namespace Giblet { namespace CodeHook { namespace x86
{

	Instruction::Instruction()
	{
		Reset();
	}


	const OpcodeInfo *Instruction::FindOpcode(OpcodeIterator &opIterIn) const
	{
		const OpcodeInfo	*const *tables(opcodes32Table);

		OpcodeIterator		iter(opIterIn);
		while(*tables)
		{
			const OpcodeInfo *info(*(tables++));

			while(AM_INV != info->GetAddressingMode())
			{
				//	Handle opcode
				if(info->GetPrefix() != 0 && info->GetPrefix() == iter[0] && info->GetOpcode() == iter[1])
				{
					iter.NextI8();
					iter.NextI8();
				}

				else if(info->GetPrefix() == 0 && info->GetOpcode() == iter[0])
				{
					iter.NextI8();
				}

				else
				{
					info++;
					continue;
				}

				//	If this is an extended opcode go process the postbytes
				if(PO::NONE != info->GetPostbyte())
				{
					PostByte	modRM;
					size_t		size;

					size = modRM.assign(iter, false);
					if(0 == size)
					{
						return NULL;
					}

					//	Make sure that the comm
					if(		PO::REGISTER != info->GetPostbyte()
						&&	info->GetPostbyte() != modRM.GetRegisterOrOpcode())
					{
						iter = opIterIn;
						info++;
						continue;
					}
				}

				opIterIn = iter;
				return info;
			}
		}

		return NULL;
	}




	size_t Instruction::ProcessPrefix(OpcodeIterator &iter)
	{
		m_PrefixByteCount = 0;

		//	collect the prefix bytes
		while(true)
		{
			switch(iter[0])
			{
			case INST_PREFIX_LOCK:
			case INST_PREFIX_REPN:
			case INST_PREFIX_REP:
			case INST_PREFIX_OPSIZE:
			case INST_PREFIX_ADSIZE:
			case INST_PREFIX_SEGMENT_ES:
			case INST_PREFIX_SEGMENT_CS:
			case INST_PREFIX_SEGMENT_SS:
			case INST_PREFIX_SEGMENT_DS:
			case INST_PREFIX_SEGMENT_FS:
			case INST_PREFIX_SEGMENT_GS:
				m_PrefixBytes[m_PrefixByteCount++] = iter.NextI8();
				continue;
			}
			break;
		}

		return m_PrefixByteCount;
	}


	void Instruction::ProcessOperand(OpcodeIterator& opIter, Operand& operand, OPERAND_TYPE type)
	{
		size_t size = 0;

		switch(type)
		{
		case OT_GPREG:	//	Explicit register specified in the opcode info
		case OT_GPREG8:	//	Explicit register specified in the opcode info
		case OT_SREG:	//	Explicit register specified in the opcode info
			operand.create(type);
			break;

		case OT_MRMr:	//	Implicit register specified in mod r/m postbyte
		case OT_MRMsr:
		case OT_MRMr8:
			assert(PO::REGISTER == m_Info->GetPostbyte());

		case OT_MRM:	//	Mod/RM using machine word spec
		case OT_MRM8:	//	Mod/RM explicit 8bit values
			assert(NULL == m_Info->GetExplicitRegister());
			assert(m_Postbyte.GetHasModRM());
			assert(PO::NONE != m_Info->GetPostbyte());
			operand.create(type);
			break;

		case OT_IMM:	//	Immediate value. Size = machine word
		case OT_IMM8:	//	Immediate value. Size = 8 bits
		case OT_IMM16:	//	Immediate value. Size = 16 bits
		case OT_IMM32:	//	Immediate value. Size = 32 bits
		case OT_IMM64:	//	Immediate value. Size = 64 bits
			size = SizeOfOperandByType(type);
			break;

		case OT_REL:	//	Relative value. Size = machine word
		case OT_REL8:	//	Relative value. Size = 8 bits
		case OT_REL16:	//	Relative value. Size = 16 bits
		case OT_REL32:	//	Relative value. Size = 32 bits
		case OT_REL64:	//	Relative value. Size = 64 bits
			size = SizeOfOperandByType(type);
			break;

		case OT_NONE:	//	None
			break;

		case OT_PEND:
			assert(0);
			break;

		default:
			assert(0);
			break;
		}

		switch(size)
		{
		case 1:
			operand.setValue<uint8_t>(type, opIter.NextI8());
			break;

		case 2:
			operand.setValue<uint16_t>(type, opIter.NextI16());
			break;

		case 4:
			operand.setValue<uint32_t>(type, opIter.NextI32());
			break;

		case 8:
			operand.setValue<uint64_t>(type, opIter.NextI64());
			break;
		}

	}


	void Instruction::ProcessPostfix(OpcodeIterator &opIter)
	{
		//	Initialize
		m_Postbyte.assign(opIter, true);

		//	Make sure that the comm
		assert(
			PO::REGISTER == m_Info->GetPostbyte() ||
			m_Postbyte.GetRegisterOrOpcode() == m_Info->GetPostbyte());

		//	Check for SIB following MODRW postbyte
		switch(m_Postbyte.GetMode())
		{
		case PostByte::MODE::REG_IND:
		case PostByte::MODE::REG_IND_DISP:
		case PostByte::MODE::REG_IND_DISP8:
			switch(m_Postbyte.GetMode())
			{
			case PostByte::MODE::REG_IND:
				if(PostByte::REGS::PBYTE_DISP == m_Postbyte.GetRegisterOrMode())
				{
					ProcessOperand(opIter, m_Displacement, OT_IMM);
				}
				else if(m_Postbyte.GetHasSIB() && PostByte::REGS::SIBBASE_DISP == m_Postbyte.GetBase())
				{
					ProcessOperand(opIter, m_Displacement, OT_IMM);
				}
				break;

			case PostByte::MODE::REG_IND_DISP:
				ProcessOperand(opIter, m_Displacement, OT_IMM);
				break;

			case PostByte::MODE::REG_IND_DISP8:
				ProcessOperand(opIter, m_Displacement, OT_IMM8);
				break;

			default:
				assert(0);
				break;
			}

			break;

		case PostByte::MODE::REG:
			break;

		default:
			assert(0);
			break;
		}
	}


	void Instruction::Reset()
	{
		m_Info = NULL;

		m_Address = 0;

		m_CodeSize = 0;
		m_PrefixByteCount = 0;
		memset(m_PrefixBytes, 0, sizeof(m_PrefixBytes));

		m_Postbyte.Reset();
	}




	bool Instruction::Decode(OpcodeIterator &opIterIn)
	{
		const OpcodeInfo		*info;
		OpcodeIterator	iter;

		//	Reset the info
		Reset();
		m_Address = opIterIn.getAddress();
		iter = opIterIn;

		ProcessPrefix(iter);
		info = FindOpcode(iter);
		if(NULL != info)
		{
			//	Check for simple terminator
			if(false != info->GetIsTerminator())
			{
				Reset();
				iter.clear();
				return false;
			}


			//	We have our opcode, let's process it
			m_Info = info;

			if(PO::NONE != info->GetPostbyte())
			{
				ProcessPostfix(iter);
			}

			ProcessOperand(iter, m_OperandDst, info->GetOperandTypeDest());
			ProcessOperand(iter, m_OperandSrc1, info->GetOperandTypeSrc1());
			ProcessOperand(iter, m_OperandSrc2, info->GetOperandTypeSrc2());

			opIterIn = iter;
			m_CodeSize = iter.getAddress() - m_Address;
			return true;
		}
		Reset();
		return false;
	};




	void Instruction::Relocate(uint64_t address)
	{
		for(int i = 0; i < 3; i++)
		{
			Operand *op;
			switch(i)
			{
			case 0:		op = &m_OperandDst;		break;
			case 1:		op = &m_OperandSrc1;	break;
			case 2:		op = &m_OperandSrc2;	break;
			default:
				op = NULL;
				assert(0);
			}

			if(false != IsRelative(op->getType()))
			{
				uint64_t	value;

				value = (m_Address + m_CodeSize + op->getValue());
				value -= (address + m_CodeSize);

				switch(op->getSize()) {
				case 1:		op->setValue<uint8_t>(op->getType(), static_cast<uint8_t>(value));	break;
				case 2:		op->setValue<uint16_t>(op->getType(), static_cast<uint16_t>(value));	break;
				case 4:		op->setValue<uint32_t>(op->getType(), static_cast<uint32_t>(value));	break;
				case 8:		op->setValue<uint64_t>(op->getType(), static_cast<uint64_t>(value));	break;
				default:
					assert(0);
				}
			}
		}

		m_Address = address;
	}


	static uint8_t *AppendOperand(uint8_t *ptr, const Operand &operand)
	{
		size_t	size = operand.getSize();
		if(size)
		{
			uint64_t intVal =operand.getValue();

			while(size > 0)
			{
				*(ptr++) = static_cast<uint8_t>(intVal & 0xff);
				intVal >>= 8;
				size--;
			}
		}

		return ptr;
	}



	uint8_t *Instruction::Emit(uint8_t *ptr)
	{
	#ifdef _DEBUG
		uint8_t *dbgSavedPtr = ptr;
	#endif

		//
		for(size_t index = 0; index < m_PrefixByteCount; index++)
		{
			*(ptr++) = m_PrefixBytes[index];
		}

		//
		if(0 != m_Info->GetPrefix())
		{
			*(ptr++) = m_Info->GetPrefix();
		}
		*(ptr++) = m_Info->GetOpcode();

		//
		if(false != m_Postbyte.GetHasModRM())
		{
			*(ptr++) = m_Postbyte.GetModRMValue();
		}

		//
		if(false != m_Postbyte.GetHasSIB())
		{
			*(ptr++) = m_Postbyte.GetSIBValue();
		}

		ptr = AppendOperand(ptr, m_Displacement);
		ptr = AppendOperand(ptr, m_OperandDst);
		ptr = AppendOperand(ptr, m_OperandSrc1);
		ptr = AppendOperand(ptr, m_OperandSrc2);

		assert((ptr - dbgSavedPtr) == getSize());

		return ptr;
	}




#ifdef INCLUDE_TESTING
	static char *AppendOperand(char *ptr, const Operand &operand)
	{
		uint64_t intVal;
		size_t	size;
	
		intVal = operand.getValue();
		size = operand.getSize();
		if(0 != size) {
			ptr += sprintf(ptr, " :");
			while(size > 0) {
				ptr += sprintf(ptr, " %02I64X", intVal & 0xff);
				intVal >>= 8;
				size--;
			}
		}

		return ptr;
	}


	char *Instruction::DumpOperand(char *ptr, const Operand &operand) const
	{
		uint64_t	value;
	
		value = operand.getValue();
		if(false != IsRelative(operand.getType())) {
			value += m_Address;
			value += m_CodeSize;

		}

		switch(operand.getSize()) {
		case 0:
			assert(0);
			break;

		case 1:
			ptr += sprintf(ptr, "0%02I64Xh", value & 0xff);
			break;

		case 2:
			ptr += sprintf(ptr, "0%04I64Xh", value & 0xffff);
			break;

		case 4:
			ptr += sprintf(ptr, "0%08I64Xh", value & 0xffffffff);
			break;

		case 8:
			ptr += sprintf(ptr, "0%10I64Xh", value);
			break;

		default:
			assert(0);
			break;
		}

		return ptr;
	}


	char *Instruction::DumpOperandEx(
		char *ptr,
		const Operand &operand,
		size_t &currentCount) const
	{
		*ptr = 0;

		//	Check operand type
		if(OT_NONE == operand.getType()) {
			return ptr;
		}

		//	Check to prepend a comma
		if(0 != currentCount) {
			*(ptr++) = ',';
			*ptr = 0;
		}

		switch(operand.getType()) {
		case OT_GPREG:
		case OT_GPREG8:
		case OT_SREG:
			assert(NULL != m_Info->GetExplicitRegister());
			if(NULL != m_Info->GetExplicitRegister()) {
				ptr += sprintf(ptr, "%s",  m_Info->GetExplicitRegister()->GetName());
			}
			break;

		case OT_MRMsr:
			assert(NULL == m_Info->GetExplicitRegister());
			ptr += sprintf(
				ptr,
				"%s",
				REGS::GetNameByValue(
					m_Postbyte.GetRegisterOrOpcode(),
					REGS::SegmentRegisters));
			break;

		case OT_MRMr:
			assert(NULL == m_Info->GetExplicitRegister());
			ptr += sprintf(
				ptr,
				"%s",
				REGS::GetNameByValue(
					m_Postbyte.GetRegisterOrOpcode(),
					REGS::GP32Registers));
			break;

		case OT_MRMr8:
			assert(NULL == m_Info->GetExplicitRegister());
			ptr += sprintf(
				ptr,
				"%s",
				REGS::GetNameByValue(
					m_Postbyte.GetRegisterOrOpcode(),
					REGS::GP8Registers));
			break;

		case OT_MRM8:
			//ptr += sprintf(ptr, "{OT_MRM8}");

		case OT_MRM:
			if(false != m_Postbyte.IsIndirect()) {
				*(ptr++) = '[';
			}

			if(false == m_Postbyte.GetHasSIB()) {

				//	Handle special Mod R/M combo that specifies a 32bit displacemnt instead of a 
				if(		PostByte::MODE::REG_IND == m_Postbyte.GetMode()
					&&	PostByte::REGS::PBYTE_DISP == m_Postbyte.GetRegisterOrMode())
				{
					assert(OT_NONE != m_Displacement.getType());
					if(OT_NONE != m_Displacement.getType()) {
						ptr += sprintf(ptr, "0%I64Xh", m_Displacement.getValue());
					}
				}

				//	Handle register		FIXME
				else
				{
					ptr += sprintf(
						ptr,
						"%s",
						REGS::GetNameByValue(
							m_Postbyte.GetRegisterOrMode(),
							OT_MRM8 == operand.getType() ? REGS::GP8Registers : REGS::GP32Registers));
					if(0 != m_Displacement.getSize()) {
						__int64 value;

						value = m_Displacement.getSignedValue();
						if(0 != value) {
							ptr += sprintf(
								ptr,
								"%+I64d",
								value);
						}
					}
				}
			}
			else {
				size_t	scale;

				scale = (1 << m_Postbyte.GetScale());

				if(true == m_Postbyte.GetHasBase()) {
					ptr += sprintf(ptr, "%s", REGS::GetNameByValue(m_Postbyte.GetBase(), REGS::GP32Registers));
				}

				if(true == m_Postbyte.GetHasIndex()) {
					ptr += sprintf(ptr, "%s%s",
						true == m_Postbyte.GetHasBase() ? "+" : "",
						REGS::GetNameByValue(m_Postbyte.GetIndex(), REGS::GP32Registers));
				}

				if(scale > 1) {
					ptr += sprintf(ptr, "*%d", scale);
				}

				if(OT_NONE != m_Displacement.getType() && 0 != m_Displacement.getValue()) {
					ptr += sprintf(ptr, "+0%I64Xh", m_Displacement.getValue());
				}
			}


			if(false != m_Postbyte.IsIndirect()) {
				*(ptr++) = ']';
			}

			break;

		case OT_IMM:
		case OT_IMM8:
		case OT_IMM16:
		case OT_IMM32:
		case OT_IMM64:
			ptr = DumpOperand(ptr, operand);
			break;

		case OT_REL:
		case OT_REL8:
		case OT_REL16:
		case OT_REL32:
		case OT_REL64:
			ptr = DumpOperand(ptr, operand);
			break;

		case OT_NONE:
		case OT_PEND:
		default:
			assert(0);
			break;
		}


		currentCount++;
		*ptr = 0;

		return ptr;
	}


	void Instruction::Dump() const
	{
		size_t index;
		char	rawBuffer[4096];
		char	srcBuffer[4096];
		char	*ptr;

		ptr = rawBuffer;

		//
		ptr += sprintf(ptr, "%08I64X:", m_Address);

		//
		for(index = 0; index < m_PrefixByteCount; index++) {
			ptr += sprintf(ptr, " %02X", m_PrefixBytes[index]);
		}

		//
		if(0 != m_Info->GetPrefix()) {
			ptr += sprintf(ptr, " %02X", m_Info->GetPrefix());
		}
		ptr += sprintf(ptr, " %02X", m_Info->GetOpcode());

		//
		if(false != m_Postbyte.GetHasModRM()) {
			ptr += sprintf(ptr, " %02X", m_Postbyte.GetModRMValue());
		}

		//
		if(false != m_Postbyte.GetHasSIB()) {
			ptr += sprintf(ptr, " %02X", m_Postbyte.GetSIBValue());
		}

		ptr = AppendOperand(ptr, m_Displacement);
		ptr = AppendOperand(ptr, m_OperandDst);
		ptr = AppendOperand(ptr, m_OperandSrc1);
		ptr = AppendOperand(ptr, m_OperandSrc2);
		*ptr = 0;


		//
		size_t count = 0;
		ptr = srcBuffer;
		ptr = DumpOperandEx(ptr, m_OperandDst, count);
		ptr = DumpOperandEx(ptr, m_OperandSrc1, count);
		ptr = DumpOperandEx(ptr, m_OperandSrc2, count);


		//	FIXME: Need tracing back in
		//Trace(
		//	"%-44s"
		//	"%-8s"
		//	"%-30s"
		//	"%.*s\n",
		//	rawBuffer,
		//	m_Info->GetOperandName(),
		//	srcBuffer,
		//	60,
		//	m_Info->GetOperandDesc());
	}
#endif

}}}
