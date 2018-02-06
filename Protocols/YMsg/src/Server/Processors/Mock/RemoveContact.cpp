//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/Mock/RemoveContact.h>
#include <Protocols/YMsg/Server/Builders/ContactRemoved.h>
#include <Protocols/YMsg/Server/Builders/ClientProfile.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors { namespace Mock
{

	void RemoveContact::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);

		using status_type = Builders::ContactRemoved::status_type;

		auto statusCode(status_type::Success);
		if (!session.IsContact(payload.contactId))
		{
			statusCode = status_type::GeneralFailure;
		}

		if (statusCode == status_type::Success)
		{
			session.RemoveContact(payload.contactId);
		}

		Builders::ContactRemoved contactRemovedBuilder;
		contactRemovedBuilder.Build(session, payload.clientId, payload.contactId, payload.group, statusCode);
		contactRemovedBuilder.Send(session);

		//	NOTE: Send a profile refresh so the name shows up in the buddy list!
		if(statusCode == status_type::Success)
		{
			Builders::ClientProfile	clientProfileBuilder;
			clientProfileBuilder.Build(session);
			clientProfileBuilder.Send(session);
		}

	}

}}}}}}
