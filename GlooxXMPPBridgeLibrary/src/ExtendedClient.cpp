//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <GlooxXMPPBridgeLibrary/ExtendedClient.h>
#include <rostermanager.h>


namespace Giblet { namespace Bridges { namespace Gloox
{

	ExtendedClient::ExtendedClient(
		authenticatedcallback_type authHandler,
		std::shared_ptr<Protocols::YMsg::PacketDispatcher> dispatcher,
		const gloox::JID& jid,
		const std::string& password,
		int port)
		:
		Client(jid, password, port),
		authHandler_(authHandler),
		dispatcher_(dispatcher)
	{}


	void ExtendedClient::initialize()
	{
		registerConnectionListener(this);
	}



	void ExtendedClient::setPresenceHandler(std::unique_ptr<gloox::PresenceHandler> handler)
	{
		if (presenceHandler_)
		{
			removePresenceHandler(presenceHandler_.get());
			presenceHandler_.release();
		}

		registerPresenceHandler(handler.get());
		presenceHandler_ = move(handler);
	}


	bool ExtendedClient::pump()
	{
		return recv(0) == gloox::ConnectionError::ConnNoError;
	}


	void ExtendedClient::onConnect()
	{
		authHandler_(*dispatcher_, *this);
	}


	void ExtendedClient::onDisconnect(gloox::ConnectionError e)
	{
	}


	bool ExtendedClient::onTLSConnect(const gloox::CertInfo& info)
	{
		return true;
	}

}}}
