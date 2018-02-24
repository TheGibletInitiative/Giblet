//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/PacketDispatcher.h>
#include <client.h>
#include <connectionlistener.h>
#include <functional>


namespace Giblet { namespace Bridges { namespace Gloox
{

	class ExtendedClient
		:
		public gloox::Client,
		public gloox::ConnectionListener
	{
	public:

		using authenticatedcallback_type = std::function<void(Protocols::YMsg::PacketDispatcher&, ExtendedClient&)>;
		using dispatcher_type = Protocols::YMsg::PacketDispatcher;

		ExtendedClient(
			authenticatedcallback_type authHandler,
			std::shared_ptr<dispatcher_type> dispatcher,
			const gloox::JID& jid,
			const std::string& password,
			int port = -1);

		virtual void initialize();

		virtual void setPresenceHandler(std::unique_ptr<gloox::PresenceHandler> handler);
		virtual bool pump();

		void onConnect() override;
		void onDisconnect(gloox::ConnectionError error) override;
		bool onTLSConnect(const gloox::CertInfo& info) override;


	protected:

		const authenticatedcallback_type		authHandler_;
		const std::shared_ptr<dispatcher_type>	dispatcher_;
		std::unique_ptr<gloox::PresenceHandler>	presenceHandler_;
	};

}}}
