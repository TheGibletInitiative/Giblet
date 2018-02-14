//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Types.h>
#include <string>
#include <string_view>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class BlockedContactManagementLink
	{
	public:

		using string_type = std::string;
		using string_view_type = std::string_view;


	public:

		BlockedContactManagementLink() = default;
		BlockedContactManagementLink(const BlockedContactManagementLink&) = delete;
		virtual ~BlockedContactManagementLink() = default;

		//
		virtual void Add(string_view_type clientId, string_view_type contactId) = 0;
		virtual void Remove(string_view_type clientId, string_view_type contactId) = 0;
	};

}}}
