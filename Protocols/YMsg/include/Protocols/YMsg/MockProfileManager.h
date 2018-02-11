//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/ProfileManager.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class MockProfileManager : public ProfileManager
	{
	public:

		using ProfileManager::ProfileManager;

		void Add(string_view_type profileId) override;
		void Activate(string_view_type id) override;
		void Deactivate(string_view_type id) override;
	};

}}}
