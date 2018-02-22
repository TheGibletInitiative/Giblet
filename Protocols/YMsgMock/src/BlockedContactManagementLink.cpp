//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsgMock/BlockedContactManagementLink.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	MockBlockedContactManagementLink::MockBlockedContactManagementLink(
		std::shared_ptr<BlockedContactManager> blockedContactManager, 
		std::shared_ptr<BlockedContactManagementEvents> blockedContactManagementEvents)
		:
		blockedContactManager_(blockedContactManager),
		blockedContactManagementEvents_(blockedContactManagementEvents)
	{
		if (!blockedContactManager_)
		{
			throw std::invalid_argument("blockedContactManager_ cannot be null");
		}

		if (!blockedContactManagementEvents_)
		{
			throw std::invalid_argument("blockedContactManagementEvents_ cannot be null");
		}
	}


	void MockBlockedContactManagementLink::Add(string_view_type clientId, string_view_type contactId)
	{
		if (blockedContactManager_->IsBlocked(contactId))
		{
			blockedContactManagementEvents_->OnAlreadyBlocked(clientId, contactId);
			return;
		}

		blockedContactManagementEvents_->OnBlocked(clientId, contactId);
	}


	void MockBlockedContactManagementLink::Remove(string_view_type clientId, string_view_type contactId)
	{
		if (!blockedContactManager_->IsBlocked(contactId))
		{
			blockedContactManagementEvents_->OnNotInBlockedList(clientId, contactId);
		}

		blockedContactManagementEvents_->OnUnblocked(clientId, contactId);
	}

}}}
