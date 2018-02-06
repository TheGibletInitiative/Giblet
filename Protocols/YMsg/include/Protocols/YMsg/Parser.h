//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Header.h>
#include <Protocols/YMsg/Types.h>
#include <string_view>
#include <vector>


namespace Giblet { namespace Protocols { namespace YMsg
{

	//	TODO: Add a mapped payload parser that can do stuff automatically from a table
	struct Payload
	{
		using header_type = Header;
		using key_type = int;
		using string_type = std::string;
		using string_view_type = std::string_view;
		using serviceid_type = header_type::serviceid_type;

		virtual ~Payload() = default;

		//	TODO: This should probably be pure virtual wiht a null payload to deliver the functionality we need
		virtual bool OnKeyPair(key_type key, string_view_type value)
		{
			((void)key);
			((void)value);

			return false;
		}
	};


	class PacketParser
	{
	public:

		using payload_type = Payload;
		using header_type = payload_type::header_type;
		using key_type = payload_type::key_type;
		using string_type = payload_type::string_type;
		using string_view_type = payload_type::string_view_type;
		using buffer_type = std::vector<char>;
		using const_iterator = buffer_type::const_iterator;


	public:

		virtual ~PacketParser() = default;

		virtual void Parse(
			payload_type& payload,
			const header_type& header,
			const_iterator payloadBegin,
			const_iterator payloadEnd);


	protected:

		using position_type = std::pair<const_iterator, const_iterator>;

		static position_type FindTerminator(const_iterator begin, const_iterator end);
	};

}}}
