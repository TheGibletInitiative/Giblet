//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/AddContact.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors
{

	void AddContact::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);

		//	Add the contact to our session
		session.ContactManagement().AddContact(payload.clientId, payload.contactId, payload.group, payload.message);
	}

}}}}}
