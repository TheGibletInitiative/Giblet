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
		virtual ~ClientConnection() = default;


		virtual SOCKET GetSocket() const;	//	TODO: Abstract SOCKET - switch to Boost ASIO in the near future
		virtual void SendToClient(const char *data, const size_t length);

		virtual sessionid_type GetSessionId() const;
		virtual protocolversion_type GetProtocolVersion() const;

		virtual void OnProtocolSync(protocolversion_type version);
		virtual void BeginSession(sessionid_type id);


	protected:

		SOCKET					socket_;
		sessionid_type			sessionId_;
		protocolversion_type	protocolVersion_;
	};

}}}
