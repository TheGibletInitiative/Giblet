//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/MockContactManager.h>
#include <ctime>


namespace Giblet { namespace Protocols { namespace YMsg
{

	void MockContactManager::DenyRequest(
		string_view_type clientId,
		string_view_type contactId,
		string_view_type message)
	{
		if (profileManager_->IsClientOrProfileId(contactId))
		{
			//	We just bounce back a deny contact add request to the client
			//	NOTE: We swap the client and contact id's to send back to client
			OnAddRequestDenied(contactId, clientId, message);
		}
	}


	void MockContactManager::AddContact(string_view_type clientId, string_view_type contactId, string_view_type group, string_view_type message)
	{
		if (group.empty())
		{
			OnEmptyGroup(clientId, contactId);
			return;
		}

		if (IsContact(contactId))
		{
			OnDuplicateContact(clientId, contactId, group);
			return;
		}

		if (blockedContactManager_->IsBlocked(contactId))
		{
			OnContactIsIneligible(clientId, contactId, group);
			return;
		}

		if (contactId == string_type("grag"))
		{
			OnContactNoExist(clientId, contactId, group);
			return;
		}

		OnAddRequestPending(clientId, contactId, group);

		//	If we are adding our own id or profile send it back to ourselves
		if (profileManager_->IsClientOrProfileId(contactId))
		{
			//	FIXME: We SHOULD have to swap the client and contact id's to send back to client but that's not what is required here.
			//	either the parser fields are incorrect or the builder is.
			OnAddContactRequest(clientId, contactId, message, time(nullptr));
		}
		return;
	}


	void MockContactManager::RemoveContact(string_view_type clientId, string_view_type contactId, string_view_type group)
	{
		if (!IsContact(contactId))
		{
			OnRemoveContactFailed(clientId, contactId, group);
			return;
		}

		OnContactRemoved(clientId, contactId, group);
	}


	void MockContactManager::RenameGroup(string_type currentName, string_type newName)
	{
		if (currentName == newName)
		{
			//	current name and new name cannot be identical. Case not handled!
			OnGroupRenameFailed(currentName, newName);
			return;
		}

		if (groupedContacts_.find(newName) != groupedContacts_.end())
		{
			//	new name already exists. Case not handled!
			OnGroupRenameFailed(currentName, newName);
			return;
		}

		auto currentGroup(groupedContacts_.find(currentName));
		if (currentGroup == groupedContacts_.end())
		{
			//	Current group does not exist. Case not handled!
			OnGroupRenameFailed(currentName, newName);
			return;
		}

		OnGroupRenamed(currentName, newName);
	}

}}}
