//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/ClientConnection.h>
#include <WinSock2.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	ClientConnection::ClientConnection(socket_type sessionSocket)
		: socket_(sessionSocket)
	{}


	void ClientConnection::SendToClient(const char *data, const size_t length)
	{
		send(socket_, data, length, 0);
	}


	ClientConnection::sessionid_type ClientConnection::GetSessionId() const
	{
		return sessionId_;
	}

	ClientConnection::protocolversion_type ClientConnection::GetProtocolVersion() const
	{
		return protocolVersion_;
	}


	void ClientConnection::SetProtocolVersion(protocolversion_type version)
	{
		protocolVersion_ = version;
	}


	void ClientConnection::SetSessionId(sessionid_type sessionId)
	{
		sessionId_ = sessionId;
	}

}}}
