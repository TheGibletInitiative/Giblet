//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/AuthenticationPacketDispatcher.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class MockAuthenticationPacketDispatcher : public Giblet::Protocols::YMsg::AuthenticationPacketDispatcher
	{
	public:

		using AuthenticationPacketDispatcher::AuthenticationPacketDispatcher;

		void OnSessionAuthenticated(context_type& context) override;

	};

}}}
