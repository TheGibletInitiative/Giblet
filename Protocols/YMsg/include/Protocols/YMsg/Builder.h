//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Header.h>
#include <Protocols/YMsg/SessionContext.h>
#include <Protocols/YMsg/Types.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class Builder
	{
	public:

		using buffer_type = std::vector<char>;
		using header_type = Header;
		using serviceid_type = header_type::serviceid_type;
		using attribute_type = header_type::attribute_type;
		using string_type = std::string;			//	should be a UTF8 string
		using string_view_type = std::string_view;	//	should be a UTF8 string
		using key_type = int;
		using connection_type = ClientConnection;

		static const size_t DefaultMinBufferReserver = 512;
		static const size_t PaxPayloadSize = 0x10000;


	public:

		explicit Builder(size_t minBufferReserve = DefaultMinBufferReserver);

		virtual ~Builder() = default;

		virtual void Initialize(const header_type& header);
		virtual void Initialize(connection_type& connection, serviceid_type serviceId, attribute_type attributeId);
		virtual void Append(int key, const string_type& value);
		virtual void Append(int key, const string_view_type& value);
		virtual void Append(int key, const char* value);

		template<class Type_>
		typename std::enable_if<std::is_integral<Type_>::value, void>::type Append(int key, const Type_& value)
		{
			Append(key, string_view_type(std::to_string(value)));
		}

		template<class Type_>
		typename std::enable_if<std::is_enum<Type_>::value, void>::type Append(int key, const Type_& value)
		{
			Append(key, static_cast<std::underlying_type<Type_>::type>(value));
		}

		virtual void Send(connection_type& connection);


	private:

		size_t		minBufferReserve_;
		buffer_type	buffer_;
	};

}}}
