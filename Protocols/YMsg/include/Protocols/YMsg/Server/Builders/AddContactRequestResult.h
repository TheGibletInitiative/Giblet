//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class AddContactRequestResult : public Builder
	{
	public:

		//	TODO: Check to see if any of these requires a special attribute
		enum class StatusCode
		{
			AddPending = 0,
			GeneralFailure = 1,
			DuplicateContact = 2,			//	Already in list
			ContactDoesNotExist = 3,
			InvalidCookies = 4,
			RecursiveLockup = 5,			//	This REALLY borks messenger
			ContactListFull = 6,
			TooManyGroups = 7,
			GroupAlreadyExists = 8,			//	Not sure what would trigger this
			CannotDeleteContact = 9,
			BlockedListFull = 10,
			CannotDeleteReservedId = 11,	//	Can't delete reserved id. Part of parental controls maybe?
			CannotDeleteContact2 = 12,		//	Duplicate status code. Gotta have two different reasons :)
		};


		virtual void Build(
			session_type& session,
			string_view_type clientId,
			string_view_type contactId,
			string_view_type contactGroup,
			StatusCode status)
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::ContactId, contactId);
			Append(Keys::ClientId, clientId);
			Append(Keys::Group, contactGroup);
			Append(Keys::Status, status);
		}


	protected:

		struct Keys
		{
			static const key_type ContactId = 1;
			static const key_type ClientId = 7;
			static const key_type Group = 65;
			static const key_type Status = 66;
		};

		static const serviceid_type ServiceId = 131;
		static const attribute_type AttributeId = 1;
	};

}}}}}
