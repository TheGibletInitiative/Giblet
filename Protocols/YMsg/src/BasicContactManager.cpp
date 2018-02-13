//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/ContactManager.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	//	FIXME: Need a proper load for loading a batch
	void ContactManager::LoadContact(const contact_info_type& contact)
	{
		if (!contacts_.emplace(make_pair(contact.id, contact)).second)
		{
			throw std::runtime_error("Unable to insert contact");
		}
	}



	ContactManager::const_iterator ContactManager::begin() const
	{
		return contacts_.begin();
	}

	ContactManager::const_iterator ContactManager::end() const
	{
		return contacts_.end();
	}


	ContactManager::const_iterator ContactManager::FindContact(string_view_type contactId) const
	{
		return contacts_.find(string_type(contactId));
	}


	ContactManager::iterator ContactManager::FindContact(string_view_type contactId)
	{
		return contacts_.find(string_type(contactId));
	}


	bool ContactManager::IsContact(string_view_type contactId) const
	{
		return FindContact(contactId) != contacts_.end();
	}


	void ContactManager::AddPendingContact(string_view_type contactId, string_view_type group)
	{
		//	FIXME: Check for pre-existing contact!
		const contact_info_type contactInfo(contactId, group, contact_info_type::State::AddPending);
		if (!contacts_.emplace(make_pair(contactId, contactInfo)).second)
		{
			throw std::runtime_error("Unable to insert contact information");
		}
	}


	void ContactManager::SetContactAdded(string_view_type contactId, string_view_type group)
	{
		((void)contactId);
		((void)group);

		throw std::runtime_error("Not implemented");
	}


	void ContactManager::RemoveContact(string_view_type contactId, string_view_type group)
	{
		((void)group);
		contacts_.erase(FindContact(contactId));
	}


	void ContactManager::RenameGroup(string_type currentName, string_type newName)
	{
		if (currentName == newName)
		{
			std::invalid_argument("current name and new name cannot be identical. Case not handled!");
		}

		for (auto& contact : contacts_)
		{
			if (contact.second.group == currentName)
			{
				contact.second.group = newName;
			}
		}
	}



	const ContactManager::contact_info_type* ContactManager::SetContactAvailable(string_view_type contactId)
	{
		auto contactInfo(FindContact(contactId));
		if (contactInfo == contacts_.end())
		{
			//throw std::runtime_error("Don't know how to handle idle contact not in list");
			return nullptr;
		}

		contactInfo->second.availability = availability_type::Available;

		return &contactInfo->second;
	}


	const ContactManager::contact_info_type* ContactManager::SetContactIdle(string_view_type contactId, string_view_type idleUnknown, string_view_type message, bool isBusy)
	{
		auto contactInfo(FindContact(contactId));
		if (contactInfo == contacts_.end())
		{
			//throw std::runtime_error("Don't know how to handle idle contact not in list");
			return nullptr;
		}

		contactInfo->second.idleUnknown = idleUnknown;
		contactInfo->second.availability = availability_type::Idle;
		contactInfo->second.isBusy = isBusy;
		contactInfo->second.customMessage = message;

		return &contactInfo->second;
	}


	const ContactManager::contact_info_type* ContactManager::SetContactAway(string_view_type contactId, availability_type availability, bool isBusy)
	{
		auto contactInfo(FindContact(contactId));
		if (contactInfo == contacts_.end())
		{
			//throw std::runtime_error("Don't know how to handle idle contact not in list");
			return nullptr;
		}

		contactInfo->second.availability = availability;
		contactInfo->second.isBusy = isBusy;

		return &contactInfo->second;
	}


	const ContactManager::contact_info_type* ContactManager::SetContactStatusMessage(string_view_type contactId, string_view_type message, bool isBusy)
	{
		auto contactInfo(FindContact(contactId));
		if (contactInfo == contacts_.end())
		{
			//throw std::runtime_error("Don't know how to handle idle contact not in list");
			return nullptr;
		}

		contactInfo->second.availability = availability_type::Custom;
		contactInfo->second.isBusy = isBusy;
		contactInfo->second.customMessage = message;

		return &contactInfo->second;
	}


	const ContactManager::contact_info_type* ContactManager::SetContactOffline(string_view_type contactId)
	{
		auto contactInfo(FindContact(contactId));
		if (contactInfo == contacts_.end())
		{
			//throw std::runtime_error("Don't know how to handle idle contact not in list");
			return nullptr;
		}

		contactInfo->second.availability = availability_type::Offline;

		return &contactInfo->second;
	}

}}}
