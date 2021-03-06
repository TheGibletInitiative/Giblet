//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/SetAway.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors
{

	void SetAway::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);

		const auto onlineStatus(to_enum<availability_type>(payload.statusId));
		auto& presenceManagement(session.PresenceManagement());

		if (onlineStatus == availability_type::Invisible)
		{
			presenceManagement.SetInvisible();
		}
		else
		{
			const bool isBusy(payload.busyState.empty() ? false : std::stol(payload.busyState.data()) != 0);

			//	FIXME: Add special handler for idle state to include time idle
			if (onlineStatus == availability_type::Idle)
			{
				presenceManagement.SetIdle(payload.customMessage, isBusy, payload.idleUnknown);
			}
			else if (onlineStatus == availability_type::Custom)
			{
				presenceManagement.SetCustomStatus(payload.customMessage, isBusy);
			}
			else 
			{
				presenceManagement.SetAway(onlineStatus, isBusy);
			}
		}
	}

}}}}}
