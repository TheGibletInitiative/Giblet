//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/BlockedContactManager.h>
#include <Protocols/YMsg/Server/Builders/ContactBlockedOrUnblocked.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

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


	void BlockedContactManager::Add(string_view_type contactId)
	{
		//	FIXME: Report error if found but keep on going just in case
		if (find(blockedUsers_.begin(), blockedUsers_.end(), contactId) == blockedUsers_.end())
		{
			blockedUsers_.push_back(std::string(contactId));
		}
	}


	void BlockedContactManager::Remove(string_view_type contactId)
	{
		auto blockedContact(find(blockedUsers_.begin(), blockedUsers_.end(), contactId));
		//	FIXME: Report error if not found and keep going just in case.
		if (blockedContact != blockedUsers_.end())
		{
			blockedUsers_.erase(blockedContact);
		}
	}

}}}
