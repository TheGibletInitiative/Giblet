//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/MockProfileManager.h>
#include <Protocols/YMsg/Server/Builders/ProfileActivated.h>
#include <Protocols/YMsg/Server/Builders/ProfileDeactivated.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	void MockProfileManager::Add(string_view_type profileId)
	{
		if (IsClientOrProfileId(profileId))
		{
			throw std::invalid_argument("Profile id already exists");
		}

		OnAdded(profileId);
	}


	void MockProfileManager::Activate(string_view_type profileId)
	{
		if (!IsClientOrProfileId(profileId))
		{
			throw std::invalid_argument("FIXME: Profile does not exist");
		}

		OnActivated(profileId);
	}


	void MockProfileManager::Deactivate(string_view_type profileId)
	{
		if (!IsClientOrProfileId(profileId))
		{
			throw std::invalid_argument("FIXME: Profile does not exist");
		}

		OnDeactivated(profileId);
	}

}}}
