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
	public:

		using status_type = detail::ContactBlockedStatus;
		using reason_type = detail::ContactBlockedFailedReason;


	protected:

		virtual void Build(
			session_type& session,
			string_view_type clientId,
			string_view_type contactId,
			status_type status,
			reason_type reason = reason_type::None)
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::ClientId, clientId);
			Append(Keys::ContactId, contactId);
			Append(Keys::Status, status);
			if (reason == reason_type::None)
			{
				Append(Keys::Reason, " ");
			}
			else
			{
				Append(Keys::Reason, reason);
			}
		}


	protected:

		//	NOTE: Some key values are different than in later versions (at least in my docs)
		struct Keys
		{
			static const key_type ClientId = 1;		//	FIXME: Shown as 0 in later versions. Verify.
			static const key_type ContactId = 5;
			static const key_type Status = 4;		//	FIXME: Shown as 13 in later versions. Verify.
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
			ContactBlockedOrUnblocked::Build(session, clientId, contactId, status_type::Blocked);
		}
	};


	class ContactUnblocked : public ContactBlockedOrUnblocked
	{
	public:

		using ContactBlockedOrUnblocked::Build;

		virtual void Build(session_type& session, string_view_type clientId, string_view_type contactId)
		{
			ContactBlockedOrUnblocked::Build(session, clientId, contactId, status_type::Unblocked);
		}
	};


	//	FIXME: This is untested/unused at the moment!
	class ContactNotBlocked : public ContactBlockedOrUnblocked
	{
	public:

		using ContactBlockedOrUnblocked::Build;

		virtual void Build(
			session_type& session,
			string_view_type clientId,
			string_view_type contactId,
			reason_type reason)
		{
			if (reason == reason_type::None)
			{
				throw std::invalid_argument("reason cannot be None");
			}

			ContactBlockedOrUnblocked::Build(
				session,
				clientId,
				contactId,
				status_type::Blocked,	//	FIXME: Verify this is correct. Should be Action::Error(3) I think!
				reason);
		}
	};

}}}}}
