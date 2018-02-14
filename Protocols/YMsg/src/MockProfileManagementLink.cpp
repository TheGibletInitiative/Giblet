//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/MockProfileManagementLink.h>
#include <Protocols/YMsg/Server/Builders/ProfileActivated.h>
#include <Protocols/YMsg/Server/Builders/ProfileDeactivated.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	MockProfileManagementLink::MockProfileManagementLink(
		std::shared_ptr<ProfileManager> profileManager,
		std::shared_ptr<ProfileManagementEvents> profileManagementEvents)
		:
		profileManager_(profileManager),
		profileManagementEvents_(profileManagementEvents)
	{
		if (!profileManager_)
		{
			throw std::invalid_argument("profileManager_ cannot be null");
		}

		if (!profileManagementEvents_)
		{
			throw std::invalid_argument("profile management events cannot be null");
		}
	}


	void MockProfileManagementLink::Add(string_view_type profileId)
	{
		if (profileManager_->IsClientOrProfileId(profileId))
		{
			throw std::invalid_argument("Profile id already exists");
		}

		profileManagementEvents_->OnAdded(profileId);
	}


	void MockProfileManagementLink::Activate(string_view_type profileId)
	{
		if (!profileManager_->IsClientOrProfileId(profileId))
		{
			throw std::invalid_argument("FIXME: Profile does not exist");
		}

		profileManagementEvents_->OnActivated(profileId);
	}


	void MockProfileManagementLink::Deactivate(string_view_type profileId)
	{
		if (!profileManager_->IsClientOrProfileId(profileId))
		{
			throw std::invalid_argument("FIXME: Profile does not exist");
		}

		profileManagementEvents_->OnDeactivated(profileId);
	}

}}}
