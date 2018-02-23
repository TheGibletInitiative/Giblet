//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/PacketDispatcher.h>
#include <iostream>


namespace Giblet { namespace Protocols { namespace YMsg
{

	PacketDispatcher::PacketDispatcher(container_type processors)
		: processors_(processors)
	{}


	void PacketDispatcher::SetDispatchMap(container_type dispatchMap)
	{
		processors_ = move(dispatchMap);
	}


	void PacketDispatcher::Dispatch(
		ConnectionPump& pump,
		ProtocolStream& stream,
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

		processor->second(pump, stream, *this, header, payloadBegin, payloadEnd);
	}

}}}
