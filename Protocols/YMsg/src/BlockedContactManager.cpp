//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/BlockedContactManager.h>
#include <Protocols/YMsg/Server/Builders/ContactBlockedOrUnblocked.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	BlockedContactManager::BlockedContactManager(std::shared_ptr<ClientConnection> connection)
		: connection_(connection)
	{
		if (!connection_)
		{
			throw std::invalid_argument("connection cannot be null");
		}
	}


	void BlockedContactManager::Load(container_type&& blockedContacts)
	{
		blockedUsers_ = move(blockedContacts);
	}




	BlockedContactManager::const_iterator BlockedContactManager::begin() const
	{
		return blockedUsers_.begin();
	}

	BlockedContactManager::const_iterator BlockedContactManager::end() const
	{
		return blockedUsers_.end();
	}


	bool BlockedContactManager::IsBlocked(string_view_type contactId) const
	{
		return find(blockedUsers_.begin(), blockedUsers_.end(), contactId) != blockedUsers_.end();
	}


	void BlockedContactManager::OnAlreadyBlocked(string_view_type clientId, string_view_type contactId)
	{
		((void)clientId);
		((void)contactId);

		throw std::invalid_argument("[WIP] Adding duplicate contacts to blocked list not supported yet");
	}


	void BlockedContactManager::OnNotInBlockedList(string_view_type clientId, string_view_type contactId)
	{
		((void)clientId);
		((void)contactId);

		throw std::invalid_argument("[WIP] Removing contacts that do not exist in blocked list not supported yet");
	}
	

	void BlockedContactManager::OnBlocked(string_view_type clientId, string_view_type contactId)
	{
		//	FIXME: Report error if found but keep on going just in case
		if (find(blockedUsers_.begin(), blockedUsers_.end(), contactId) == blockedUsers_.end())
		{
			blockedUsers_.push_back(std::string(contactId));
		}

		Server::Builders::ContactBlocked builder;
		builder.Build(*connection_, clientId, contactId);
		builder.Send(*connection_);
	}


	void BlockedContactManager::OnUnblocked(string_view_type clientId, string_view_type contactId)
	{
		auto blockedContact(find(blockedUsers_.begin(), blockedUsers_.end(), contactId));
		//	FIXME: Report error if not found and keep going just in case.
		if (blockedContact != blockedUsers_.end())
		{
			blockedUsers_.erase(blockedContact);
		}

		Server::Builders::ContactUnblocked builder;
		builder.Build(*connection_, clientId, contactId);
		builder.Send(*connection_);
	}

}}}
