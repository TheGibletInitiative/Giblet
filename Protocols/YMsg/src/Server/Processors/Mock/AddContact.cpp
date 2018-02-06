//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/Mock/AddContact.h>
#include <Protocols/YMsg/Server/Builders/AddContactRequestResult.h>
#include <Protocols/YMsg/Server/Builders/AddContactRequest.h>
#include <Protocols/YMsg/Server/Builders/ClientProfile.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors { namespace Mock
{

	void AddContact::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		static const string_type nonExistingUser("grag");
		((void)header);

		//	TODO: Make member def maybe
		using status_type = Builders::AddContactRequestResult::status_type;

		auto responseStatus(status_type::AddPending);

		if (session.IsContact(payload.contactId))
		{
			responseStatus = status_type::DuplicateContact;
		}
		else if (session.IsBlocked(payload.contactId))
		{
			//	FIXME: There is either a status code for this that we're missing or it's handled by Messenger.
			//	NOTE: It does seem to be handled by messenger but that doesn't mean third party clients worked that way
			responseStatus = status_type::GeneralFailure;
		}
		else if (payload.contactId == nonExistingUser)
		{
			responseStatus = status_type::ContactDoesNotExist;
		}
		else if (payload.group.empty())
		{
			//	FIXME: There is either a status code for this that we're missing or it's handled by Messenger
			responseStatus = status_type::GroupAlreadyExists;
		}


		//	Tell messenger the add request was sent
		Builders::AddContactRequestResult addRequestSentBuilder;
		addRequestSentBuilder.Build(session, payload.contactId, payload.clientId, payload.group, responseStatus);
		addRequestSentBuilder.Send(session);

		//	loopback the add request
		if (responseStatus == status_type::AddPending)
		{
			//	Add the contact to our session
			session.AddContact(
				payload.group,
				session_type::contact_info_type(session_type::contact_info_type::State::AddPending, payload.contactId));


			//	NOTE: Send a profile refresh so the name shows up in the buddy list!
			Builders::ClientProfile	clientProfileBuilder;
			clientProfileBuilder.Build(session);
			clientProfileBuilder.Send(session);


			//	If we are adding our own profile send it back to ourselves
			if (session.IsProfileOrClientId(payload.contactId))
			{
				Builders::AddContactRequest addRequestBuilder;
				addRequestBuilder.Build(
					session,
					payload.clientId,
					payload.contactId,
					payload.message,
					time(nullptr));
				addRequestBuilder.Send(session);
			}
		}
	}

}}}}}}
