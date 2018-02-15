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

	ProtocolStream::ProtocolStream(std::shared_ptr<dispatcher_type> dispatcher)
		: dispatcher_(dispatcher)
	{}




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




	bool ProtocolStream::Append(session_type& session)
	{
		const auto currentSize = data_.size();
		DWORD incomingSize = 0;
		if (SOCKET_ERROR == ioctlsocket(session.GetConnection().GetSocket(), FIONREAD, (DWORD*)&incomingSize))
		{
			std::cerr << "Unable to retrieve number of bytes available on socket\n";
			return false;
		}

		if (!incomingSize)
		{
			return false;
		}

		data_.resize(currentSize + incomingSize);
		if (recv(session.GetConnection().GetSocket(), &data_[currentSize], incomingSize, 0) == SOCKET_ERROR)
		{
			data_.resize(currentSize);
			std::cerr << "Unable to read " << incomingSize << " bytes from socket\n";
			return false;
		}

		if (HasPacket())
		{
			Consume(session);
		}

		return true;
	}


	void ProtocolStream::Consume(session_type& session)
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

		dispatcher_->Dispatch(session, header, payloadBegin, payloadEnd);

		data_.erase(data_.begin(), payloadEnd);
	}

}}}
