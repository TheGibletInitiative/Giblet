//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/AuthenticationHandler.h>
#include <Protocols/YMsg/Server/Builders/ProtocolAck.h>
#include <Protocols/YMsg/Server/Builders/ChallengeResponse.h>
#include <Protocols/YMsg/Server/Payloads/Authentication.h>
#include <Protocols/YMsg/Server/Payloads/ProtocolSync.h>
#include <Protocols/YMsg/Server/Payloads/ChallengeRequest.h>
#include <Protocols/YMsg/SharedPtrAsRefArg.h>
#include <iostream>


namespace Giblet { namespace Protocols { namespace YMsg
{

	AuthenticationHandler::AuthenticationHandler(std::shared_ptr<ClientConnection> connection)
		: connection_(connection)
	{}


	void AuthenticationHandler::OnProtocolSync(
		const header_type& header,
		const_iterator payloadBegin,
		const_iterator payloadEnd)
	{
		Server::Payloads::ProtocolSync payload;
		PacketParser parser;
		parser.Parse(payload, header, payloadBegin, payloadEnd);

		connection_->SetProtocolVersion(header.protoVersion);

		//	Acknowledge the sync
		Server::Builders::ProtocolAck builder;
		builder.Build(*connection_);
		builder.Send(*connection_);
	}


	void AuthenticationHandler::OnChallengeRequest(
		const header_type& header,
		const_iterator payloadBegin,
		const_iterator payloadEnd)
	{
		Server::Payloads::ChallengeRequest payload;
		PacketParser parser;
		parser.Parse(payload, header, payloadBegin, payloadEnd);

		clientId_ = payload.clientId;
		//	We don't use any authentication at the moment so we just use method 0
		//	which causes messenger to omit credentials
		Server::Builders::ChallengeResponse builder;
		builder.Build(*connection_, payload.clientId);
		builder.Send(*connection_);
	}


	void AuthenticationHandler::OnAuthenticate(
		ConnectionPump& pump,
		PacketDispatcher& dispatcher,
		const header_type& header,
		const_iterator payloadBegin,
		const_iterator payloadEnd)
	{
		Server::Payloads::Authentication payload;
		PacketParser parser;
		parser.Parse(payload, header, payloadBegin, payloadEnd);

		if (payload.clientId1_ != clientId_)
		{
			//	FIXME: send error and disconnect
			//	connection_->close();
		}

		//	FIXME: Need to define attributes the same way we do keys
		initialAvailability_ =
			header.attribute == static_cast<std::underlying_type<availability_type>::type>(availability_type::Invisible)
			? availability_type::Offline
			: availability_type::Available;

		password_ = "password";
		OnClientAuthenticated(pump, dispatcher);
	}




	AuthenticationHandler::container_type AuthenticationHandler::CreateDispatchMap(std::shared_ptr<AuthenticationHandler> handler)
	{
		using namespace std::placeholders;
		
		return
		{
			{Server::Payloads::ProtocolSync::ServiceId, bind(&AuthenticationHandler::OnProtocolSync, handler, _4, _5, _6)},
			{Server::Payloads::ChallengeRequest::ServiceId, bind(&AuthenticationHandler::OnChallengeRequest, handler, _4, _5, _6)},
			{Server::Payloads::Authentication::ServiceId, bind(&AuthenticationHandler::OnAuthenticate, handler, _1, _3, _4, _5, _6)},
		};
	}



}}}
