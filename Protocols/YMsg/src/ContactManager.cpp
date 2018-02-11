//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/ContactManager.h>
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

	ContactManager::ContactManager(
		std::shared_ptr<ClientConnection> connection,
		std::shared_ptr<BlockedContactManager> blockedContactManager,
		std::shared_ptr<ProfileManager> profileManager)
		:
		connection_(connection),
		blockedContactManager_(blockedContactManager),
		profileManager_(profileManager)
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

	//	FIXME: Need a proper load for loading a batch
	void ContactManager::LoadContact(string_view_type group, const contact_info_type& contact)
	{
		contacts_.push_back(contact);
		groupedContacts_[string_type(group)].push_back(contact.id);
	}



	ContactManager::const_iterator ContactManager::begin() const
	{
		return contacts_.begin();
	}

	ContactManager::const_iterator ContactManager::end() const
	{
		return contacts_.end();
	}


	ContactManager::grouped_const_iterator ContactManager::grouped_contacts_begin() const
	{
		return groupedContacts_.begin();
	}

	ContactManager::grouped_const_iterator ContactManager::grouped_contacts_end() const
	{
		return groupedContacts_.end();
	}


	ContactManager::const_iterator ContactManager::FindContact(string_view_type contactId) const
	{
		auto predicate = [contactId](const contact_info_type& existingContact) -> bool
		{
			return existingContact.id == contactId;
		};

		return find_if(contacts_.begin(), contacts_.end(), predicate);
	}


	ContactManager::iterator ContactManager::FindContact(string_view_type contactId)
	{
		auto predicate = [contactId](const contact_info_type& existingContact) -> bool
		{
			return existingContact.id == contactId;
		};

		return find_if(contacts_.begin(), contacts_.end(), predicate);
	}


	bool ContactManager::IsContact(string_view_type contactId) const
	{
		return FindContact(contactId) != contacts_.end();
	}


	void ContactManager::RefreshProfile() const
	{
		Server::Builders::ClientProfile	builder;
		builder.Build(
			*connection_,
			profileManager_->GetClientId(),
			*profileManager_,
			*this,
			*blockedContactManager_);
		builder.Send(*connection_);
	}


	void ContactManager::OnAddRequestDenied(string_view_type clientId, string_view_type contactId, string_view_type message)
	{
		//	FIXME: Re need to remove them from our internal list and refresh the profile.

		Server::Builders::DenyContactAddRequest builder;
		builder.Build(*connection_, clientId, contactId, message);	
		builder.Send(*connection_);

		//	FIXME: refresh the profile
	}


	void ContactManager::OnAddContactRequest(string_view_type clientId, string_view_type contactId, string_view_type message, time_t timestamp)
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


	void ContactManager::OnRemoveContactFailed(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		Server::Builders::ContactRemoved builder;
		builder.Build(*connection_, clientId, contactId, group, Server::Builders::ContactRemoved::status_type::GeneralFailure);
		builder.Send(*connection_);
	}


	void ContactManager::OnAddRequestPending(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		//	FIXME: Check for pre-existing contact!
		contacts_.push_back(contact_info_type(contact_info_type::State::AddPending, contactId));
		groupedContacts_[string_type(group)].push_back(string_type(contactId));


		//	Tell messenger the add request was sent
		Server::Builders::AddContactRequestResult builder;
		builder.Build(*connection_, contactId, clientId, group, addrequeststatus_type::AddPending);
		builder.Send(*connection_);

		RefreshProfile();
	}


	void ContactManager::OnEmptyGroup(string_view_type clientId, string_view_type contactId)
	{
		//	FIXME: There is either a status code for this that we're missing or it's handled by Messenger
		Server::Builders::AddContactRequestResult builder;
		builder.Build(*connection_, contactId, clientId, string_view_type(), addrequeststatus_type::GroupAlreadyExists);
		builder.Send(*connection_);
	}


	void ContactManager::OnContactIsIneligible(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		//	FIXME: There is either a status code for this that we're missing or it's handled by Messenger.
		//	NOTE: It does seem to be handled by messenger but that doesn't mean third party clients worked that way
		Server::Builders::AddContactRequestResult builder;
		builder.Build(*connection_, contactId, clientId, group, addrequeststatus_type::GeneralFailure);
		builder.Send(*connection_);
	}


	void ContactManager::OnContactNoExist(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		Server::Builders::AddContactRequestResult builder;
		builder.Build(*connection_, contactId, clientId, group, addrequeststatus_type::ContactDoesNotExist);
		builder.Send(*connection_);
	}


	void ContactManager::OnDuplicateContact(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		Server::Builders::AddContactRequestResult builder;
		builder.Build(*connection_, contactId, clientId, group, addrequeststatus_type::DuplicateContact);
		builder.Send(*connection_);
	}


	void ContactManager::OnContactAdded(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		((void)clientId);
		((void)contactId);
		((void)group);

		throw std::runtime_error("Not implemented");
	}


	void ContactManager::OnContactRemoved(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		contacts_.erase(FindContact(contactId));
		for (auto& contactGroup : groupedContacts_)
		{
			auto contactIterator(find(contactGroup.second.begin(), contactGroup.second.end(), contactId));
			if (contactIterator != contactGroup.second.end())
			{
				contactGroup.second.erase(contactIterator);
			}
		}

		Server::Builders::ContactRemoved builder;
		builder.Build(*connection_, clientId, contactId, group, Server::Builders::ContactRemoved::status_type::Success);
		builder.Send(*connection_);

		RefreshProfile();
	}


	void ContactManager::OnGroupRenameFailed(string_view_type currentName, string_view_type newName)
	{
		((void)currentName);
		((void)newName);

		std::invalid_argument("Group rename failed. Not implemented!");
	}


	void ContactManager::OnGroupRenamed(string_type currentName, string_type newName)
	{
		if (currentName == newName)
		{
			std::invalid_argument("current name and new name cannot be identical. Case not handled!");
		}

		if (groupedContacts_.find(newName) != groupedContacts_.end())
		{
			std::invalid_argument("new name already exists. Case not handled!");
		}

		auto currentGroup(groupedContacts_.find(currentName));
		if (currentGroup == groupedContacts_.end())
		{
			std::invalid_argument("Current group does not exist. Case not handled!");
		}

		groupedContacts_[newName] = move(currentGroup->second);
		groupedContacts_.erase(currentGroup);

		Server::Builders::ContactGroupRenamed builder;
		builder.Build(*connection_, profileManager_->GetClientId(), newName, currentName);
		builder.Send(*connection_);

		RefreshProfile();
	}



	void ContactManager::OnContactAvailable(string_view_type contactId)
	{
		auto contactInfo(FindContact(contactId));
		if (contactInfo == contacts_.end())
		{
			throw std::runtime_error("Don't know how to handle idle contact not in list");
		}

		contactInfo->availability = availability_type::Available;

		Server::Builders::UpdateContactStatus builder;
		builder.Build(*connection_, *contactInfo);
		builder.Send(*connection_);
	}


	void ContactManager::OnContactIdle(string_view_type contactId, string_view_type idleUnknown, string_view_type message, bool isBusy)
	{
		auto contactInfo(FindContact(contactId));
		if (contactInfo == contacts_.end())
		{
			throw std::runtime_error("Don't know how to handle idle contact not in list");
		}

		contactInfo->idleUnknown = idleUnknown;
		contactInfo->availability = availability_type::Idle;
		contactInfo->isBusy = isBusy;
		contactInfo->customMessage = message;

		Server::Builders::UpdateContactStatus builder;
		builder.Build(*connection_, *contactInfo);
		builder.Send(*connection_);
	}


	void ContactManager::OnContactAway(string_view_type contactId, availability_type availability, bool isBusy)
	{
		auto contactInfo(FindContact(contactId));
		if (contactInfo == contacts_.end())
		{
			throw std::runtime_error("Don't know how to handle idle contact not in list");
		}

		contactInfo->availability = availability;
		contactInfo->isBusy = isBusy;

		Server::Builders::UpdateContactStatus builder;
		builder.Build(*connection_, *contactInfo);
		builder.Send(*connection_);
	}


	void ContactManager::OnContactStatusMessage(string_view_type contactId, string_view_type message, bool isBusy)
	{
		auto contactInfo(FindContact(contactId));
		if (contactInfo == contacts_.end())
		{
			throw std::runtime_error("Don't know how to handle idle contact not in list");
		}

		contactInfo->availability = availability_type::Custom;
		contactInfo->isBusy = isBusy;
		contactInfo->customMessage = message;


		Server::Builders::UpdateContactStatus builder;
		builder.Build(*connection_, *contactInfo);
		builder.Send(*connection_);

	}


	void ContactManager::OnContactOffline(string_view_type contactId)
	{
		auto contactInfo(FindContact(contactId));
		if (contactInfo == contacts_.end())
		{
			throw std::runtime_error("Don't know how to handle idle contact not in list");
		}

		contactInfo->availability = availability_type::Offline;

		Server::Builders::ContactOffline builder;
		builder.Build(*connection_, contactId);
		builder.Send(*connection_);
	}


}}}
