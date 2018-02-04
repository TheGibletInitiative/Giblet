//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include <cstdint>
#include <algorithm>
#include <utility>

namespace
{
	bool ApplyFlatPatch(uint8_t* address, const uint8_t* begin, const uint8_t* end)
	{
		const auto size = end - begin;

		DWORD oldPageSettings = 0;
		if (!VirtualProtect(address, size, PAGE_READWRITE, &oldPageSettings))
		{
			MessageBoxA(nullptr, "Unable to access memory for flat patch", "Page Settings Error", MB_OK);
			return false;
		}

		std::copy(begin, end, address);

		VirtualProtect(address, size, oldPageSettings, nullptr);

		return true;
	}


	const uint8_t returnTrueInEax[] = { 0xb8, 0x01, 0x00, 0x00, 0x00, 0xc2, 0x04, 0x00 };	//	mov eax, 1 / ret 4
}




void ApplyCopyPatcha()
{
	ApplyFlatPatch(reinterpret_cast<uint8_t*>(0x4c3603), std::begin(returnTrueInEax), std::end(returnTrueInEax));
}

