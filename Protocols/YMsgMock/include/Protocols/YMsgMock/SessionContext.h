//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/SessionContext.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class MockSessionContext : public SessionContext
	{
	public:

		using SessionContext::SessionContext;


		virtual void BeginSession() override;
	};

}}}
