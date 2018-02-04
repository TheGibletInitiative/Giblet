//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <vector>
#include <string>

extern "C"
{
	struct mg_connection;
}


void mg_send_data(mg_connection& conn, const std::string& data);
void mg_send_data(mg_connection& conn, const std::vector<char>& data);

namespace Giblet { namespace YahooWeb
{

	std::string GetFullURL(mg_connection& conn);

	bool GenericServeFile(mg_connection& conn, const std::string& filepath);
	bool GenericHandler(mg_connection& conn, const std::string& rootPath);

}}
