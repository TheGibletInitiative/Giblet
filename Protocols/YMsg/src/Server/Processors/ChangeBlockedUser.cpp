//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/ChangeBlockedUser.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors
{

	void ChangeBlockedUser::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);

		static const string_type blockContactMode("1");
		static const string_type unblockContactMode("2");

		if (payload.mode == blockContactMode)
		{
			session.GetBlockedContactManager().Add(payload.clientId, payload.contactId);
		}
		else if (payload.mode == unblockContactMode)
		{
			session.GetBlockedContactManager().Remove(payload.clientId, payload.contactId);
		}
	}

}}}}}
