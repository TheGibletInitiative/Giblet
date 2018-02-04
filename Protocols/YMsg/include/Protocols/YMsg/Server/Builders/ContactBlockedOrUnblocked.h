//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class ContactBlockedOrUnblocked : public Builder
	{
	protected:

		enum class Event
		{
			Blocked = 1,
			Unblocked = 2
		};

		virtual void Build(
			session_type& session,
			string_view_type clientId,
			string_view_type contactId,
			Event event,
			string_view_type reason = " ")	//	TODO: Create enum
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::ClientId, clientId);	//	from
			Append(Keys::ContactId, contactId);	//	to
			Append(Keys::Event, event);
			Append(Keys::Reason, reason);		//	appears to be an error code. 0 = success. !0 generates general failure.
		}


	protected:

		//	NOTE: Some key values are different than in later versions (at least in my docs)
		struct Keys
		{
			static const key_type ClientId = 1;		//	FIXME: Shown as 0 in later versions. Verify.
			static const key_type ContactId = 5;
			static const key_type Event = 4;			//	FIXME: Shown as 13 in later versions. Verify.
			static const key_type Reason = 66;		//	FIXME: Determine reasons and when they apply.
		};

		static const serviceid_type ServiceId = 133;
		//	FIXME: Verify this is the only attribute used. May have a different on for failure (ContactNotBlocked)
		static const attribute_type AttributeId = 1;
	};


	class ContactBlocked : public ContactBlockedOrUnblocked
	{
	public:

		virtual void Build(session_type& session, string_view_type clientId, string_view_type contactId)
		{
			ContactBlockedOrUnblocked::Build(session, clientId, contactId, Event::Blocked);
		}
	};


	class ContactUnblocked : public ContactBlockedOrUnblocked
	{
	public:

		using ContactBlockedOrUnblocked::Build;

		virtual void Build(session_type& session, string_view_type clientId, string_view_type contactId)
		{
			ContactBlockedOrUnblocked::Build(session, clientId, contactId, Event::Unblocked);
		}
	};


	//	FIXME: This is untested/unused at the moment!
	class ContactNotBlocked : public ContactBlockedOrUnblocked
	{
	public:

		using ContactBlockedOrUnblocked::Build;

		enum class Reason
		{
			Duplicate = 2,
			IsFriend = 12,
			//	 NotIgnored = 3
		};

		virtual void Build(
			session_type& session,
			string_view_type clientId,
			string_view_type contactId,
			Reason reason)
		{
			ContactBlockedOrUnblocked::Build(
				session,
				clientId,
				contactId,
				Event::Blocked,	//	FIXME: Verify this is correct. Should be Action::Error(3) I think!
				std::to_string(static_cast<int>(reason)));
		}
	};

}}}}}
