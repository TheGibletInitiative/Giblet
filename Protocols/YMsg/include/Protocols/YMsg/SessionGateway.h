//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	struct ISessionGateway
	{
		virtual ~ISessionGateway() = default;

		virtual void DispatchPacket(const Builder& builder) = 0;
	};


	class SessionGateway : public ISessionGateway
	{
	public:

		void DispatchPacket(const Builder& builder) override;
	};

}}}
