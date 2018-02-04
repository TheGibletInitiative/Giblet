//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include <YahooWeb/MultilHostWebServer.h>
#include <YahooWeb/MyYahooWebServer.h>
#include <YahooWeb/Utilities.h>
#include <mongoose.h>


namespace Giblet { namespace YahooWeb
{

	namespace
	{
		bool OnDebugSendPacketRequest(mg_connection& conn)
		{
			char buffer[1024];

			//	Grab the service id
			const auto serviceIdLength(mg_get_var(&conn, "serviceid", buffer, sizeof(buffer)));
			if (!serviceIdLength)
			{
				return false;
			}
			const auto serviceId(std::stoi(buffer, nullptr, 10));

			//	Grab the attribute
			const auto attributeIdLength(mg_get_var(&conn, "attribute", buffer, sizeof(buffer)));
			if (!attributeIdLength)
			{
				return false;
			}
			const auto attribute(std::stol(buffer, nullptr, 10));


			//	Build the packet
			using Builder = Protocols::YMsg::Builder;
			Builder builder;

			builder.Initialize(Builder::header_type(0, 0, serviceId, attribute));
			for (int i(0);; ++i)
			{
				const auto keyLength(mg_get_var(&conn, ("key" + std::to_string(i)).c_str(), buffer, sizeof(buffer)));
				if (keyLength < 0)
				{
					break;
				}

				if (keyLength)
				{
					const auto key(std::stol(buffer, nullptr, 10));
					const auto valueLength(mg_get_var(&conn, ("value" + std::to_string(i)).c_str(), buffer, sizeof(buffer)));

					builder.Append(key, buffer);
				}
			}

			static_cast<MultilHostWebServer*>(conn.server_param)->DispatchPacket(builder);

			return true;
		}
	}




	MyYahooWebServer::MyYahooWebServer(std::shared_ptr<session_gateway_type> sessionGateway, std::string rootPath)
		: WebServer(sessionGateway, move(rootPath))
	{}


	bool MyYahooWebServer::OnRequest(mg_connection& conn)
	{
		if (!conn.uri)
		{
			return false;
		}

		static const std::string channelTearoff("/tearoff/channel.html");

		if (conn.uri == channelTearoff)
		{
			char buffer[1024] = { 0 };
			auto value = mg_get_var(&conn, "action", buffer, sizeof(buffer));
			if (value >= 0)
			{
				const std::string action(buffer, buffer + value);
				if (action == "send")
				{
					return OnDebugSendPacketRequest(conn);
				}
			}
		}

		return GenericHandler(conn, rootPath_);
	}

}}
