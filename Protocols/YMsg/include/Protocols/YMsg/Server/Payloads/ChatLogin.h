//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	class ChatLogin : public Payload
	{
	public:

		static const serviceid_type ServiceId = 150;


	protected:

		struct Keys
		{
			static const key_type ClientId1 = 109;	//	FIXME: Main client id?
			static const key_type ClientId2 = 1;	//	FIXME: is this a profile id or just a duplicate pf 109
			static const key_type Region = 98;		//	TODO: Does this come from the XML based chat directory?
			static const key_type MessengerVersion = 135;
			static const key_type Unknown = 6;		//	TODO: Always "abdce". Figure out where this comes from
		};


	public:

		bool OnKeyPair(key_type key, string_view_type value) override;


	public:

		string_view_type	clientId1;
		string_view_type	clientId2;
	};

}}}}}
