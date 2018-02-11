//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/BlockedContactManager.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class MockBlockedContactManager : public BlockedContactManager
	{
	public:

		using BlockedContactManager::BlockedContactManager;

		void Add(string_view_type clientId, string_view_type contactId) override;
		void Remove(string_view_type clientId, string_view_type contactId) override;
	};

}}}
