//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/SessionContext.h>
#include <Protocols/YMsg/PacketDispatcher.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class V11PacketDispatcher : public PacketDispatcher<SessionContext>
	{
	public:

		explicit V11PacketDispatcher(std::shared_ptr<SessionContext> session);


	protected:

		template<class ProcessorType_>
		static void AddProcessor(container_type& processors)
		{
			using namespace std::placeholders;

			processors.emplace(make_pair(
				typename ProcessorType_::payload_type::ServiceId,
				bind(&ProcessorType_::Parse, ProcessorType_(), _1, _2, _3, _4)));
		}


	protected:

		const std::shared_ptr<SessionContext> session_;
	};

}}}
