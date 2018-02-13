//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/ContactManager.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class PresenceEvents
	{
	public:

		using string_type = std::string;			//	should be a UTF8 string
		using string_view_type = std::string_view;	//	should be a UTF8 string
		using encoding_type = detail::TextEncoding;
		using addrequeststatus_type = detail::AddContactRequestResultStatus;
		using availability_type = detail::Availability;


	public:

		explicit PresenceEvents(
			std::shared_ptr<ClientConnection> connection,
			std::shared_ptr<ContactManager> contactManager);

		PresenceEvents(const PresenceEvents&) = delete;


		//	Triggered by server
		virtual void OnContactAvailable(string_view_type contactId);
		virtual void OnContactIdle(string_view_type contactId, string_view_type idleUnknown, string_view_type message, bool isBusy);
		virtual void OnContactAway(string_view_type contactId, availability_type availability, bool isBusy);
		virtual void OnContactStatusMessage(string_view_type contactId, string_view_type message, bool isBusy);
		virtual void OnContactOffline(string_view_type contactId);

	protected:

		const std::shared_ptr<ClientConnection>			connection_;
		const std::shared_ptr<ContactManager>			contactManager_;
	};


}}}
