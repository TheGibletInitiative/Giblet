//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/Mock/RenameContactGroup.h>
#include <Protocols/YMsg/Server/Builders/ContactGroupRenamed.h>
#include <Protocols/YMsg/Server/Builders/ClientProfile.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors { namespace Mock
{

	void RenameContactGroup::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		((void)header);

		if (!session.RenameGroup(string_type(payload.currentGroupName), string_type(payload.newGroupName)))
		{
			return;
		}

		Builders::ContactGroupRenamed builder;

		builder.Build(session, payload.clientId, payload.newGroupName, payload.currentGroupName);
		builder.Send(session);

		//	NOTE: Send a profile refresh so the change shows up in the buddy list!
		Builders::ClientProfile	clientProfileBuilder;
		clientProfileBuilder.Build(session);
		clientProfileBuilder.Send(session);
	}

}}}}}}
