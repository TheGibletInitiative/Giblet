//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	class PrivateMessage : public Payload
	{
	public:

		static const serviceid_type ServiceId = 6;


	protected:

		struct Keys
		{
			static const key_type ClientId = 1;
			static const key_type ContactId = 5;
			static const key_type Message = 14;
			static const key_type Encoding = 97;
			static const key_type IMVironment = 63;
			static const key_type Unknown64 = 64;	//	TODO: Always "0". What does it MEAN?!?!
			//	TODO: Observed "0", "1" (was to buddy not def), 
			//	Is not included if contacting buddy showing offline
			//	is not included if contacting someone not on your buddy list
			static const key_type Unknown1002 = 1002;
		};

	public:

		bool OnKeyPair(key_type key, string_view_type value) override;


	public:

		string_view_type	clientId;
		string_view_type	contactId;
		string_view_type	message;
		string_view_type	encoding;	//	FIXME: Use enum.
	};

}}}}}
