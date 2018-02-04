//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once


namespace Giblet { namespace CodeHook { namespace x86
{

	class MemoryBlock
	{
	public:

		explicit MemoryBlock(const void *memory);
		~MemoryBlock();


		bool Lock(DWORD prot = PAGE_READWRITE);
		bool Unlock();


	private:

		const void*	m_Memory;
		DWORD		m_LockedProtection;
	};

}}}
