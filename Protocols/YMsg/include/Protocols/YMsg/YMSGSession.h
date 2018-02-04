//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <map>
#include <winsock2.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	enum class Availability
	{
		Offline =		 -1,
		Available =		  0,
		Brb =			  1,
		Busy =			  2,
		Notathome =		  3,
		NotAtDesk =		  4,
		NotInOffice =	  5,
		OnPhone =		  6,
		OnVacation =	  7,
		OutToLunch =	  8,
		SteppedOut =	  9,
		Invisible =		 12,
		Custom =		 99,
		Idle =			999,
	};

	struct ContactInfo
	{
		enum State
		{
			Linked,
			AddPending
		};

		using string_type = std::string;
		using string_view_type = std::string_view;

		explicit ContactInfo(
			State state,
			string_view_type id,
			Availability availability = Availability::Offline,
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
			Availability availability = Availability::Offline,
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

		string_type		id;
		Availability	availability;
		bool			isBusy;
		string_type		customMessage;
		string_type		idleUnknown;
		State			state;
	};




	class YMSGSession
	{
	public:

		using string_type = std::string;			//	should be a UTF8 string
		using string_view_type = std::string_view;	//	should be a UTF8 string
		using contact_info_type = ContactInfo;
		using sessionid_type = uint32_t;
		using protocolversion_type = uint16_t;
		using const_profile_iterator = std::vector<string_type>::const_iterator;
		using const_blockeduser_iterator = std::vector<string_type>::const_iterator;

		using contact_container_type = std::vector<contact_info_type>;
		using const_contact_iterator = contact_container_type::const_iterator;
		using groupedcontact_container_type = std::map<string_type, std::vector<string_type>>;
		using groupedcontact_const_iterator = groupedcontact_container_type::const_iterator;


	public:

		explicit YMSGSession(SOCKET sessionSocket);

		YMSGSession(const YMSGSession&) = delete;



		void OnProtocolSync(protocolversion_type version)
		{
			protocolVersion_ = version;
		}

		void BeginSession(sessionid_type id, string_view_type clientId, Availability initialAvailability)
		{
			sessionId_ = id;
			availability_ = initialAvailability;
			clientId_ = clientId;
			loggedIn_ = true;
		}



		//
		void AddProfile(string_view_type profileId);
		void AddContact(string_view_type group, const contact_info_type& contact);
		void AddBlockedId(string_view_type id);
		void RemoveContact(string_view_type contactId);

		bool RenameGroup(string_type currentName, string_type newName);

		//
		bool IsContact(string_view_type contactId) const
		{
			for (const auto& contact : contacts_)
			{
				if (contact.id == contactId)
				{
					return true;
				}
			}

			return false;
		}

		bool IsBlocked(string_view_type contactId) const
		{
			return find(blockedUsers_.begin(), blockedUsers_.end(), contactId) != blockedUsers_.end();
		}


		bool IsProfileOrClientId(string_view_type id) const
		{
			return clientId_ == id || find(profiles_.begin(), profiles_.end(), id) != profiles_.end();
		}


		//
		sessionid_type GetSessionId() const
		{
			return sessionId_;
		}

		protocolversion_type GetProtocolVersion() const
		{
			return protocolVersion_;
		}

		string_view_type GetClientId() const
		{
			return clientId_;
		}


		void SetAvailability(Availability availability)
		{
			availability_ = availability;
		}



		//
		const_profile_iterator profiles_begin() const
		{
			return profiles_.begin();
		}

		const_profile_iterator profiles_end() const
		{
			return profiles_.end();
		}


		const_contact_iterator contacts_begin() const
		{
			return contacts_.begin();
		}

		const_contact_iterator contacts_end() const
		{
			return contacts_.end();
		}


		groupedcontact_const_iterator grouped_contacts_begin() const
		{
			return groupedContacts_.begin();
		}

		groupedcontact_const_iterator grouped_contacts_end() const
		{
			return groupedContacts_.end();
		}


		const_blockeduser_iterator blockedusers_begin() const
		{
			return blockedUsers_.begin();
		}
		
		const_blockeduser_iterator blockedusers_end() const
		{
			return blockedUsers_.end();
		}



		//
		SOCKET GetSocket() const;	//	TODO: Abstract SOCKET - switch to Boost ASIO in the near future
		void SendToClient(const char *data, const size_t length);


	protected:


		const_contact_iterator FindContact(string_view_type contactId) const;

		SOCKET							sessionSocket_;
		sessionid_type					sessionId_;
		protocolversion_type			protocolVersion_;
		bool							loggedIn_;
		string_type						clientId_;
		std::vector<string_type>		profiles_;			//	TODO: Move to mocked profile manager
		contact_container_type			contacts_;			//	TODO: Move to mocked address book
		groupedcontact_container_type	groupedContacts_;	//	TODO: Move to mocked address book
		std::vector<string_type>		blockedUsers_;		//	TODO: Move to mocked blocked contact manager
		Availability					availability_;
	};

}}}
