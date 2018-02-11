//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class ProtocolAck : public Builder
	{
	public:

		virtual void Build(connection_type& connection)
		{
			Initialize(connection, ServiceId, AttributeId);
		}


	protected:

		static const serviceid_type ServiceId = 76;
		//	TODO: Since this is an ACK packet the attribute should be 1 but this doesn't seem
		//	to cause any problems. Need to investigate.
		static const attribute_type AttributeId = 0;
	};

}}}}}
