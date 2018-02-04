//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include "cpu_Win32MemoryBlock.h"
#include <cassert>


namespace Giblet { namespace CodeHook { namespace x86
{

	MemoryBlock::MemoryBlock(const void *memory)
	{
		assert(NULL != memory);
		m_Memory = memory;
		m_LockedProtection = 0;
	}


	MemoryBlock::~MemoryBlock()
	{
		if(m_LockedProtection)
		{
			Unlock();
		}
	}


	bool MemoryBlock::Lock(DWORD prot)
	{
		assert(0 == m_LockedProtection);

		size_t result;
		MEMORY_BASIC_INFORMATION	mbi;

		if(0 != m_LockedProtection)
		{
			return false;
		}

		//	Get the memory information
		result = ::VirtualQuery(m_Memory, &mbi, sizeof(mbi));
		if(sizeof(mbi) != result)
		{
			return false;
		}

		//	Change the protection
		result = ::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, prot, &m_LockedProtection);
		assert(0 != m_LockedProtection);
		if (FALSE == result)
		{
			return false;
		}

		m_LockedProtection = mbi.Protect;

		return true;
	}


	bool MemoryBlock::Unlock()
	{
		assert(0 != m_LockedProtection);

		size_t result;
		MEMORY_BASIC_INFORMATION	mbi;

		if(0 == m_LockedProtection)
		{
			return false;
		}

		//	Get the memory information
		result = ::VirtualQuery(m_Memory, &mbi, sizeof(mbi));
		if(sizeof(mbi) != result)
		{
			return false;
		}

		//	Change the protection
		result = ::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, m_LockedProtection, &m_LockedProtection);
		m_LockedProtection = 0;
		if (FALSE == result)
		{
			return false;
		}

		return true;
	}

}}}
