//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/AuthenticationHandler.h>
#include <GlooxXMPPBridgeLibrary/ExtendedClient.h>


namespace Giblet { namespace Bridges { namespace Gloox
{

	class AuthenticationHandler
		:
		public std::enable_shared_from_this<AuthenticationHandler>,
		public Protocols::YMsg::AuthenticationHandler
	{
	public:

		using Protocols::YMsg::AuthenticationHandler::AuthenticationHandler;

		void OnClientAuthenticated(Protocols::YMsg::ConnectionPump& pump, Protocols::YMsg::PacketDispatcher& dispatcher) override;


	protected:

		virtual void OnSessionAuthenticated(Protocols::YMsg::PacketDispatcher& dispatcher, ExtendedClient& client);
		virtual void Populate(Protocols::YMsg::ContactManager& contactManager, ExtendedClient& client);
	};

}}}
