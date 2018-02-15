//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Header.h>
#include <Protocols/YMsg/Parser.h>
#include <Protocols/YMsg/SessionContext.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	template<class ParserType_, class PayloadType_ = ParserType_>
	class PayloadProcessor
	{
	public:

		using parser_type = ParserType_;
		using header_type = typename parser_type::header_type;

		using payload_type = PayloadType_;
		using key_type = typename payload_type::key_type;
		using string_type = typename payload_type::string_type;
		using string_view_type = typename payload_type::string_view_type;
		using serviceid_type = typename payload_type::serviceid_type;
		using session_type = SessionContext;

		virtual ~PayloadProcessor() = default;
	};


	struct IPacketProcessor
	{
		using parser_type = PacketParser;
		using header_type = parser_type::header_type;
		using key_type = parser_type::key_type;
		using string_type = parser_type::string_type;
		using string_view_type = parser_type::string_view_type;
		using session_type = SessionContext;

		virtual ~IPacketProcessor() = default;

		virtual void Parse(
			session_type& session,
			const header_type& header,
			parser_type::const_iterator payloadBegin,
			parser_type::const_iterator payloadEnd) = 0;
	};


	class NullPacketParser : public IPacketProcessor
	{
	public:

		virtual void Parse(
			session_type& session,
			const header_type& header,
			parser_type::const_iterator payloadBegin,
			parser_type::const_iterator payloadEnd)
		{
			((void)session);
			((void)header);
			((void)payloadBegin);
			((void)payloadEnd);
		}
	};


	template<class PayloadHandlerType_>
	class PacketProcessor : public IPacketProcessor
	{
	public:

		using handler_type = PayloadHandlerType_;
		using payload_type = typename handler_type::payload_type;

		virtual void Parse(
			session_type& session,
			const header_type& header,
			parser_type::const_iterator payloadBegin,
			parser_type::const_iterator payloadEnd)
		{
			payload_type fields;

			parser_type parser;
			parser.Parse(fields, header, payloadBegin, payloadEnd);

			handler_type handler;
			handler.Process(session, header, fields);
		}
	};
}}}
