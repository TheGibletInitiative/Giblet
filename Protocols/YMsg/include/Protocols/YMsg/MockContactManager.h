//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/ContactManager.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class MockContactManager : public ContactManager
	{
	public:

		using ContactManager::ContactManager;


		void DenyRequest(string_view_type clientId, string_view_type contactId, string_view_type message) override;
		void AddContact(string_view_type clientId, string_view_type contactId, string_view_type group, string_view_type message) override;
		void RemoveContact(string_view_type clientId, string_view_type contactId, string_view_type group) override;
		void RenameGroup(string_type currentName, string_type newName) override;
	};

}}}
