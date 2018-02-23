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

	class AuthenticationHandler
	{
	public:

		using header_type = Header;
		using const_iterator = PacketDispatcher::const_iterator;
		using function_type = PacketDispatcher::function_type;
		using container_type = PacketDispatcher::container_type;
		using string_type = std::string;
		using availability_type = detail::Availability;


		AuthenticationHandler(std::shared_ptr<ClientConnection> connection);

		static container_type CreateDispatchMap(std::shared_ptr<AuthenticationHandler> handler);


	public:

		virtual void OnProtocolSync(
			PacketDispatcher& dispatcher,
			const header_type& header,
			const_iterator payloadBegin,
			const_iterator payloadEnd);


		virtual void OnChallengeRequest(
			PacketDispatcher& dispatcher,
			const header_type& header,
			const_iterator payloadBegin,
			const_iterator payloadEnd);

		virtual void OnAuthenticate(
			PacketDispatcher& dispatcher,
			const header_type& header,
			const_iterator payloadBegin,
			const_iterator payloadEnd);

		virtual void OnClientAuthenticated(PacketDispatcher& dispatcher);

		virtual void OnSessionAuthenticated(PacketDispatcher& dispatcher) = 0;


	protected:

		const std::shared_ptr<ClientConnection> connection_;
		string_type			clientId_;
		availability_type	initialAvailability_ = availability_type::Offline;

	};

}}}
