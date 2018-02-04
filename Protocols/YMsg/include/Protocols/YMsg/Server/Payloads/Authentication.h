//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	class Authentication : public Payload
	{
	public:

		static const serviceid_type ServiceId = 84;

	protected:

		struct Keys
		{
			static const key_type AuthResponse1 = 6;
			static const key_type AuthResponse2 = 96;
			static const key_type ClientId1 = 0;
			static const key_type ProfileId = 2;
			static const key_type ClientId2 = 1;
			static const key_type ClientVersion = 135;
			static const key_type Unknown148 = 148;		//	Observed "420" and "480"
		};


	public:


		bool OnKeyPair(key_type key, string_view_type value) override;


	public:

		string_view_type	clientId1_;
		string_view_type	clientId2_;
		string_view_type	authResponse1_;
		string_view_type	authResponse2_;
		string_view_type	clientVersion_;
		string_view_type	unknown148_;
		std::vector<string_view_type>	profileIds_;
		
	};

}}}}}
