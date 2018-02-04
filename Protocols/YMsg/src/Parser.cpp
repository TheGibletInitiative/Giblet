//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Parser.h>
#include <iostream>
#include <string>

namespace Giblet { namespace Protocols { namespace YMsg
{

	namespace
	{
		//	FIXME - we really need to address the overflow in the cast!
		static const PacketParser::const_iterator::value_type TerminatorByte1 = static_cast<PacketParser::buffer_type::value_type>(0xc0);
		static const PacketParser::const_iterator::value_type TerminatorByte2 = static_cast<PacketParser::buffer_type::value_type>(0x80);
	}




	PacketParser::position_type PacketParser::FindTerminator(const_iterator begin, const_iterator end)
	{
		while (begin != end)
		{
			if (*begin == TerminatorByte1)
			{
				auto current = begin;
				++current;

				if (current == end)
				{
					return position_type(begin, current);
				}

				if (*current == TerminatorByte2)
				{
					return position_type(begin, ++current);
				}
			}
			++begin;
		}

		return position_type(begin, begin);
	}




	void PacketParser::Parse(
		payload_type& payload,
		const header_type& header,	//	FIXME: This doesn't get use din this context and doesn't seem to be necessary (at least at this point)
		const_iterator payloadBegin,
		const_iterator payloadEnd)
	{
		((void)header);

		for (auto start(payloadBegin); start != payloadEnd;)
		{
			//	It's possible to have a null terminator that indicates the end of the payload with trailing garbage.
			if (!*start)
			{
				break;
			}

			auto position = FindTerminator(start, payloadEnd);
			auto end = position.first;
			auto next = position.second;
			if (start == end)
			{
				throw std::runtime_error("Empty key value. Dunno how to handle this!");
			}
			auto key = std::strtol(std::string(start, end).c_str(), nullptr, 10);


			start = next;
			position = FindTerminator(start, payloadEnd);
			end = position.first;
			next = position.second;

			auto value(start == end ? string_view_type() : string_view_type(&(*start), distance(start, end)));
			
			const auto result(payload.OnKeyPair(key, value));
			std::cerr << (std::string(result ? "  " : "!!") + "[" + std::to_string(key) + "]  ==>  " + std::string(value) + "\n");

			start = next;
		}
	}

}}}
