//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Header.h>
#include <WinSock2.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	Header::Header(
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


	Header::Header(const rawheader_type& other)
		:
		protoVersion(ntohs(other.protoVersion)),
		vendorId(ntohs(other.vendorId)),
		payloadSize(ntohs(other.payloadSize)),
		serviceId(ntohs(other.serviceId)),
		attribute(ntohl(other.attribute)),
		sessionId(ntohl(other.sessionId))
	{}

	Header::operator rawheader_type() const
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


}}}

