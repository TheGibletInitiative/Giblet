//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Processor.h>
#include <Protocols/YMsg/Server/Payloads/SetAway.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors
{

	class SetAway : public PayloadProcessor<Payloads::SetAway>
	{
	protected:

		using availability_type = detail::Availability;

	public:

		virtual void Process(session_type& session, const header_type& header, payload_type& payload);
	};

}}}}}
