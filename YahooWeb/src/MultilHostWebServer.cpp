//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include <YahooWeb/MultilHostWebServer.h>
#include <YahooWeb/Utilities.h>
#include <mongoose.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>


namespace Giblet { namespace YahooWeb
{

	MultilHostWebServer::MultilHostWebServer(std::shared_ptr<session_gateway_type> sessionGateway)
		: WebServer(sessionGateway, std::string())
	{}


	bool MultilHostWebServer::Add(std::string hostname, handler_function_type handler)
	{
		return handlers_.insert_or_assign(move(hostname), move(handler)).second;
	}


	bool MultilHostWebServer::Add(std::string hostname, WebServer& handler)
	{
		return handlers_.insert_or_assign(
			move(hostname),
			bind(&WebServer::OnRequest, &handler, std::placeholders::_1)).second;
	}


	bool MultilHostWebServer::OnRequest(mg_connection& conn)
	{
		std::cerr << "Processing " << GetFullURL(conn) << "\n";

		const auto headerHost(mg_get_header(&conn, "Host"));
		auto handled = false;
		if (headerHost)
		{
			std::string host(headerHost);
			const auto pos(host.find(':'));
			if (pos != std::string::npos)
			{
				host.resize(pos);
			}

			auto handler = handlers_.find(host);
			if (handler != handlers_.end())
			{
				handled = handler->second(conn);
			}
		}

		if (!handled)
		{
			mg_send_data(conn, GetFullURL(conn));
		}

		return true;
	}


	void MultilHostWebServer::DispatchPacket(const Protocols::YMsg::Builder& builder)
	{
		sessionGateway_->DispatchPacket(builder);
	}

}}
