//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	//	This is the same as AddContactRequest. Probably the same for accept as well. Need to consolidate.
	//	Rename to ContactRequest and add BuildRequest, BuildDeny, BuildStatus
	//	See the documentation in AddContactRequest for FIXME's and other information until consolidation occurs.
	class DenyContactAddRequest : public Builder
	{
	public:

		virtual void Build(
			connection_type& connection,
			string_view_type clientId,
			string_view_type contactId,
			string_view_type message)
		{
			Initialize(connection, ServiceId, AttributeId);
			Append(Keys::ContactId, contactId);
			Append(Keys::ClientId, clientId);
			Append(Keys::Message, message);
		}


	protected:

		struct Keys
		{
			static const key_type ContactId = 1;
			static const key_type ClientId = 3;
			static const key_type Message = 14;	//	Message encoding was not implemented until Messenger V7.0 (YMSG13?)
		};

		static const serviceid_type ServiceId = 15;
		//	1	Status message
		//	3	Buddy Add Request
		//	7	Deny Buddy Request
		static const attribute_type AttributeId = 7;
	};

}}}}}
