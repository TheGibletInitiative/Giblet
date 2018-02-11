//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/ClientConnection.h>
#include <Protocols/YMsg/ProfileManager.h>
#include <Protocols/YMsg/BlockedContactManager.h>
#include <vector>
#include <string>
#include <string_view>
#include <map>
#include <memory>


namespace Giblet { namespace Protocols { namespace YMsg
{

	struct ContactInfo
	{
		enum State
		{
			Linked,
			AddPending
		};

		using string_type = std::string;
		using string_view_type = std::string_view;
		using availability_type = detail::Availability;
		using client_type = detail::ClientType;

		explicit ContactInfo(
			State state,
			string_view_type id,
			availability_type availability = availability_type::Offline,
			bool isBusy = false,
			string_view_type customMessage = string_view_type())
			:
			id(id),
			state(state),
			availability(availability),
			isBusy(isBusy),
			customMessage(customMessage)
		{}

		ContactInfo(
			State state,
			string_view_type id,
			//	TODO: Need to figure out what the possible values are and stuff it in an enum
			string_view_type idleUnknown,
			//	FIXME: Idle unknown assume idle state!
			availability_type availability = availability_type::Offline,
			//	FIXME: Idle could assume busy/not available state!
			bool isBusy = false,
			string_view_type customMessage = string_view_type())
			:
			id(id),
			availability(availability),
			state(state),
			idleUnknown(idleUnknown),
			isBusy(isBusy),
			customMessage(customMessage)
		{}

		string_type			id;
		availability_type	availability;
		bool				isBusy;
		string_type			customMessage;
		string_type			idleUnknown;
		State				state;
		bool				isInChat;
	};



	class ContactManager
	{
	public:

		using string_type = std::string;			//	should be a UTF8 string
		using string_view_type = std::string_view;	//	should be a UTF8 string
		using contact_info_type = ContactInfo;
		using container_type = std::vector<contact_info_type>;
		using const_iterator = container_type::const_iterator;
		using iterator = container_type::iterator;
		using grouped_container_type = std::map<string_type, std::vector<string_type>>;
		using grouped_const_iterator = grouped_container_type::const_iterator;
		using encoding_type = detail::TextEncoding;
		using addrequeststatus_type = detail::AddContactRequestResultStatus;
		using availability_type = detail::Availability;


	public:

		explicit ContactManager(
			std::shared_ptr<ClientConnection> connection,
			std::shared_ptr<BlockedContactManager> blockedContactManager,
			std::shared_ptr<ProfileManager> profileManager);

		ContactManager(const ContactManager&) = delete;

		virtual void LoadContact(string_view_type group, const contact_info_type& contact);

		const_iterator begin() const;
		const_iterator end() const;

		grouped_const_iterator grouped_contacts_begin() const;
		grouped_const_iterator grouped_contacts_end() const;

		//
		virtual bool IsContact(string_view_type contactId) const;

		//	Triggered by client
		virtual void DenyRequest(string_view_type clientId, string_view_type contactId, string_view_type message) = 0;
		virtual void AddContact(string_view_type clientId, string_view_type contactId, string_view_type group, string_view_type message) = 0;
		virtual void RemoveContact(string_view_type clientId, string_view_type contactId, string_view_type group) = 0;
		virtual void RenameGroup(string_type currentName, string_type newName) = 0;


	public:

		//	Triggered by server
		virtual void OnAddRequestDenied(string_view_type clientId, string_view_type contactId, string_view_type message);
		virtual void OnAddContactRequest(string_view_type clientId, string_view_type contactId, string_view_type message, time_t timestamp);
		virtual void OnRemoveContactFailed(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnAddRequestPending(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnEmptyGroup(string_view_type clientId, string_view_type contactId);
		virtual void OnContactIsIneligible(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnContactNoExist(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnDuplicateContact(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnContactAdded(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnContactRemoved(string_view_type clientId, string_view_type contactId, string_view_type group);
		virtual void OnGroupRenameFailed(string_view_type currentName, string_view_type newName);
		virtual void OnGroupRenamed(string_type currentName, string_type newName);

		virtual void OnContactAvailable(string_view_type contactId);
		virtual void OnContactIdle(string_view_type contactId, string_view_type idleUnknown, string_view_type message, bool isBusy);
		virtual void OnContactAway(string_view_type contactId, availability_type availability, bool isBusy);
		virtual void OnContactStatusMessage(string_view_type contactId, string_view_type message, bool isBusy);
		virtual void OnContactOffline(string_view_type contactId);

	protected:

		virtual void RefreshProfile() const;

		const_iterator FindContact(string_view_type contactId) const;
		iterator FindContact(string_view_type contactId);

		const std::shared_ptr<ClientConnection>			connection_;
		const std::shared_ptr<BlockedContactManager>	blockedContactManager_;
		const std::shared_ptr<ProfileManager>			profileManager_;
		container_type			contacts_;			//	TODO: Move to mocked address book
		grouped_container_type	groupedContacts_;	//	TODO: Move to mocked address book
	};

}}}
