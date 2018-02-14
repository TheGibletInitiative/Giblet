//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/ContactManagementLink.h>
#include <Protocols/YMsg/ContactManagementEvents.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class MockContactManagementLink : public ContactManagementLink
	{
	public:

		MockContactManagementLink(
			std::shared_ptr<BlockedContactManager> blockedContactManager,
			std::shared_ptr<ProfileManager> profileManager,
			std::shared_ptr<ContactManager> contactManager,
			std::shared_ptr<ContactManagementEvents> contactEvents);


		void DenyRequest(string_view_type clientId, string_view_type contactId, string_view_type message) override;
		void AddContact(string_view_type clientId, string_view_type contactId, string_view_type group, string_view_type message) override;
		void RemoveContact(string_view_type clientId, string_view_type contactId, string_view_type group) override;
		void RenameGroup(string_type currentName, string_type newName) override;


	protected:

		const std::shared_ptr<BlockedContactManager>	blockedContactManager_;
		const std::shared_ptr<ProfileManager>			profileManager_;
		const std::shared_ptr<ContactManagementEvents>	contactEvents_;
		const std::shared_ptr<ContactManager>			contactManager_;
	};

}}}
