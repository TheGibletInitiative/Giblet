//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <connectiontcpclient.h>


namespace Giblet { namespace Bridges { namespace Gloox
{

	class ExtendedConnectionTCPClient : public gloox::ConnectionTCPClient
	{
	public:

		using ConnectionTCPClient::ConnectionTCPClient;

		int handle() const;
	};

}}}
