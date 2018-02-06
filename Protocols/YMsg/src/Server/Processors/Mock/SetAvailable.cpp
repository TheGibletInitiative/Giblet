//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/Mock/SetAvailable.h>
#include <Protocols/YMsg/Server/Builders/ContactOnline.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors { namespace Mock
{

	void SetAvailable::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);
		((void)payload);

		session.SetAvailability(availability_type::Available);

		for (auto profile(session.profiles_begin()); profile != session.profiles_end(); ++profile)
		{
			Builders::UpdateContactStatus contactOnlineBuilder;
			contactOnlineBuilder.Build(session, ContactInfo(ContactInfo::Linked, *profile, availability_type::Available));
			contactOnlineBuilder.Send(session);
		}
	}

}}}}}}
