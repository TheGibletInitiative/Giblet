//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/ChatJoin.h>
#include <Protocols/YMsg/Server/Builders/ChatroomJoined.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors
{

	void ChatJoin::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);

		static const std::vector<string_view_type> participants{ { "donna", "dave", "jay", "martin", "joe", "barry", "greg" } };

		Builders::ChatroomJoined builder;

		builder.Build(session.GetConnection(), payload.category, payload.room, "topic", "voicecookie--", "othercookie--", participants);
		builder.Send(session.GetConnection());
	}

}}}}}
