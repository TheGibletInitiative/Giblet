//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/AuthenticationPacketDispatcher.h>
#include <Protocols/YMsg/Server/Builders/ProtocolAck.h>
#include <Protocols/YMsg/Server/Builders/ChallengeResponse.h>
#include <Protocols/YMsg/Server/Payloads/Authentication.h>
#include <Protocols/YMsg/Server/Payloads/ProtocolSync.h>
#include <Protocols/YMsg/Server/Payloads/ChallengeRequest.h>
#include <iostream>


namespace Giblet { namespace Protocols { namespace YMsg
{

	AuthenticationPacketDispatcher::AuthenticationPacketDispatcher(
		std::shared_ptr<Giblet::Protocols::YMsg::ClientConnection> connection,
		std::weak_ptr<Giblet::Protocols::YMsg::ProtocolStream> protocolStream)
		:
		PacketDispatcher(CreateProcessorMap()),
		connection_(connection),
		protocolStream_(protocolStream)
	{}



	void AuthenticationPacketDispatcher::OnProtocolSync(
		context_type& context,
		const header_type& header,
		const_iterator payloadBegin,
		const_iterator payloadEnd)
	{
		Server::Payloads::ProtocolSync payload;
		PacketParser parser;
		parser.Parse(payload, header, payloadBegin, payloadEnd);

		connection_->SetProtocolVersion(header.protoVersion);

		//	Acknowledge the sync
		Giblet::Protocols::YMsg::Server::Builders::ProtocolAck builder;
		builder.Build(*connection_);
		builder.Send(*connection_);
	}


	void AuthenticationPacketDispatcher::OnChallengeRequest(
		context_type& context,
		const header_type& header,
		const_iterator payloadBegin,
		const_iterator payloadEnd)
	{
		Giblet::Protocols::YMsg::Server::Payloads::ChallengeRequest payload;
		Giblet::Protocols::YMsg::PacketParser parser;
		parser.Parse(payload, header, payloadBegin, payloadEnd);

		context.clientId_ = payload.clientId;
		//	We don't use any authentication at the moment so we just use method 0
		//	which causes messenger to omit credentials
		Giblet::Protocols::YMsg::Server::Builders::ChallengeResponse builder;
		builder.Build(*connection_, payload.clientId);
		builder.Send(*connection_);
	}


	void AuthenticationPacketDispatcher::OnAuthenticate(
		context_type& context,
		const header_type& header,
		const_iterator payloadBegin,
		const_iterator payloadEnd)
	{
		Giblet::Protocols::YMsg::Server::Payloads::Authentication payload;
		Giblet::Protocols::YMsg::PacketParser parser;
		parser.Parse(payload, header, payloadBegin, payloadEnd);

		if (payload.clientId1_ != context.clientId_)
		{
			//	FIXME: send error and disconnect
			//	connection_->close();
		}

		using availability_type = context_type::availability_type;

		//	FIXME: Need to define attributes the same way we do keys
		context.initialAvailability_ =
			header.attribute == static_cast<std::underlying_type<availability_type>::type>(availability_type::Invisible) 
			? availability_type::Offline
			: availability_type::Available;

		OnClientAuthenticated(context);
	}


	void AuthenticationPacketDispatcher::OnClientAuthenticated(context_type& context)
	{
		//	Get rid of the processors so no incoming packets from the client are processed
		//	until authentication has completed.
		processors_.clear();

		//	connect to [XMPP] server

		//	authenticate with server

		//	retrieve profile

		//	retrieve roster/buddy list

		//	Everything complete
		OnSessionAuthenticated(context);
	}


	AuthenticationPacketDispatcher::container_type AuthenticationPacketDispatcher::CreateProcessorMap()
	{
		using namespace Giblet::Protocols::YMsg::Server;
		using namespace std::placeholders;

		return
		{
			{Payloads::ProtocolSync::ServiceId, bind(&AuthenticationPacketDispatcher::OnProtocolSync, this, _1, _2, _3, _4)},
			{Payloads::ChallengeRequest::ServiceId, bind(&AuthenticationPacketDispatcher::OnChallengeRequest, this, _1, _2, _3, _4)},
			{Payloads::Authentication::ServiceId, bind(&AuthenticationPacketDispatcher::OnAuthenticate, this, _1, _2, _3, _4)},
		};
		;
	}


}}}
