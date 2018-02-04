//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include <YahooWeb/YahooInsiderWebServer.h>
#include <YahooWeb/Utilities.h>
#include <mongoose.h>
#include <iostream>


namespace Giblet { namespace YahooWeb
{

	YahooInsiderWebServer::YahooInsiderWebServer(std::shared_ptr<session_gateway_type> sessionGateway)
		: WebServer(sessionGateway, std::string())
	{}


	bool YahooInsiderWebServer::OnRequest(mg_connection& conn)
	{
		static const std::string chatInfoPage("/ycontent/");

		if (!conn.uri || conn.uri != chatInfoPage)
		{
			return false;
		}

		const std::string& fullURL(GetFullURL(conn));
		std::cerr << ("Loading " + fullURL + '\n');
		//	TODO: Handle IMV downloads (messenger doesn't accept them at the moment)
		auto getImvPos(fullURL.find("?getimv="));
		if(getImvPos != fullURL.npos)
		{
			getImvPos += 8;
			auto getImvEndPos(fullURL.find('&', getImvPos));
			if (getImvEndPos == fullURL.npos)
			{
				getImvEndPos = fullURL.size();
			}

			const auto name(fullURL.substr(getImvPos, getImvEndPos - getImvPos));
			return GenericServeFile(
				conn,
				"htdoc/insider.yahoo.com/imv/" + name + ".yim");
		}

		//	Deliver the chat room topic list.		
		if(fullURL.find("?chatroom_") != fullURL.npos)
		{
			//	FIXME: As with above this has dynamic content that needs to be generated
			if (fullURL.find("chatroom_") != std::string::npos)
			{
				return GenericServeFile(conn, "htdoc/insider.yahoo.com/roomTopics.xml");
			}

			return false;
		}

		//	There can can be multiple content sources (chat, audibles, filters) in the request
		//	The response needs to be build from all of those sources based on the request
		mg_send_data(conn, "<?xml version=\"1.0\" encoding=\"utf-8\" ?>");
		mg_send_data(conn, "<yahooinsider intl=\"us\">");


		//	Assume retrieving configuration files
		//	&filter=0&imv=0&system=0&sms=0&chatcat=0&ab2=0
		if (fullURL.find("&filter=0") != fullURL.npos)
		{
			GenericServeFile(conn, "htdoc/insider.yahoo.com/filters.xml");
		}
		if (fullURL.find("&imv=0") != fullURL.npos)
		{
			GenericServeFile(conn, "htdoc/insider.yahoo.com/imvironments.xml");
		}
		if (fullURL.find("&system=0") != fullURL.npos)
		{
			GenericServeFile(conn, "htdoc/insider.yahoo.com/system.xml");
		}
		if (fullURL.find("&sms=0") != fullURL.npos)
		{
			GenericServeFile(conn, "htdoc/insider.yahoo.com/sms.xml");
		}
		if (fullURL.find("&chatcat=") != std::string::npos)
		{
			GenericServeFile(conn, "htdoc/insider.yahoo.com/chatCategories.xml");
		}
		if (fullURL.find("&ab2=0") != fullURL.npos)
		{
			GenericServeFile(conn, "htdoc/insider.yahoo.com/addressbook.xml");
		}

		mg_send_data(conn, "</yahooinsider>");

		return true;
	}

}}
