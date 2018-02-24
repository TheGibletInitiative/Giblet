//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>
#include <functional>
#include <map>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class ConnectionPump;
	class ProtocolStream;

	class PacketDispatcher : public std::enable_shared_from_this<PacketDispatcher>
	{
	public:

		using header_type = Header;
		using const_iterator = PacketParser::const_iterator;
		using function_type = std::function<void(ConnectionPump&, ProtocolStream&, PacketDispatcher&, const header_type&, const_iterator, const_iterator)>;
		using container_type = std::map<uint16_t, function_type>;


	public:

		PacketDispatcher() = default;
		explicit PacketDispatcher(container_type processors);
		PacketDispatcher(const PacketDispatcher&) = delete;
		PacketDispatcher& operator=(const PacketDispatcher&) = delete;

		virtual void SetDispatchMap(container_type dispatchMap);

		virtual void Dispatch(
			ConnectionPump& pump,
			ProtocolStream& stream,
			const header_type& header,
			const_iterator payloadBegin,
			const_iterator payloadEnd);


	protected:

		container_type	processors_;
	};

}}}
