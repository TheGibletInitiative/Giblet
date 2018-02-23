//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/SessionContext.h>
#include <Protocols/YMsg/PacketDispatcher.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace DispatchMaps
{

	namespace V11
	{
		using container_type = PacketDispatcher::container_type;

		container_type Create(std::shared_ptr<SessionContext> session);
	};

}}}}
