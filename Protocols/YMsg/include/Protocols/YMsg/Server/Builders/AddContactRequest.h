//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>
#include <ctime>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	//	TODO: This is the same as DenyContactAddRequest - consolidate and add all possible attributes
	class AddContactRequest : public Builder
	{
	public:

		virtual void Build(
			session_type& session,
			string_view_type clientId,
			string_view_type contactId,
			string_view_type message,		//	TODO: message, messageEncoding, and timestamp should be delivered as a group.
			time_t timestamp)				//	TODO: ^^^
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::ContactId, contactId);
			Append(Keys::ClientId, clientId);
			Append(Keys::Message, message);
			Append(Keys::Timestamp, timestamp);
		}


	protected:

		struct Keys
		{
			static const key_type ContactId = 1;
			static const key_type ClientId = 3;
			static const key_type Message = 14;	//	Message encoding was not implemented until Messenger V7.0 (YMSG13?)
			static const key_type Timestamp = 15;
	};

		static const serviceid_type ServiceId = 15;
		//	1	Status message
		//	3	Buddy Add Request
		//	7	Deny Buddy Request
		static const attribute_type AttributeId = 3;
	};

}}}}}
