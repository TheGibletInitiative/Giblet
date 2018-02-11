//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Types.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class ClientConnection
	{
	public:

		using sessionid_type = uint32_t;
		using protocolversion_type = uint16_t;


		explicit ClientConnection(SOCKET sessionSocket);

		ClientConnection(const ClientConnection&) = delete;


		SOCKET GetSocket() const;	//	TODO: Abstract SOCKET - switch to Boost ASIO in the near future
		void SendToClient(const char *data, const size_t length);

		sessionid_type GetSessionId() const;
		protocolversion_type GetProtocolVersion() const;

		void OnProtocolSync(protocolversion_type version);
		void BeginSession(sessionid_type id);


	protected:

		SOCKET					socket_;
		sessionid_type			sessionId_;
		protocolversion_type	protocolVersion_;
	};

}}}
