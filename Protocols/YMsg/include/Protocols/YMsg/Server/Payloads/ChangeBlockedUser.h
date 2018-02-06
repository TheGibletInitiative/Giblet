//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	class ChangeBlockedUser : public Payload
	{
	public:

		static const serviceid_type ServiceId = 133;


	protected:

		struct Keys
		{
			static const key_type ClientId = 1;
			static const key_type ContactId = 7;
			static const key_type Mode = 13;
		};


	public:

		bool OnKeyPair(key_type key, string_view_type value) override;


	public:

		string_view_type	clientId;
		string_view_type	contactId;
		string_view_type	mode;			//	TODO: Change to enum
	};

}}}}}
