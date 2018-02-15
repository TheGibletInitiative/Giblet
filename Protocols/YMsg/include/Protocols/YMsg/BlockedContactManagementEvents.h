//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/BlockedContactManager.h>
#include <Protocols/YMsg/ClientConnection.h>
#include <memory>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class BlockedContactManagementEvents
	{
	public:

		using string_type = std::string;
		using string_view_type = std::string_view;


	public:

		BlockedContactManagementEvents(
			std::shared_ptr<ClientConnection> connection,
			std::shared_ptr<BlockedContactManager> blockedContactManager);
		BlockedContactManagementEvents(const BlockedContactManagementEvents&) = delete;
		virtual ~BlockedContactManagementEvents() = default;

		virtual void OnNotInBlockedList(string_view_type clientId, string_view_type contactId);
		virtual void OnAlreadyBlocked(string_view_type clientId, string_view_type contactId);
		virtual void OnBlocked(string_view_type clientId, string_view_type contactId);
		virtual void OnUnblocked(string_view_type clientId, string_view_type contactId);


	protected:

		const std::shared_ptr<ClientConnection>			connection_;
		const std::shared_ptr<BlockedContactManager>	blockedContactManager_;
	};

}}}
