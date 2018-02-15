//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/ContactManager.h>
#include <memory>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class ContactManagementEvents
	{
	public:

		using string_type = std::string;			//	should be a UTF8 string
		using string_view_type = std::string_view;	//	should be a UTF8 string
		using encoding_type = detail::TextEncoding;
		using addrequeststatus_type = detail::AddContactRequestResultStatus;
		using availability_type = detail::Availability;


	public:

		ContactManagementEvents(
			std::shared_ptr<ClientConnection> connection,
			std::shared_ptr<BlockedContactManager> blockedContactManager,
			std::shared_ptr<ProfileManager> profileManager,
			std::shared_ptr<ContactManager> contactManager);
		ContactManagementEvents(const ContactManagementEvents&) = delete;
		virtual ~ContactManagementEvents() = default;


		//	Triggered by server
		virtual void OnAddContactRequest(string_view_type clientId, string_view_type contactId, string_view_type message, time_t timestamp);
		virtual void OnAddRequestPending(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnAddRequestDenied(string_view_type clientId, string_view_type contactId, string_view_type message);
		virtual void OnContactAdded(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnContactRemoved(string_view_type clientId, string_view_type contactId, string_view_type group);

		virtual void OnEmptyGroup(string_view_type clientId, string_view_type contactId);
		virtual void OnContactIsIneligible(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnContactNoExist(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnDuplicateContact(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnRemoveContactFailed(string_view_type clientId, string_view_type contactId, string_view_type group);

		virtual void OnGroupRenameFailed(string_view_type currentName, string_view_type newName);
		virtual void OnGroupRenamed(string_type currentName, string_type newName);


	protected:

		virtual void RefreshProfile() const;

		const std::shared_ptr<ClientConnection>			connection_;
		const std::shared_ptr<BlockedContactManager>	blockedContactManager_;
		const std::shared_ptr<ProfileManager>			profileManager_;
		const std::shared_ptr<ContactManager>			contactManager_;
	};


}}}
