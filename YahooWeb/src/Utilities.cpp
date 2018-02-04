//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include <YahooWeb/Utilities.h>
#include <mongoose.h>
#include <fstream>
#include <vector>
#include <string>


void mg_send_data(mg_connection& conn, const std::string& data)
{
	mg_send_data(&conn, data.data(), data.size());
}


void mg_send_data(mg_connection& conn, const std::vector<char>& data)
{
	mg_send_data(&conn, data.data(), data.size());
}


namespace Giblet { namespace YahooWeb
{

	std::string GetFullURL(mg_connection& conn)
	{
		std::string fullURL;

		const auto host(mg_get_header(&conn, "Host"));
		if (host)
		{
			fullURL += host;
		}

		if (conn.uri)
		{
			fullURL += conn.uri;
		}

		if (conn.query_string)
		{
			fullURL += '?';
			fullURL += conn.query_string;
		}

		return fullURL;
	}


	bool GenericServeFile(mg_connection& conn, const std::string& filepath)
	{
		std::ifstream input(filepath, std::ifstream::binary);
		if (!input.is_open())
		{
			return false;
		}

		std::vector<char> reply{ std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>() };
		mg_send_data(&conn, reply.data(), reply.size());

		return true;
	}


	bool GenericHandler(mg_connection& conn, const std::string& rootPath)
	{
		if (!conn.uri)
		{
			return false;
		}

		return GenericServeFile(conn, rootPath + conn.uri);
	}

}}

