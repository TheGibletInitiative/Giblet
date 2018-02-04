//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <YahooWeb/WebServer.h>


namespace Giblet { namespace YahooWeb
{

	class YahooInsiderWebServer : public WebServer
	{
	public:

		explicit YahooInsiderWebServer(std::shared_ptr<session_gateway_type> sessionGateway);

		bool OnRequest(mg_connection& conn) override;
	};

}}
