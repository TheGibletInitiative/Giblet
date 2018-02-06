//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{


	class TypingNotification : public Builder
	{
	public:

		using status_type = detail::TypingStatus;


	public:

		virtual void Build(
			session_type& session,
			string_view_type clientId,
			string_view_type contactId,
			status_type status)
		{
			static const string_view_type typingOperation("TYPING");

			Initialize(session, ServiceId, AttributeId);
			Append(Keys::Operation, typingOperation);
			Append(Keys::ClientId, clientId);	
			Append(Keys::ContactId, contactId);	
			Append(Keys::StatusOld, " ");		//	This is always blank on 5.6
			Append(Keys::Status, status);		
		}


	protected:

		struct Keys
		{
			static const key_type Operation = 49;	//	What to do : TYPING, WEBCAMINVITE
			static const key_type ClientId = 5;
			static const key_type ContactId = 4;
			static const key_type StatusOld = 14;
			static const key_type Status = 13;
		};

		static const serviceid_type ServiceId = 75;
		static const attribute_type AttributeId = 1;	//	NOTE: Attribute is 22 when sent by client
	};

}}}}}
