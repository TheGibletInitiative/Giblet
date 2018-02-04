//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include "cpuIA32_Postbyte.h"


namespace Giblet { namespace CodeHook { namespace x86
{

	void PostByte::Reset()
	{
		m_ModRM.Reset();
		m_SIB.Reset();
	}


	size_t PostByte::assign(OpcodeIterator &iter, bool autoInc)
	{
		size_t	count = 0;

		m_ModRM.SetDataValue(iter[0]);
		count++;

		//	Check for SIB following MODRW postbyte
		if(MODE::REG != GetMode() && REGS::PBYTE_SIB == GetRegisterOrMode())
		{
			m_SIB.SetDataValue(iter[1]);

			//if(REGS::SIBSCALE_INVALID == m_SIB.m_BitData.m_Index)
			//{
			//	return 0;
			//}

			count++;
		}

		if(autoInc)
		{
			iter.NextI8();
			if(GetHasSIB())
			{
				iter.NextI8();
			}
		}

		return count;
	}




	bool PostByte::IsIndirect() const
	{
		bool	result;

		result = false;

		switch(GetMode())
		{
		case PostByte::MODE::REG_IND:
		case PostByte::MODE::REG_IND_DISP:
		case PostByte::MODE::REG_IND_DISP8:
			result = true;
			break;

		case PostByte::MODE::REG:
			break;

		default:
			assert(0);
			break;
		}

		return result;
	}

}}}
