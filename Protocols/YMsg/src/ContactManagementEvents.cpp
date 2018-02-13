//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/ContactManagementEvents.h>
#include <Protocols/YMsg/Server/Builders/DenyContactAddRequest.h>
#include <Protocols/YMsg/Server/Builders/AddContactRequestResult.h>
#include <Protocols/YMsg/Server/Builders/ClientProfile.h>
#include <Protocols/YMsg/Server/Builders/AddContactRequest.h>
#include <Protocols/YMsg/Server/Builders/ContactRemoved.h>
#include <Protocols/YMsg/Server/Builders/ContactGroupRenamed.h>
#include <Protocols/YMsg/Server/Builders/DenyContactAddRequest.h>
#include <Protocols/YMsg/Server/Builders/ContactOnline.h>
#include <Protocols/YMsg/Server/Builders/ContactOffline.h>
#include <algorithm>


namespace Giblet { namespace Protocols { namespace YMsg
{

	ContactManagementEvents::ContactManagementEvents(
		std::shared_ptr<ClientConnection> connection,
		std::shared_ptr<BlockedContactManager> blockedContactManager,
		std::shared_ptr<ProfileManager> profileManager,
		std::shared_ptr<ContactManager> contactManager)
		:
		connection_(connection),
		blockedContactManager_(blockedContactManager),
		profileManager_(profileManager),
		contactManager_(contactManager)
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
			throw std::invalid_argument("contacts cannot be null");
		}
	}


	void ContactManagementEvents::RefreshProfile() const
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


	void ContactManagementEvents::OnAddRequestDenied(string_view_type clientId, string_view_type contactId, string_view_type message)
	{
		//	FIXME: Re need to remove them from our internal list and refresh the profile.

		Server::Builders::DenyContactAddRequest builder;
		builder.Build(*connection_, clientId, contactId, message);	
		builder.Send(*connection_);

		//	FIXME: refresh the profile
	}


	void ContactManagementEvents::OnAddContactRequest(string_view_type clientId, string_view_type contactId, string_view_type message, time_t timestamp)
	{
		Server::Builders::AddContactRequest builder;
		builder.Build(
			*connection_,
			clientId,
			contactId,
			message,
			timestamp);
		builder.Send(*connection_);
	}


	void ContactManagementEvents::OnRemoveContactFailed(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		Server::Builders::ContactRemoved builder;
		builder.Build(*connection_, clientId, contactId, group, Server::Builders::ContactRemoved::status_type::GeneralFailure);
		builder.Send(*connection_);
	}


	void ContactManagementEvents::OnAddRequestPending(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		contactManager_->AddPendingContact(contactId, group);

		//	Tell messenger the add request was sent
		Server::Builders::AddContactRequestResult builder;
		builder.Build(*connection_, contactId, clientId, group, addrequeststatus_type::AddPending);
		builder.Send(*connection_);

		RefreshProfile();
	}


	void ContactManagementEvents::OnEmptyGroup(string_view_type clientId, string_view_type contactId)
	{
		//	FIXME: There is either a status code for this that we're missing or it's handled by Messenger
		Server::Builders::AddContactRequestResult builder;
		builder.Build(*connection_, contactId, clientId, string_view_type(), addrequeststatus_type::GroupAlreadyExists);
		builder.Send(*connection_);
	}


	void ContactManagementEvents::OnContactIsIneligible(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		//	FIXME: There is either a status code for this that we're missing or it's handled by Messenger.
		//	NOTE: It does seem to be handled by messenger but that doesn't mean third party clients worked that way
		Server::Builders::AddContactRequestResult builder;
		builder.Build(*connection_, contactId, clientId, group, addrequeststatus_type::GeneralFailure);
		builder.Send(*connection_);
	}


	void ContactManagementEvents::OnContactNoExist(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		Server::Builders::AddContactRequestResult builder;
		builder.Build(*connection_, contactId, clientId, group, addrequeststatus_type::ContactDoesNotExist);
		builder.Send(*connection_);
	}


	void ContactManagementEvents::OnDuplicateContact(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		Server::Builders::AddContactRequestResult builder;
		builder.Build(*connection_, contactId, clientId, group, addrequeststatus_type::DuplicateContact);
		builder.Send(*connection_);
	}


	void ContactManagementEvents::OnContactAdded(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		((void)clientId);
		((void)contactId);
		((void)group);

		throw std::runtime_error("Not implemented");
	}


	void ContactManagementEvents::OnContactRemoved(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		contactManager_->RemoveContact(contactId, group);

		Server::Builders::ContactRemoved builder;
		builder.Build(*connection_, clientId, contactId, group, Server::Builders::ContactRemoved::status_type::Success);
		builder.Send(*connection_);

		RefreshProfile();
	}


	void ContactManagementEvents::OnGroupRenameFailed(string_view_type currentName, string_view_type newName)
	{
		((void)currentName);
		((void)newName);

		std::invalid_argument("Group rename failed. Not implemented!");
	}


	void ContactManagementEvents::OnGroupRenamed(string_type currentName, string_type newName)
	{
		contactManager_->RenameGroup(currentName, newName);

		Server::Builders::ContactGroupRenamed builder;
		builder.Build(*connection_, profileManager_->GetClientId(), newName, currentName);
		builder.Send(*connection_);

		RefreshProfile();
	}

}}}
