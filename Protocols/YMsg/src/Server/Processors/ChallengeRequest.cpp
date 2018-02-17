//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/ChallengeRequest.h>
#include <Protocols/YMsg/Server/Builders/ChallengeResponse.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors
{

	void ChallengeRequest::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);

		//	We don't use any authentication at the moment so we just use method 0
		//	which causes messenger to omit credentials
		Server::Builders::ChallengeResponse builder;
		builder.Build(session.GetConnection(), payload.clientId);
		builder.Send(session.GetConnection());
	}

}}}}}
