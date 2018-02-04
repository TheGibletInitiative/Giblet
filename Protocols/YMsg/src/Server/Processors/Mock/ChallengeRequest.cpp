//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/Mock/ChallengeRequest.h>
#include <Protocols/YMsg/Server/Builders/ChallengeResponse.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors { namespace Mock
{

	void ChallengeRequest::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);

		//	NOTE: We no longer initialize the session with the client id here as that should occur
		//	after authentication and via an event from the login service
		Builders::ChallengeResponse builder;
		builder.Build(session, payload.clientId);
		builder.Send(session);
	}

}}}}}}
