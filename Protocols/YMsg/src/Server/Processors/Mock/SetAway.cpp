//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/Mock/SetAway.h>
#include <Protocols/YMsg/Server/Builders/ContactOnline.h>
#include <Protocols/YMsg/Server/Builders/ContactOffline.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors { namespace Mock
{

	void SetAway::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);

		const auto onlineStatus(to_enum<availability_type>(payload.statusId));

		if (onlineStatus == availability_type::Invisible)
		{
			session.SetAvailability(onlineStatus);

			for (auto profile(session.profiles_begin()); profile != session.profiles_end(); ++profile)
			{
				Builders::ContactOffline builder;
				builder.Build(session, *profile);
				builder.Send(session);
			}
		}
		//	FIXME: Add special handler for idle state to include time idle
		else
		{
			const bool busyState(payload.busyState.empty() ? false : std::stol(payload.busyState.data()) != 0);

			session.SetAvailability(onlineStatus);

			//	user online
			for (auto profile(session.profiles_begin()); profile != session.profiles_end(); ++profile)
			{
				const ContactInfo onlineContact(ContactInfo::State::Linked, *profile, payload.idleUnknown, onlineStatus, busyState, payload.customMessage);

				Builders::UpdateContactStatus builder;
				builder.Build(session, onlineContact);
				builder.Send(session);
			}
		}
	}

}}}}}}
