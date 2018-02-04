//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	class SetAway : public Payload
	{
	public:

		static const serviceid_type ServiceId = 3;


	protected:

		struct Keys
		{
			static const key_type StatusId = 10;
			static const key_type IdleUnknown138 = 138;	//	1 = no, 2 = ?yes?
			static const key_type CustomMessage = 19;	//	Only included if StatusId is set to custom
			static const key_type BusyState = 47;		//	Only included if StatusId is set to custom
		};


	public:

		bool OnKeyPair(key_type key, string_view_type value) override;


	public:

		string_view_type	statusId;		//	TODO: Use enum.
		string_view_type	customMessage;
		string_view_type	idleUnknown;
		string_view_type	busyState;		//	TODO: Use bool.
	};

}}}}}
