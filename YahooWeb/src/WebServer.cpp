//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include <YahooWeb/WebServer.h>
#include <YahooWeb/Utilities.h>
#include <mongoose.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>


namespace Giblet { namespace YahooWeb
{

	WebServer::WebServer(std::shared_ptr<session_gateway_type> sessionGateway, std::string rootPath)
		:
		sessionGateway_(sessionGateway),
		rootPath_(rootPath)
	{
		if (!sessionGateway_)
		{
			throw std::invalid_argument("sessionGateway cannot be null");
		}
	}



	int WebServer::ConnectionCallback(mg_connection* conn)
	{
		if (!conn)
		{
			return 0;
		}

		return static_cast<WebServer*>(conn->server_param)->OnRequest(*conn);
	}


	bool WebServer::OnRequest(mg_connection& conn)
	{
		std::cerr << "Loading " << GetFullURL(conn) << "\n";

		mg_send_data(conn, GetFullURL(conn));
		return true;
	}

}}
