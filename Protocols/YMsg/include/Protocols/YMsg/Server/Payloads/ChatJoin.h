//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	class ChatJoin : public Payload
	{
	public:

		static const serviceid_type ServiceId = 152;


	protected:

		struct Keys
		{
			static const key_type ClientId = 1;
			static const key_type Category = 129;
			static const key_type Room = 104;
			static const key_type Unknown = 62;	//	Always "2"
		};


	public:

		bool OnKeyPair(key_type key, string_view_type value) override;


	public:

		string_view_type	clientId;
		string_view_type	category;
		string_view_type	room;
	};

}}}}}
