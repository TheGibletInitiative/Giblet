//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/Authentication.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors
{

	void Authentication::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		//	FIXME: Need to define attributes the same way we do keys
		const auto initialAvailability(
			header.attribute == static_cast<std::underlying_type<detail::Availability>::type>(detail::Availability::Invisible) 
			? detail::Availability::Offline
			: detail::Availability::Available);

		session.BeginSession(rand(), payload.clientId1_, initialAvailability);
	}

}}}}}
