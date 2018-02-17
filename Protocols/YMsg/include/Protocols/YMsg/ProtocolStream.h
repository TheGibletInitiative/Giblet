//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>
#include <Protocols/YMsg/Processor.h>
#include <functional>


namespace Giblet { namespace Protocols { namespace YMsg
{

	//	FIXME: Rename to ProtocolBuffer or something
	class ProtocolStream
	{
	public:

		using buffer_type = std::vector<char>;
		using const_iterator_type = buffer_type::const_iterator;
		using connection_type = ClientConnection;
		using DispatcherFunction = std::function<void(const Header&, const_iterator_type, const_iterator_type)>;


	public:

		explicit ProtocolStream(std::shared_ptr<connection_type> connection);
		ProtocolStream(std::shared_ptr<connection_type> connection, DispatcherFunction dispatcher);
		ProtocolStream(const ProtocolStream&) = delete;
		virtual ~ProtocolStream() = default;

		ProtocolStream& operator=(const ProtocolStream&) = delete;


		virtual void SetDispatcher(DispatcherFunction dispatcher);

		virtual bool ProcessPendingData();


	protected:

		virtual void Consume();

		bool HasHeader() const;
		bool IsValidHeader() const;
		bool HasPacket();

		const RawHeader* GetRawHeader() const;


	protected:

		const std::shared_ptr<connection_type>	connection_;
		buffer_type								data_;
		DispatcherFunction						dispatcher_;
	};

}}}

