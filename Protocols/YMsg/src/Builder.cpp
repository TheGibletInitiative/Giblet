//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Builder.h>
#include <WinSock2.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	Builder::Builder(size_t minBufferReserve)
		: minBufferReserve_(minBufferReserve)
	{}


	void Builder::Initialize(const header_type& header)
	{
		buffer_.reserve(minBufferReserve_);
		buffer_.resize(sizeof(RawHeader));
		*reinterpret_cast<RawHeader*>(buffer_.data()) = header;
	}


	void Builder::Initialize(session_type& session, serviceid_type serviceId, attribute_type attributeId)
	{
		return Initialize(header_type(session.GetSessionId(), session.GetProtocolVersion(), serviceId, attributeId));
	}


	void Builder::Append(int key, const string_type& value)
	{
		Append(key, string_view_type(value));
	}


	void Builder::Append(int key, const char* value)
	{
		Append(key, string_view_type(value));
	}


	void Builder::Append(int key, const string_view_type& value)
	{
		static const std::initializer_list<buffer_type::value_type> terminator{
			static_cast<buffer_type::value_type>(0xc0),
			static_cast<buffer_type::value_type>(0x80)
		};


		const auto keyString(std::to_string(key));

		//	Calculate new payload buffer size and bail if too large
		if (buffer_.size() + keyString.size() + terminator.size() + terminator.size() >= PaxPayloadSize)
		{
			throw std::runtime_error("Payload size too large. Not handled!");
		}


		buffer_.insert(buffer_.end(), keyString.begin(), keyString.end());
		buffer_.insert(buffer_.end(), terminator.begin(), terminator.end());
		buffer_.insert(buffer_.end(), value.begin(), value.end());
		buffer_.insert(buffer_.end(), terminator.begin(), terminator.end());
	}


	void Builder::Send(session_type& session)
	{
		//	Validate buffer size does not exceed 64k
		if (buffer_.size() >= PaxPayloadSize)
		{
			throw std::runtime_error("Payload size too large. Not handled!");
		}

		reinterpret_cast<RawHeader*>(buffer_.data())->payloadSize = htons(buffer_.size() - sizeof(RawHeader));
		session.SendToClient(buffer_.data(), buffer_.size());
		
		buffer_ = buffer_type();
	}

}}}
