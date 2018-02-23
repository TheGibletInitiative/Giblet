//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/ProtocolStream.h>
#include <WinSock2.h>
#include <iostream>


namespace Giblet { namespace Protocols { namespace YMsg
{

	ProtocolStream::ProtocolStream(DispatcherFunction dispatcher)
		: dispatcher_(dispatcher)
	{
		if (!dispatcher_)
		{
			throw std::invalid_argument("dispatcher_ cannot be null");
		}
	}




	bool ProtocolStream::HasHeader() const
	{
		return data_.size() >= sizeof(RawHeader);
	}


	bool ProtocolStream::IsValidHeader() const
	{
		return HasHeader() && ntohl(GetRawHeader()->signature) == Header::Signature;
	}


	bool ProtocolStream::HasPacket()
	{
		auto header(GetRawHeader());

		return header && data_.size() >= ntohs(header->payloadSize) + sizeof(RawHeader);
	}


	const RawHeader* ProtocolStream::GetRawHeader() const
	{
		return HasHeader() ? reinterpret_cast<const RawHeader*>(data_.data()) : nullptr;
	}




	bool ProtocolStream::ProcessPendingData(ConnectionPump& pump, socket_type socket, size_t incomingSize)
	{
		const auto currentSize = data_.size();

		if (!incomingSize)
		{
			return false;
		}

		data_.resize(currentSize + incomingSize);
		if (recv(socket, &data_[currentSize], incomingSize, 0) == SOCKET_ERROR)
		{
			data_.resize(currentSize);
			std::cerr << "Unable to read " << incomingSize << " bytes from socket\n";
			return false;
		}

		if (HasPacket())
		{
			Consume(pump);
		}

		return true;
	}


	void ProtocolStream::Consume(ConnectionPump& pump)
	{
		if (!HasPacket())
		{
			throw std::runtime_error("Unable to consume data");
		}

		Header header(*reinterpret_cast<RawHeader*>(data_.data()));
		auto payloadBegin(data_.begin());
		advance(payloadBegin, sizeof(RawHeader));
		auto payloadEnd(payloadBegin);
		advance(payloadEnd, header.payloadSize);

		if (dispatcher_)
		{
			dispatcher_(pump, *this, header, payloadBegin, payloadEnd);
		}

		data_.erase(data_.begin(), payloadEnd);
	}

}}}
