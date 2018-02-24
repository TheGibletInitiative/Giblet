//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <GlooxXMPPBridgeLibrary/AuthenticationHandler.h>
#include <GlooxXMPPBridgeLibrary/ExtendedClient.h>
#include <GlooxXMPPBridgeLibrary/ExtendedConnectionTCPClient.h>
#include <GlooxXMPPBridgeLibrary/PresenceHandler.h>
#include <Protocols/YMsgMock/BlockedContactManagementLink.h>
#include <Protocols/YMsgMock/ContactManagementLink.h>
#include <Protocols/YMsgMock/PresenceManagementLink.h>
#include <Protocols/YMsgMock/ProfileManagementLink.h>
#include <Protocols/YMsgMock/SessionContext.h>
#include <Protocols/YMsg/DispatchMaps.h>
#include <disco.h>
#include <rostermanager.h>
#include <iostream>


namespace Giblet { namespace Bridges { namespace Gloox
{

	void AuthenticationHandler::OnClientAuthenticated(Protocols::YMsg::ConnectionPump& pump, Protocols::YMsg::PacketDispatcher& dispatcher)
	{
		using namespace std::placeholders;

		//	connect to [XMPP] server
		const auto client = std::make_shared<ExtendedClient>(
			std::bind(&AuthenticationHandler::OnSessionAuthenticated, shared_from_this(), _1, _2),
			dispatcher.shared_from_this(),
			gloox::JID(clientId_ + "/giblet"),
			password_);
		client->initialize();
		client->disco()->setVersion( "GibletYIM", gloox::GLOOX_VERSION );
		client->disco()->setIdentity( "client", "bridge" );

		//j->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );
		
		auto connection(std::make_unique<ExtendedConnectionTCPClient>(client.get(), client->logInstance(), client->server(), client->port()));
		const auto connectionPtr(connection.get());
		client->setConnectionImpl(connection.release());
		if (!client->connect(false))
		{
			//	FIXME: Do something meaningful and bail
			throw std::runtime_error("Connection failed");
		}

		const auto socketHandle(connectionPtr->handle());
		if (socketHandle == INVALID_SOCKET)
		{
			throw std::runtime_error("Invalid socket handle");
		}

		pump.AttachSocket(connectionPtr->handle(), std::bind(&ExtendedClient::pump, client));
	}


	void AuthenticationHandler::Populate(
		Protocols::YMsg::ContactManager& contactManager,
		ExtendedClient& client)
	{
		const auto rosterManager(client.rosterManager());
		if (!rosterManager)
		{
			throw std::runtime_error("No roaster manager available");
		}

		auto roster(rosterManager->roster());
		if (!roster)
		{
			throw std::runtime_error("roster cannot be null");
		}

		for (const auto& itemPair : *roster)
		{
			const auto& item(*itemPair.second);
			const auto& groups(item.groups());
			if (groups.empty())
			{
				throw std::runtime_error("Don't know how to handle empty groups yet");
			}

			std::cerr << "Roster Item: " << item.jidJID().full() << '\n';
			for (const auto& group : groups)
			{
				using ContactInfo = Protocols::YMsg::ContactInfo;

				contactManager.LoadContact(ContactInfo(item.jidJID().full(), group, ContactInfo::Linked));
			}
		}
	}



	void AuthenticationHandler::OnSessionAuthenticated(
		Protocols::YMsg::PacketDispatcher& dispatcher,
		ExtendedClient& client)
	{
		using namespace Protocols::YMsg;

		const auto profileManager(std::make_shared<ProfileManager>());
		const auto profileManagementEvents(std::make_shared<ProfileManagementEvents>(connection_, profileManager));
		const auto profileManagement(std::make_shared<MockProfileManagementLink>(profileManager, profileManagementEvents));
		const auto blockedContactManager(std::make_shared<BlockedContactManager>());
		const auto blockedContactManagementEvents(std::make_shared<BlockedContactManagementEvents>(connection_, blockedContactManager));
		const auto blockedContactManagementLink(std::make_shared<MockBlockedContactManagementLink>(blockedContactManager, blockedContactManagementEvents));
		const auto contactManager(std::make_shared<ContactManager>());
		const auto contactManagementEvents(std::make_shared<ContactManagementEvents>(connection_, blockedContactManager, profileManager, contactManager));
		const auto contactManagement(std::make_shared<MockContactManagementLink>(blockedContactManager, profileManager, contactManager, contactManagementEvents));
		const auto presenceEvents(std::make_shared<PresenceEvents>(connection_, contactManager));
		const auto presenceManagementLink(std::make_shared<MockPresenceManagementLink>(profileManager, contactManager, presenceEvents, initialAvailability_));
		const auto session(std::make_shared<MockSessionContext>(connection_, blockedContactManager, blockedContactManagementLink, profileManager, profileManagement, contactManager, contactManagement, presenceManagementLink));

		profileManager->Load(clientId_, {});
		Populate(*contactManager, client);
		blockedContactManager->Load({ "wierdo", "jerk", "germinator" });

		client.setPresenceHandler(std::make_unique<PresenceHandler>(presenceEvents));

		//	Start the session
		connection_->SetSessionId(rand());
		session->BeginSession();

		////	FIXME: We will be destroyed during the call to SetDispatcher. Not ideal!
		dispatcher.SetDispatchMap(DispatchMaps::V11::Create(session));
	}

}}}
