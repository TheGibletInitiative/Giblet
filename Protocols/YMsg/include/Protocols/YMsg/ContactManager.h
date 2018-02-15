//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/ClientConnection.h>
#include <Protocols/YMsg/ProfileManager.h>
#include <Protocols/YMsg/BlockedContactManager.h>
#include <map>


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
			string_view_type id,
			string_view_type group,
			State state,
			availability_type availability = availability_type::Offline,
			bool isBusy = false,
			string_view_type customMessage = string_view_type())
			:
			id(id),
			group(group),
			state(state),
			availability(availability),
			isBusy(isBusy),
			customMessage(customMessage)
		{}

		ContactInfo(
			string_view_type id,
			string_view_type group,
			State state,
			//	TODO: Need to figure out what the possible values are and stuff it in an enum
			string_view_type idleUnknown,
			//	FIXME: Idle unknown assume idle state!
			availability_type availability = availability_type::Offline,
			//	FIXME: Idle could assume busy/not available state!
			bool isBusy = false,
			string_view_type customMessage = string_view_type())
			:
			id(id),
			group(group),
			availability(availability),
			state(state),
			idleUnknown(idleUnknown),
			isBusy(isBusy),
			customMessage(customMessage)
		{}

		string_type			id;
		string_type			group;
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
		using container_type = std::map<string_type, contact_info_type>;
		using const_iterator = container_type::const_iterator;
		using iterator = container_type::iterator;
		using encoding_type = detail::TextEncoding;
		using addrequeststatus_type = detail::AddContactRequestResultStatus;
		using availability_type = detail::Availability;


	public:

		ContactManager() = default;
		ContactManager(const ContactManager&) = delete;
		virtual ~ContactManager() = default;

		virtual void LoadContact(const contact_info_type& contact);

		const_iterator begin() const;
		const_iterator end() const;

		//
		virtual bool IsContact(string_view_type contactId) const;

		virtual void AddPendingContact(string_view_type contactId, string_view_type group);
		virtual void SetContactAdded(string_view_type contactId, string_view_type group);
		virtual void RemoveContact(string_view_type contactId, string_view_type group);
		virtual void RenameGroup(string_type currentName, string_type newName);

		virtual const contact_info_type* SetContactAvailable(string_view_type contactId);
		virtual const contact_info_type* SetContactIdle(string_view_type contactId, string_view_type idleUnknown, string_view_type message, bool isBusy);
		virtual const contact_info_type* SetContactAway(string_view_type contactId, availability_type availability, bool isBusy);
		virtual const contact_info_type* SetContactStatusMessage(string_view_type contactId, string_view_type message, bool isBusy);
		virtual const contact_info_type* SetContactOffline(string_view_type contactId);


	protected:

		const_iterator FindContact(string_view_type contactId) const;
		iterator FindContact(string_view_type contactId);

		container_type			contacts_;			//	TODO: Move to mocked address book
	};

}}}
