//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/BlockedContactManagementLink.h>
#include <Protocols/YMsg/BlockedContactManagementEvents.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class MockBlockedContactManagementLink : public BlockedContactManagementLink
	{
	public:

		explicit MockBlockedContactManagementLink(
			std::shared_ptr<BlockedContactManager> blockedContactManager,
			std::shared_ptr<BlockedContactManagementEvents> blockedContactManagementEvents);
		
		void Add(string_view_type clientId, string_view_type contactId) override;
		void Remove(string_view_type clientId, string_view_type contactId) override;

	protected:

		const std::shared_ptr<BlockedContactManager>			blockedContactManager_;
		const std::shared_ptr<BlockedContactManagementEvents>	blockedContactManagementEvents_;
	};

}}}
