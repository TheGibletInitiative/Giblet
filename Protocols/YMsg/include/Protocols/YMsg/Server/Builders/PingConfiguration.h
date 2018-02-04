//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class PingConfiguration : public Builder
	{
	public:

		virtual void Build(session_type& session)
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::Ticks, "60");
			Append(Keys::Tocks, "13");
		}


	protected:

		struct Keys
		{
			static const key_type Ticks = 143;
			static const key_type Tocks = 144;
		};

		static const serviceid_type ServiceId = 18;
		static const attribute_type AttributeId = 0;
	};

}}}}}
