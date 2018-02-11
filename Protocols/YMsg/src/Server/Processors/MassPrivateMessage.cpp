//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/MassPrivateMessage.h>
#include <Protocols/YMsg/Server/Builders/SystemMessage.h>
#include <ctime>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors
{

	void MassPrivateMessage::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);
		((void)payload);

		Builders::SystemMessage builder;

		builder.Build(session.GetConnection(), "Sending mass private messages is not supported.", time(nullptr));
		builder.Send(session.GetConnection());
	}

}}}}}
