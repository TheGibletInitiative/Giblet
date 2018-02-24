//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <presencehandler.h>
#include <Protocols/YMsg/PresenceEvents.h>
#include <chrono>


namespace Giblet { namespace Bridges { namespace Gloox
{

	class PresenceHandler : public gloox::PresenceHandler
	{
	public:

		PresenceHandler(std::shared_ptr<Protocols::YMsg::PresenceEvents> presenceEvents);

		void handlePresence(const gloox::Presence& presence);


	protected:

		const std::shared_ptr<Protocols::YMsg::PresenceEvents>	presenceEvents_;
		std::chrono::milliseconds								sessionBeginTimestamp_;
	};

}}}
