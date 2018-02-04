//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class SystemMessage : public Builder
	{
	public:

		virtual void Build(session_type& session, string_view_type message, time_t timestamp)
		{
			//	During testing the message window was able to disable a utf8 encoded string without providing
			//	the encoding key pair (see private message - service 6). 
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::Message, message);	
			Append(Keys::Timestamp, timestamp);	
		}


	protected:

		struct Keys
		{
			static const key_type Message = 14;
			static const key_type Timestamp = 15;
		};

		static const serviceid_type ServiceId = 20;
		static const attribute_type AttributeId = 1;
	};

}}}}}
