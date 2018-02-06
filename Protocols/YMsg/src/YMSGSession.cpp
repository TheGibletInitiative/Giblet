//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/YMSGSession.h>
#include <algorithm>
#include <Windows.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	YMSGSession::YMSGSession(SOCKET sessionSocket)
		:
		sessionSocket_(sessionSocket),
		loggedIn_(false),
		sessionId_(0),
		protocolVersion_(0),
		availability_(availability_type::Offline)
	{}




	SOCKET YMSGSession::GetSocket() const
	{
		return sessionSocket_;
	}


	void YMSGSession::SendToClient(const char *data, const size_t length)
	{
		send(sessionSocket_, data, length, 0);
	}




	void YMSGSession::AddProfile(string_view_type profileId)
	{
		if (find(profiles_.begin(), profiles_.end(), profileId) != profiles_.end())
		{
			throw std::invalid_argument("Duplicate profile");
		}

		profiles_.push_back(std::string(profileId));
	}


	void YMSGSession::AddContact(string_view_type group, const contact_info_type& contact)
	{
		if (group.empty())
		{
			throw std::invalid_argument("Group cannot be empty");
		}

		if (FindContact(contact.id) != contacts_.end())
		{
			throw std::invalid_argument("Duplicate cotnact");
		}

		contacts_.push_back(contact);
		groupedContacts_[string_type(group)].push_back(string_type(contact.id));
	}

	void YMSGSession::RemoveContact(string_view_type contactId)
	{
		contacts_.erase(FindContact(contactId));
		for (auto& group : groupedContacts_)
		{
			auto contactIterator(find(group.second.begin(), group.second.end(), contactId));
			if (contactIterator != group.second.end())
			{
				group.second.erase(contactIterator);
			}
		}
	}


	bool YMSGSession::RenameGroup(string_type currentName, string_type newName)
	{
		if (currentName != newName)
		{
			if(groupedContacts_.find(newName) != groupedContacts_.end())
			{
				return false;
			}

			auto currentGroup = groupedContacts_.find(currentName);
			if (currentGroup == groupedContacts_.end())
			{
				//	FIXME: More descriptor error type?
				return false;
			}

			groupedContacts_[newName] = move(currentGroup->second);
			groupedContacts_.erase(currentGroup);
		}

		return true;
	}


	void YMSGSession::AddBlockedId(string_view_type id)
	{
		if (find(blockedUsers_.begin(), blockedUsers_.end(), id) != blockedUsers_.end())
		{
			throw std::invalid_argument("Duplicate blocked user id");
		}

		blockedUsers_.push_back(std::string(id));
	}




	YMSGSession::const_contact_iterator YMSGSession::FindContact(string_view_type contactId) const
	{
		auto predicate = [contactId](const contact_info_type& existingContact) -> bool
		{
			return existingContact.id == contactId;
		};

		return find_if(contacts_.begin(), contacts_.end(), predicate);
	}

}}}
