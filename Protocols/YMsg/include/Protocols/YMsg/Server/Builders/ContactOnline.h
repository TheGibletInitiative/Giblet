//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class SetContactStatusBase : public Builder
	{
	protected:

		static const serviceid_type ServiceId = 1;

		struct Keys
		{
			static const key_type Count = 8;

			//	User list entries
			struct ContactInfo
			{
				static const key_type ContactId = 7;
				static const key_type Availability = 10;	//	See Availability enum
				static const key_type Unknown11 = 11;		//	TODO: Possible timestamp, crc, session id, etc.. Observed as "0", "BD00ADE4", "C20AAC3C"
				static const key_type Unknown13 = 13;		//	FIXME: Figure out what this really is! Observed as "0"
				static const key_type CustomMessage = 19;
				static const key_type IsInChat = 17;
				static const key_type SecondsIdle = 137;	//	TODO: Possible 6.0+ but observed in 5.5/5.6 timeframe
				static const key_type IdleUnknown138 = 138;	//	FIXME: Possible 6.0+
				static const key_type IsBusy= 47;
			};
		};

		using availability_type = detail::Availability;


	protected:


		virtual void AppendContact(const ContactInfo& contact)
		{
			//	FIXME: Some data is not necessary depending on the value of contact.availability. Ajust to handle this
			Append(Keys::ContactInfo::ContactId, contact.id);
			Append(Keys::ContactInfo::Availability, contact.availability);
			//	FIXME: Now sure how to handle if this is empty
			//	FIXME: This appears to have something to do with idle but we're including it just in case.
			if (!contact.idleUnknown.empty())
			{
				Append(Keys::ContactInfo::IdleUnknown138, contact.idleUnknown);
			}
			if (contact.availability == availability_type::Idle)
			{
				//	FIXME: Determine value by timestamp that _should_ be kept in the contact details. It should use 0 to indicate no show or whatever
				Append(Keys::ContactInfo::SecondsIdle, "0");
			}
			Append(Keys::ContactInfo::IsInChat, contact.isInChat);
			if (contact.availability == availability_type::Custom)
			{
				Append(Keys::ContactInfo::CustomMessage, contact.customMessage);
			}
			Append(Keys::ContactInfo::IsBusy, contact.isBusy);
			Append(Keys::ContactInfo::Unknown13, "1");
		}


		virtual void AppendContacts(const std::vector<ContactInfo>& contacts)
		{
			//	TODO: Verify whether the contact count can be omitted or if we need to include it with a value of "0".
			Append(Keys::Count, contacts.size());

			for (const auto& contact : contacts)
			{
				AppendContact(contact);
			}
		}
	};




	class UpdateContactStatus : public SetContactStatusBase
	{
	protected:

		//	NOTE: The attribute is important.
		//	1 has only been observed after login when a contact comes online or changes their status.
		//	If 1 is used notifications are shown.
		static const attribute_type AttributeId = 1;


	public:

		virtual void Build(session_type& session, const ContactInfo& contact)
		{
			//	NOTE: Some odd behavior was observed when sending multiple contacts. Need to verify to see
			//	if it wasn't something else being worked on at the time that caused the issue.
			Initialize(session, ServiceId, AttributeId);
			//	NOTE: Messenger 5.6 accepts the update if this field is omitted. If the field is set
			//	to "0" the update is ignored. It has been observed as "1" so we include it.
			Append(Keys::Count, 1);	
			AppendContact(contact);
		}
	};


	//	Consider another name?
	class SetContactStatus : public SetContactStatusBase
	{
	protected:

		struct Keys : SetContactStatusBase::Keys
		{
			static const key_type ClientId1 = 0;
			static const key_type ClientId2 = 1;
		};


		//	NOTE: The attribute is important.
		//	0 has only been observed during a login and includes the client id.
		//	If 0 is used no notifications are displayed.
		//	TODO: Determine if this should be used when the client requests the profile
		static const attribute_type AttributeId = 0;


	public:

		virtual void Build(
			session_type& session,
			string_view_type clientId,
			const std::vector<ContactInfo>& contacts)
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::ClientId1, clientId);
			Append(Keys::ClientId2, clientId);
			AppendContacts(contacts);
		}

	};

}}}}}
