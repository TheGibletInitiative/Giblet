//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/BlockedContactManager.h>
#include <Protocols/YMsg/Server/Builders/ContactBlockedOrUnblocked.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	BlockedContactManagementEvents::BlockedContactManagementEvents(
		std::shared_ptr<ClientConnection> connection,
		std::shared_ptr<BlockedContactManager> blockedContactManager)
		:
		connection_(connection),
		blockedContactManager_(blockedContactManager)
	{
		if (!connection_)
		{
			throw std::invalid_argument("connection cannot be null");
		}

		if (!blockedContactManager_)
		{
			throw std::invalid_argument("blockedContactManager cannot be null");
		}
	}



	void BlockedContactManagementEvents::OnAlreadyBlocked(string_view_type clientId, string_view_type contactId)
	{
		((void)clientId);
		((void)contactId);

		throw std::invalid_argument("[WIP] Adding duplicate contacts to blocked list not supported yet");
	}


	void BlockedContactManagementEvents::OnNotInBlockedList(string_view_type clientId, string_view_type contactId)
	{
		((void)clientId);
		((void)contactId);

		throw std::invalid_argument("[WIP] Removing contacts that do not exist in blocked list not supported yet");
	}
	

	void BlockedContactManagementEvents::OnBlocked(string_view_type clientId, string_view_type contactId)
	{
		//	FIXME: Report error if found but keep on going just in case
		blockedContactManager_->Add(contactId);

		Server::Builders::ContactBlocked builder;
		builder.Build(*connection_, clientId, contactId);
		builder.Send(*connection_);
	}


	void BlockedContactManagementEvents::OnUnblocked(string_view_type clientId, string_view_type contactId)
	{
		blockedContactManager_->Remove(contactId);

		Server::Builders::ContactUnblocked builder;
		builder.Build(*connection_, clientId, contactId);
		builder.Send(*connection_);
	}

}}}
