//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/SessionGateway.h>
#include <functional>
#include <map>

extern "C" { struct mg_connection; }

namespace Giblet { namespace YahooWeb
{

	class WebServer
	{
	public:

		//	FIXME: This needs to be interface to the back-end message broker.
		using session_gateway_type = Protocols::YMsg::ISessionGateway;

	public:

		WebServer(std::shared_ptr<session_gateway_type> sessionGateway, std::string rootPath);
		virtual ~WebServer() = default;

		virtual bool OnRequest(mg_connection& conn);

		static int ConnectionCallback(mg_connection* conn);


	protected:

		const std::shared_ptr<session_gateway_type>	sessionGateway_;
		std::string				rootPath_;
	};

}}
