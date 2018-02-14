//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/MockContactManagementLink.h>
#include <ctime>


namespace Giblet { namespace Protocols { namespace YMsg
{

	MockContactManagementLink::MockContactManagementLink(
		std::shared_ptr<BlockedContactManager> blockedContactManager,
		std::shared_ptr<ProfileManager> profileManager,
		std::shared_ptr<ContactManager> contactManager,
		std::shared_ptr<ContactManagementEvents> contactEvents)
		:
		blockedContactManager_(blockedContactManager),
		profileManager_(profileManager),
		contactEvents_(contactEvents),
		contactManager_(contactManager)
	{

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


	void MockContactManagementLink::DenyRequest(
		string_view_type clientId,
		string_view_type contactId,
		string_view_type message)
	{
		if (profileManager_->IsClientOrProfileId(contactId))
		{
			//	We just bounce back a deny contact add request to the client
			//	NOTE: We swap the client and contact id's to send back to client
			contactEvents_->OnAddRequestDenied(contactId, clientId, message);
		}
	}


	void MockContactManagementLink::AddContact(string_view_type clientId, string_view_type contactId, string_view_type group, string_view_type message)
	{
		if (group.empty())
		{
			contactEvents_->OnEmptyGroup(clientId, contactId);
			return;
		}

		if (contactManager_->IsContact(contactId))
		{
			contactEvents_->OnDuplicateContact(clientId, contactId, group);
			return;
		}

		if (blockedContactManager_->IsBlocked(contactId))
		{
			contactEvents_->OnContactIsIneligible(clientId, contactId, group);
			return;
		}

		if (contactId == string_type("grag"))
		{
			contactEvents_->OnContactNoExist(clientId, contactId, group);
			return;
		}

		contactEvents_->OnAddRequestPending(clientId, contactId, group);

		//	If we are adding our own id or profile send it back to ourselves
		if (profileManager_->IsClientOrProfileId(contactId))
		{
			//	FIXME: We SHOULD have to swap the client and contact id's to send back to client but that's not what is required here.
			//	either the parser fields are incorrect or the builder is.
			contactEvents_->OnAddContactRequest(clientId, contactId, message, time(nullptr));
		}
		return;
	}


	void MockContactManagementLink::RemoveContact(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		if (!contactManager_->IsContact(contactId))
		{
			contactEvents_->OnRemoveContactFailed(clientId, contactId, group);
			return;
		}

		contactEvents_->OnContactRemoved(clientId, contactId, group);
	}


	void MockContactManagementLink::RenameGroup(string_type currentName, string_type newName)
	{
		if (currentName == newName)
		{
			//	current name and new name cannot be identical. Case not handled!
			contactEvents_->OnGroupRenameFailed(currentName, newName);
			return;
		}

		//if (groupedContacts_.find(newName) != groupedContacts_.end())
		//{
		//	//	new name already exists. Case not handled!
		//	OnGroupRenameFailed(currentName, newName);
		//	return;
		//}

		//auto currentGroup(groupedContacts_.find(currentName));
		//if (currentGroup == groupedContacts_.end())
		//{
		//	//	Current group does not exist. Case not handled!
		//	OnGroupRenameFailed(currentName, newName);
		//	return;
		//}

		contactEvents_->OnGroupRenamed(currentName, newName);
	}

}}}
