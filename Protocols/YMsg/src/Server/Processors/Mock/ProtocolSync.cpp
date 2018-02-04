//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/Mock/ProtocolSync.h>
#include <Protocols/YMsg/Server/Builders/ProtocolAck.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors { namespace Mock
{

	void ProtocolSync::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);
		((void)payload);

		session.OnProtocolSync(header.protoVersion);

		Builders::ProtocolAck builder;
		builder.Build(session);
		builder.Send(session);
	}

}}}}}}
