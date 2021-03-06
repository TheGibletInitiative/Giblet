//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/RenameContactGroup.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors
{

	void RenameContactGroup::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);

		session.ContactManagement().RenameGroup(string_type(payload.currentGroupName), string_type(payload.newGroupName));
	}

}}}}}
