//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include "cpuIA32_Translator.h"
#include "cpu_PostByte.h"
#include <cassert>


namespace Giblet { namespace CodeHook { namespace x86
{

	struct PostByte
	{
	public:

		struct MODE
		{
			static const uint8_t REG_IND		=	0x00;
			static const uint8_t REG_IND_DISP8	=	0x01;
			static const uint8_t REG_IND_DISP	=	0x02;
			static const uint8_t REG			=	0x03;
		};


		struct REGS
		{
			static const uint8_t PBYTE_SIB		=	0x04;
			static const uint8_t PBYTE_DISP		=	0x05;
			static const uint8_t SIBBASE_DISP	=	0x05;
			static const uint8_t SIBINDEX_NONE	=	0x04;
			static const uint8_t SIBREG_EBP		=	0x05;
		};


	private:

		struct MODRM
		{
			uint8_t		m_RegMode:3;
			uint8_t		m_RegOpcode:3;
			uint8_t		m_Mode:2;
		};


		struct SIB
		{
			uint8_t		m_Base:3;
			uint8_t		m_Index:3;
			uint8_t		m_Scale:2;
		};


	public:

		void Reset();

		size_t assign(OpcodeIterator &iter, bool autoInc);

		//	Mode/Register/Memory
		bool GetHasModRM() const
		{
			return m_ModRM.HasData();
		}

		uint8_t GetModRMValue() const
		{
			return m_ModRM.GetDataValue();
		}

		uint8_t GetMode() const
		{
			assert(GetHasModRM());

			return m_ModRM.m_BitData.m_Mode;
		}

		uint8_t GetRegisterOrOpcode() const
		{
			assert(GetHasModRM());

			return m_ModRM.m_BitData.m_RegOpcode;
		}

		uint8_t GetRegisterOrMode() const
		{
			assert(GetHasModRM());

			return m_ModRM.m_BitData.m_RegMode;
		}

		//	Scale/Index/Base extension
		bool GetHasSIB() const
		{
			return m_SIB.HasData();
		}

		uint8_t GetSIBValue() const
		{
			return m_SIB.GetDataValue();
		}

		uint8_t GetBase() const
		{
			return m_SIB.m_BitData.m_Base;
		}

		uint8_t GetIndex() const
		{
			return m_SIB.m_BitData.m_Index;
		}

		uint8_t GetScale() const
		{
			return m_SIB.m_BitData.m_Scale;
		}

		bool GetHasBase() const
		{
			return (!(REGS::SIBBASE_DISP == GetBase() && MODE::REG_IND == GetMode()));
		}

		bool GetHasIndex() const
		{
			return REGS::SIBINDEX_NONE != GetIndex();
		}

		bool IsIndirect() const;


	private:

		PostByteT<uint8_t, MODRM>	m_ModRM;
		PostByteT<uint8_t, SIB>		m_SIB;
	};

}}}
