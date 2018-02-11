//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/MockBlockedContactManager.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	void MockBlockedContactManager::Add(string_view_type clientId, string_view_type contactId)
	{
		if (find(blockedUsers_.begin(), blockedUsers_.end(), contactId) != blockedUsers_.end())
		{
			OnAlreadyBlocked(clientId, contactId);
		}

		OnBlocked(clientId, contactId);
	}


	void MockBlockedContactManager::Remove(string_view_type clientId, string_view_type contactId)
	{
		if (find(blockedUsers_.begin(), blockedUsers_.end(), contactId) == blockedUsers_.end())
		{
			OnNotInBlockedList(clientId, contactId);
		}

		OnUnblocked(clientId, contactId);
	}

}}}
