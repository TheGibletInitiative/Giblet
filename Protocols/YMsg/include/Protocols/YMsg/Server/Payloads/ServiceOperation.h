//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	class ServiceOperation : public Payload
	{
	public:

		static const serviceid_type ServiceId = 75;
		//	TODO: Define attributes
		//	Typing has an attribute of 22 (or 20 as seen in older even more incomplete docs).
		//	webcam invite has an attribute of 1


	protected:

		struct Keys
		{
			static const key_type Operation = 49;
			static const key_type ClientId = 1;
			static const key_type Status = 14;		
			static const key_type Action = 13;		
			static const key_type ContactId = 5;
			static const key_type Unknown = 1002;	//	always "1"
		};


	public:

		bool OnKeyPair(key_type key, string_view_type value) override;


	public:

		//	NOTE: The meaning of the status and action fields are defined by the value in operation.
		//	All other fields (except federation id in 6.0+) should be present.
		string_view_type	operation;
		string_view_type	clientId;
		string_view_type	status;		//	TODO: Make variant<enum, enum>.
		string_view_type	action;		//	TODO: Make variant<enum, enum>.
		string_view_type	contactId;
	};

}}}}}
