//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class ProfileDeactivated : public Builder
	{
	public:

		virtual void Build(connection_type& connection, string_view_type profileId)
		{
			Initialize(connection, ServiceId, AttributeId);
			Append(Keys::ProfileId, profileId);	
		}


	protected:

		struct Keys
		{
			static const key_type ProfileId = 3;
		};

		static const serviceid_type ServiceId = 8;
		static const attribute_type AttributeId = 1;
	};

}}}}}
