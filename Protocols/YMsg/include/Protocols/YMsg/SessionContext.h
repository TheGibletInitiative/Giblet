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
#include <Protocols/YMsg/PresenceManagementLink.h>
#include <Protocols/YMsg/PresenceEvents.h>
#include <Protocols/YMsg/ProfileManager.h>
#include <Protocols/YMsg/ContactManagementLink.h>


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
			std::shared_ptr<ContactManager> contactManager,
			std::shared_ptr<ContactManagementLink> contactManagementLink,
			std::shared_ptr<PresenceManagementLink>	presenceManagementLink);

		SessionContext(const SessionContext&) = delete;

		virtual ~SessionContext() = default;

		//	FIXME: Limit usage of this.
		virtual ClientConnection& GetConnection();

		virtual ContactManager& GetContacts();
		virtual ContactManagementLink& ContactManagement();
		virtual BlockedContactManager& GetBlockedContactManager();
		virtual ProfileManager& GetProfileManager();
		virtual PresenceManagementLink& PresenceManagement();


		virtual void BeginSession(sessionid_type id, string_view_type clientId, availability_type initialAvailability);

		virtual string_view_type GetClientId() const;

		virtual void RequestAuthChallenge(string_view_type clientId);
		virtual void RequestClientProfile();

		virtual void OnAuthenticationComplete(string_view_type clientId);


	protected:

		const std::shared_ptr<ClientConnection>			connection_;
		const std::shared_ptr<BlockedContactManager>	blockedContactManager_;
		const std::shared_ptr<ProfileManager>			profileManager_;
		const std::shared_ptr<ContactManagementLink>	contactManagementLink_;
		const std::shared_ptr<ContactManager>			contactManager_;
		const std::shared_ptr<PresenceManagementLink>	presenceManagementLink_;
		bool					loggedIn_;
		string_type				clientId_;
	};

}}}
