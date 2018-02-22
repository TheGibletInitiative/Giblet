//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Types.h>
#include <functional>
#include <map>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class ConnectionPump
	{
	public:

		using socket_type = uintptr_t;
		using ondatacallback_type = std::function<bool(ConnectionPump& pump, socket_type, size_t)>;

		virtual void AttachSocket(socket_type socket, ondatacallback_type onData);
		virtual void Pump();

	protected:

		std::map<socket_type, ondatacallback_type>	onDateCallbacks_;
	};


	class ClientConnection
	{
	public:

		using sessionid_type = uint32_t;
		using protocolversion_type = uint16_t;
		using socket_type = ConnectionPump::socket_type;


		explicit ClientConnection(socket_type sessionSocket);
		ClientConnection(const ClientConnection&) = delete;
		virtual ~ClientConnection() = default;


		virtual void SendToClient(const char *data, const size_t length);

		virtual sessionid_type GetSessionId() const;
		virtual protocolversion_type GetProtocolVersion() const;

		virtual void SetProtocolVersion(protocolversion_type version);
		virtual void SetSessionId(sessionid_type sessionId);


	protected:

		socket_type				socket_;
		sessionid_type			sessionId_;
		protocolversion_type	protocolVersion_;
	};

}}}
