//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/YMSGSession.h>
#include <Protocols/YMsg/Server/Builders/ClientProfile.h>
#include <algorithm>
#include <Windows.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	SessionContext::SessionContext(
		std::shared_ptr<ClientConnection> connection,
		std::shared_ptr<BlockedContactManager> blockedContactManager,
		std::shared_ptr<ProfileManager> profileManager,
		std::shared_ptr<ContactManager> contactManager)
		:
		connection_(connection),
		profileManager_(profileManager),
		blockedContactManager_(blockedContactManager),
		contactManager_(contactManager),
		presenceManager_(connection, profileManager, contactManager_)
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
	}




	ClientConnection& SessionContext::GetConnection()
	{
		return *connection_;
	}


	ContactManager& SessionContext::GetContactManager()
	{
		return *contactManager_;
	}


	BlockedContactManager& SessionContext::GetBlockedContactManager()
	{
		return *blockedContactManager_;
	}


	ProfileManager& SessionContext::GetProfileManager()
	{
		return *profileManager_;
	}


	PresenceManager& SessionContext::GetPresenceManager()
	{
		return presenceManager_;
	}




	void SessionContext::BeginSession(sessionid_type id, string_view_type clientId, availability_type initialAvailability)
	{
		loggedIn_ = true;
		clientId_ = clientId;
		connection_->BeginSession(id);
		presenceManager_.BeginSession(initialAvailability);
	}


	SessionContext::string_view_type SessionContext::GetClientId() const
	{
		return clientId_;
	}


	void SessionContext::RequestClientProfile()
	{
		Server::Builders::ClientProfile	builder;
		builder.Build(*connection_, *this);
		builder.Send(*connection_);
	}


}}}
