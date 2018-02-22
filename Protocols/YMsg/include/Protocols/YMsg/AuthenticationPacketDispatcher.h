//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/PacketDispatcher.h>
#include <Protocols/YMsg/ClientConnection.h>
#include <Protocols/YMsg/ProtocolStream.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class AuthenticationContext
	{
	public:
		using string_type = std::string;
		using availability_type = Giblet::Protocols::YMsg::detail::Availability;

		string_type			clientId_;
		availability_type	initialAvailability_ = availability_type::Offline;
	};


	class AuthenticationPacketDispatcher : public Giblet::Protocols::YMsg::PacketDispatcher<AuthenticationContext>
	{
	public:

		AuthenticationPacketDispatcher(
			std::shared_ptr<Giblet::Protocols::YMsg::ClientConnection> connection,
			std::weak_ptr<Giblet::Protocols::YMsg::ProtocolStream> protocolStream);


	protected:

		virtual void OnProtocolSync(
			context_type& context,
			const header_type& header,
			const_iterator payloadBegin,
			const_iterator payloadEnd);


		virtual void OnChallengeRequest(
			context_type& context,
			const header_type& header,
			const_iterator payloadBegin,
			const_iterator payloadEnd);

		virtual void OnAuthenticate(
			context_type& context,
			const header_type& header,
			const_iterator payloadBegin,
			const_iterator payloadEnd);

		virtual void OnClientAuthenticated(context_type& context);

		virtual void OnSessionAuthenticated(context_type& context) = 0;

	private:

		container_type CreateProcessorMap();


	protected:

		const std::shared_ptr<Giblet::Protocols::YMsg::ClientConnection> connection_;
		const std::weak_ptr<Giblet::Protocols::YMsg::ProtocolStream> protocolStream_;
	};

}}}
