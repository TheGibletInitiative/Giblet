//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class PrivateMessage : public Builder
	{
	public:

		using encoding_type = detail::TextEncoding;


		virtual void Build(
			session_type& session,
			string_view_type clientId,
			string_view_type contactId,
			string_view_type message,
			encoding_type encoding)
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::ClientId, clientId);	
			Append(Keys::ContactId, contactId);
			Append(Keys::ClientIdAgain, clientId);		
			Append(Keys::Message, message);
			Append(Keys::Encoding, encoding);	
		}


	protected:

		struct Keys
		{
			static const key_type ClientId = 1;			//	TODO: I think this may be the optional one :)
			static const key_type ContactId = 5;
			static const key_type ClientIdAgain = 4;	//	TODO: Find out if the duplicate is needed, why it's there, and if it can be a profile id
			static const key_type Message = 14;
			static const key_type Encoding = 97;
			static const key_type IMVironment = 63;
			static const key_type Unknown64 = 64;		//	Observed as "0". Related to IMVironments.
		};

		static const serviceid_type ServiceId = 6;
		//	TODO: This can be other (odd) values depending on the message, p2p connection, offline status, etc
		static const attribute_type AttributeId = 1;
	};

}}}}}
