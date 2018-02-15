//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Types.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class ProfileManagementLink
	{
	public:

		using string_type = std::string;
		using string_view_type = std::string_view;


	public:

		ProfileManagementLink() = default;
		ProfileManagementLink(const ProfileManagementLink&) = delete;
		virtual ~ProfileManagementLink() = default;

		//	Actions executed by the client
		virtual void Add(string_view_type profileId) = 0;
		virtual void Activate(string_view_type id) = 0;
		virtual void Deactivate(string_view_type id) = 0;
	};

}}}
