//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/Mock/RequestProfile.h>
#include <Protocols/YMsg/Server/Builders/ClientProfile.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors { namespace Mock
{

	void RequestProfile::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);
		((void)payload);

		Builders::ClientProfile	clientProfileBuilder;
		clientProfileBuilder.Build(session);
		clientProfileBuilder.Send(session);
	}

}}}}}}
