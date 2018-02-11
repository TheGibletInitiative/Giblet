//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/BlockedContactManager.h>
#include <Protocols/YMsg/ClientConnection.h>
#include <Protocols/YMsg/ContactManager.h>
#include <Protocols/YMsg/PresenceManager.h>
#include <Protocols/YMsg/ProfileManager.h>

#include <Protocols/YMsg/MockPresenceManager.h>

namespace Giblet { namespace Protocols { namespace YMsg
{

	class SessionContext
	{
	public:
		using string_type = std::string;			//	should be a UTF8 string
		using string_view_type = std::string_view;	//	should be a UTF8 string
		using sessionid_type = uint32_t;
		using protocolversion_type = uint16_t;
		using availability_type = detail::Availability;
		using contact_info_type = ContactInfo;		//	FIXME: Shouldn't be necessary here (old stuff still depends on it)


		SessionContext(
			std::shared_ptr<ClientConnection> connection,
			std::shared_ptr<BlockedContactManager> blockedContactManager,
			std::shared_ptr<ProfileManager> profileManager,
			std::shared_ptr<ContactManager> contactManager);

		SessionContext(const SessionContext&) = delete;


		//	FIXME: Limit usage of this.
		ClientConnection& GetConnection();

		ContactManager& GetContactManager();
		BlockedContactManager& GetBlockedContactManager();
		ProfileManager& GetProfileManager();
		PresenceManager& GetPresenceManager();


		void BeginSession(sessionid_type id, string_view_type clientId, availability_type initialAvailability);

		string_view_type GetClientId() const;

		void RequestClientProfile();


	protected:

		const std::shared_ptr<ClientConnection>			connection_;
		const std::shared_ptr<BlockedContactManager>	blockedContactManager_;
		const std::shared_ptr<ProfileManager>			profileManager_;
		const std::shared_ptr<ContactManager>			contactManager_;
		MockPresenceManager		presenceManager_;
		bool					loggedIn_;
		string_type				clientId_;
	};

}}}
