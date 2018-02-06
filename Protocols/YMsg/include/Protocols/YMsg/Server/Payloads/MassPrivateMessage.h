//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	class MassPrivateMessage : public Payload
	{
	public:

		static const serviceid_type ServiceId = 23;


	protected:

		struct Keys
		{
			static const key_type ClientId = 1;
			static const key_type ContactId = 5;
			static const key_type Message = 14;
			static const key_type Encoding = 97;
		};

		using encoding_type = detail::TextEncoding;


	public:

		bool OnKeyPair(key_type key, string_view_type value) override;


	public:

		string_view_type				clientId;
		std::vector<string_view_type>	contactIds;
		string_view_type				message;
		encoding_type					encoding;
	};

}}}}}
