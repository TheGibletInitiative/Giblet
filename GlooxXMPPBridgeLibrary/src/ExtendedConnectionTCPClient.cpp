//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <GlooxXMPPBridgeLibrary/ExtendedConnectionTCPClient.h>


namespace Giblet { namespace Bridges { namespace Gloox
{

	int ExtendedConnectionTCPClient::handle() const
	{
		return m_socket;
	}

}}}
