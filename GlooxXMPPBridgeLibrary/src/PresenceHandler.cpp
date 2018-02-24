//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <GlooxXMPPBridgeLibrary/PresenceHandler.h>
#include <iostream>


namespace Giblet { namespace Bridges { namespace Gloox
{
	namespace
	{
		std::chrono::milliseconds GetTimestamp()
		{
			using namespace std::chrono;
			return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		}
	}

	PresenceHandler::PresenceHandler(std::shared_ptr<Protocols::YMsg::PresenceEvents> presenceEvents)
		:
		presenceEvents_(presenceEvents),
		sessionBeginTimestamp_(GetTimestamp())
	{}


	void PresenceHandler::handlePresence(const gloox::Presence& presence)
	{
		std::cerr
			<< "Presence Update: " << presence.from().full()
			<< " state = " << presence.presence()
			<< '\n';

		const bool showNotification(GetTimestamp() - sessionBeginTimestamp_ >= std::chrono::milliseconds(2000));
		if (!presence.status().empty())
		{
			bool isAwayOrBusy(
				presence.presence() == gloox::Presence::PresenceType::Away
				|| presence.presence() == gloox::Presence::PresenceType::DND
				|| presence.presence() == gloox::Presence::PresenceType::XA);

			presenceEvents_->OnContactStatusMessage(
				presence.from().bare(),
				presence.status(),
				isAwayOrBusy,
				showNotification);
		}
		else
		{
			switch (presence.presence())
			{
			case gloox::Presence::PresenceType::Available:
			case gloox::Presence::PresenceType::Chat:
				presenceEvents_->OnContactAvailable(presence.from().bare(), showNotification);
				break;

				break;

			case gloox::Presence::PresenceType::Away:
				presenceEvents_->OnContactAway(presence.from().bare(), Protocols::YMsg::PresenceEvents::availability_type::Brb, false);
				break;

			case gloox::Presence::PresenceType::DND:
				presenceEvents_->OnContactAway(presence.from().bare(), Protocols::YMsg::PresenceEvents::availability_type::Busy, true);
				break;

			case gloox::Presence::PresenceType::XA:
				presenceEvents_->OnContactAway(presence.from().bare(), Protocols::YMsg::PresenceEvents::availability_type::Busy, true);
				break;

			case gloox::Presence::PresenceType::Unavailable:
				presenceEvents_->OnContactOffline(presence.from().bare());
				break;

			case gloox::Presence::PresenceType::Probe:
			case gloox::Presence::PresenceType::Error:
			case gloox::Presence::PresenceType::Invalid:
				break;
			}
		}
	}

}}}
