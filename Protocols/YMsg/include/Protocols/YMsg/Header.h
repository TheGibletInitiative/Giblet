//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <cstdint>
#include <WinSock2.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

#pragma pack(push, 1)
	struct RawHeader
	{
		static const uint32_t Signature = 0x594d5347;	//	'YMSG'

		using version_type = uint16_t;
		using vendorid_type = uint16_t;
		using payloadsize_type = uint16_t;
		using serviceid_type = uint16_t;
		using attribute_type = uint32_t;
		using sessionid_type = uint32_t;

		uint32_t			signature;		//	Packet marker/signature (YMSG)
		version_type		protoVersion;	//	Protocol version
		vendorid_type		vendorId;		//	Vendor ID
		payloadsize_type	payloadSize;	//	total length of the packet - length of the header (20 bytes) 
		serviceid_type		serviceId;		//	The packet command/service id
		attribute_type		attribute;		//	The packet attribute
		sessionid_type		sessionId;		//	The session ID
	};
#pragma pack(pop)


	struct Header
	{
		using rawheader_type = RawHeader;
		using version_type = rawheader_type::version_type;
		using vendorid_type = rawheader_type::vendorid_type;
		using payloadsize_type = rawheader_type::payloadsize_type;
		using sessionid_type = rawheader_type::sessionid_type;
		using serviceid_type = rawheader_type::serviceid_type;
		using attribute_type = rawheader_type::attribute_type;

		static const uint32_t Signature = rawheader_type::Signature;


		Header(
			sessionid_type sessionId,
			version_type protoVersion,
			serviceid_type serviceId,
			attribute_type attribute)
			:
			protoVersion(protoVersion),
			vendorId(0),
			payloadSize(0),
			serviceId(serviceId),
			attribute(attribute),
			sessionId(sessionId)
		{}

		Header(const Header&) = default;
		Header(const rawheader_type& other)
			:
			protoVersion(ntohs(other.protoVersion)),
			vendorId(ntohs(other.vendorId)),
			payloadSize(ntohs(other.payloadSize)),
			serviceId(ntohs(other.serviceId)),
			attribute(ntohl(other.attribute)),
			sessionId(ntohl(other.sessionId))
		{}

		operator rawheader_type() const
		{
			rawheader_type header;

			header.signature = htonl(rawheader_type::Signature);
			header.protoVersion = htons(protoVersion);
			header.vendorId = htons(vendorId);
			header.payloadSize = htons(payloadSize);
			header.serviceId = htons(serviceId);
			header.attribute = htonl(attribute);
			header.sessionId = htonl(sessionId);

			return header;
		}

		version_type		protoVersion;	//	Protocol version
		vendorid_type		vendorId;		//	Vendor ID
		payloadsize_type	payloadSize;	//	total length of the packet - length of the header (20 bytes) 
		serviceid_type		serviceId;		//	The packet command/service id
		attribute_type		attribute;		//	The packet attribute
		sessionid_type		sessionId;		//	The session ID
	};

}}}

