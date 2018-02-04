//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class PrivateMessageError : public Builder
	{
	public:

		virtual void Build(
			session_type& session,
			string_view_type clientId,
			string_view_type contactId,
			string_view_type message,
			TextEncoding encoding)
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::ContactId, contactId);	
			Append(Keys::ClientId, clientId);
			Append(Keys::ContactIdAgain, contactId);		
			Append(Keys::Message, message);
			Append(Keys::Encoding, encoding);	
		}


	protected:

		struct Keys
		{
			static const key_type ContactId = 1;		//	TODO: I think this may be the optional one :)
			static const key_type ClientId = 5;
			static const key_type ContactIdAgain = 4;	//	TODO: Find out if the duplicate is needed, why it's there, and if it can be a profile id
			static const key_type Message = 16;
			static const key_type Encoding = 97;
		};

		static const serviceid_type ServiceId = 6;
		static const attribute_type AttributeId = 0xffffffff;
	};

}}}}}
