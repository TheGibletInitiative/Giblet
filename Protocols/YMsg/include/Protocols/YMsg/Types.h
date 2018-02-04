//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once

namespace Giblet { namespace Protocols { namespace YMsg
{

	enum class TextEncoding
	{
		None = 1,
		UTF8 = 2
	};

	template<class Type_>
	Type_ to_enum(std::string_view value)
	{
		return static_cast<Type_>(std::atoi(value.data()));
	}


}}}
