//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <YahooWeb/WebServer.h>


namespace Giblet { namespace YahooWeb
{

	class MultilHostWebServer : public WebServer
	{
	public:

		using handler_signature_type = bool(mg_connection&);
		using handler_function_type = std::function<handler_signature_type>;
		using session_gateway_type = Protocols::YMsg::ISessionGateway;

	public:

		explicit MultilHostWebServer(std::shared_ptr<session_gateway_type> sessionGateway);

		virtual bool Add(std::string hostname, handler_function_type handler);
		//	FIXME: handler should be type std::shared_ptr<WebServer>
		virtual bool Add(std::string hostname, WebServer& handler);

		virtual void DispatchPacket(const Protocols::YMsg::Builder& builder);

		bool OnRequest(mg_connection& conn) override;


	protected:

		using handler_container_type = std::map<std::string, handler_function_type>;

		handler_container_type	handlers_;
	};

}}
