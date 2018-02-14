//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/SessionContext.h>
#include <Protocols/YMsg/Server/Builders/ClientProfile.h>
#include <Protocols/YMsg/Server/Builders/ChallengeResponse.h>
#include <Protocols/YMsg/Server/Builders/PingConfiguration.h>
#include <Protocols/YMsg/Server/Builders/ContactOnline.h>
#include <algorithm>
#include <Windows.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	SessionContext::SessionContext(
		std::shared_ptr<ClientConnection> connection,
		std::shared_ptr<BlockedContactManager> blockedContactManager,
		std::shared_ptr<ProfileManager> profileManager,
		std::shared_ptr<ProfileManagementLink> profileManagementLink,
		std::shared_ptr<ContactManager> contactManager,
		std::shared_ptr<ContactManagementLink> contactManagementLink,
		std::shared_ptr<PresenceManagementLink>	presenceManagementLink)
		:
		connection_(connection),
		profileManager_(profileManager),
		profileManagementLink_(profileManagementLink),
		blockedContactManager_(blockedContactManager),
		contactManagementLink_(contactManagementLink),
		contactManager_(contactManager),
		presenceManagementLink_(presenceManagementLink)
	{
		if (!connection_)
		{
			throw std::invalid_argument("connection cannot be null");
		}

		if (!blockedContactManager_)
		{
			throw std::invalid_argument("blocked contact manager cannot be null");
		}

		if (!profileManager_)
		{
			throw std::invalid_argument("profile manager cannot be null");
		}

		if (!contactManager_)
		{
			throw std::invalid_argument("basicContactManager  cannot be null");
		}
		if (!presenceManagementLink_)
		{
			throw std::invalid_argument("basicContactManager  cannot be null");
		}
	}




	ClientConnection& SessionContext::GetConnection()
	{
		return *connection_;
	}


	ContactManagementLink& SessionContext::ContactManagement()
	{
		return *contactManagementLink_;
	}


	BlockedContactManager& SessionContext::GetBlockedContactManager()
	{
		return *blockedContactManager_;
	}


	ProfileManagementLink& SessionContext::ProfileManagement()
	{
		return *profileManagementLink_;
	}


	PresenceManagementLink& SessionContext::PresenceManagement()
	{
		return *presenceManagementLink_;
	}




	void SessionContext::BeginSession(sessionid_type id, string_view_type clientId, availability_type initialAvailability)
	{
		//	FIXME: Check for already being logged in
		loggedIn_ = false;
		clientId_ = clientId;
		connection_->BeginSession(id);
		presenceManagementLink_->BeginSession(initialAvailability);
	}


	SessionContext::string_view_type SessionContext::GetClientId() const
	{
		return clientId_;
	}




	void SessionContext::RequestAuthChallenge(string_view_type clientId)
	{
		//	We don't use any authentication at the moment so we just use method 0
		//	which causes messenger to omit credentials
		Server::Builders::ChallengeResponse builder;
		builder.Build(*connection_, clientId);
		builder.Send(*connection_);
	}


	void SessionContext::RequestClientProfile()
	{
		Server::Builders::ClientProfile	builder;
		builder.Build(
			*connection_,
			profileManager_->GetClientId(),
			*profileManager_,
			*contactManager_,
			*blockedContactManager_);
		builder.Send(*connection_);
	}




	void SessionContext::OnAuthenticationComplete(string_view_type clientId)
	{
		loggedIn_ = true;


		//	NOTE: We originally passed payload.clientId1_ as the name field but we need to
		//	add more management of what the actual login id is if we want to FULLY support profile
		//	id's since you could log in with them as well as your primary id.
		Server::Builders::ClientProfile	clientProfileBuilder;
		clientProfileBuilder.Build(
			*connection_,
			profileManager_->GetClientId(),
			*profileManager_,
			*contactManager_,
			*blockedContactManager_);
		clientProfileBuilder.Send(*connection_);


		//	Now sent the list of online contacts (contains multiple contacts per packet unlike RequestProfile calls)
		std::vector<ContactInfo> onlineContacts;
		for (const auto& contact : *contactManager_)
		{
			if (contact.second.availability != ContactInfo::availability_type::Offline)
			{
				onlineContacts.push_back(contact.second);
			}
		}

		Server::Builders::SetContactStatus contactOnlineBuilder;
		contactOnlineBuilder.Build(*connection_, clientId, onlineContacts);
		contactOnlineBuilder.Send(*connection_);


		//	Finally the Ping configuration
		Server::Builders::PingConfiguration pingConfigBuilder;
		pingConfigBuilder.Build(*connection_);
		pingConfigBuilder.Send(*connection_);
	}

}}}
