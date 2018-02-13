//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Types.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	struct ContactManagementLink
	{
		using string_type = std::string;			//	should be a UTF8 string
		using string_view_type = std::string_view;	//	should be a UTF8 string


		ContactManagementLink() = default;
		ContactManagementLink(const ContactManagementLink&) = delete;
		virtual ~ContactManagementLink() = default;

		//	Triggered by client
		virtual void DenyRequest(string_view_type clientId, string_view_type contactId, string_view_type message) = 0;
		virtual void AddContact(string_view_type clientId, string_view_type contactId, string_view_type group, string_view_type message) = 0;
		virtual void RemoveContact(string_view_type clientId, string_view_type contactId, string_view_type group) = 0;
		virtual void RenameGroup(string_type currentName, string_type newName) = 0;
	};

}}}
