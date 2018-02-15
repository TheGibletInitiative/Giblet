//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>
#include <Protocols/YMsg/Processor.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	struct IPacketDispatcher
	{
		using buffer_type = PacketParser::buffer_type;
		using session_type = SessionContext;

		virtual ~IPacketDispatcher() = default;

		virtual void Dispatch(
			session_type& session,
			const Header& header,
			buffer_type::const_iterator payloadBegin,
			buffer_type::const_iterator payloadEnd) = 0;
	};


	//	FIXME: Rename to ProtocolBuffer or something
	class ProtocolStream
	{
	public:

		using buffer_type = std::vector<char>;
		using dispatcher_type = IPacketDispatcher;
		using session_type = dispatcher_type::session_type;


	public:

		explicit ProtocolStream(std::shared_ptr<dispatcher_type> dispatcher);

		virtual ~ProtocolStream() = default;

		virtual bool Append(session_type& session);


	protected:

		virtual void Consume(session_type& session);

		bool HasHeader() const;
		bool IsValidHeader() const;
		bool HasPacket();

		const RawHeader* GetRawHeader() const;


	protected:

		std::shared_ptr<dispatcher_type>	dispatcher_;
		buffer_type							data_;
	};

}}}

