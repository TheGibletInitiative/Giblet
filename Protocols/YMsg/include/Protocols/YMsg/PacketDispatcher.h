//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>
#include <functional>
#include <map>
#include <iostream>


namespace Giblet { namespace Protocols { namespace YMsg
{

	template<class ContextType_>
	class PacketDispatcher
	{
	public:

		using context_type = ContextType_;
		using header_type = Header;
		using const_iterator = PacketParser::const_iterator;
		using function_type = std::function<void(context_type&, const header_type&, const_iterator, const_iterator)>;
		using container_type = std::map<uint16_t, function_type>;


	public:

		PacketDispatcher() = default;
		explicit PacketDispatcher(container_type processors)
			: processors_(processors)
		{}

		PacketDispatcher(const PacketDispatcher&) = delete;
		PacketDispatcher& operator=(const PacketDispatcher&) = delete;

		virtual void Dispatch(
			context_type& context,
			const header_type& header,
			const_iterator payloadBegin,
			const_iterator payloadEnd)
		{
			auto processor = processors_.find(header.serviceId);
			if (processor == processors_.end())
			{
				std::cerr << "Unable to find processor for service id " << header.serviceId << "  attribute " << header.attribute << "\n";
				Payload payload;
				PacketParser parser;
				parser.Parse(payload, header, payloadBegin, payloadEnd);
				return;
			}

			std::cerr << "Parsing payload for service id " << header.serviceId << "  attribute " << header.attribute << "\n";

			processor->second(context, header, payloadBegin, payloadEnd);
		}


	protected:

		container_type	processors_;
	};


}}}
