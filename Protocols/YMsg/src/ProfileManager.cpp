//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/ProfileManager.h>
#include <Protocols/YMsg/Server/Builders/ProfileDeactivated.h>

#include <Protocols/YMsg/ProfileManagementEvents.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	void ProfileManager::Load(string_view_type clientId, container_type profiles)
	{
		clientId_ = clientId;
		profiles_ = move(profiles);
	}




	ProfileManager::const_iterator ProfileManager::begin() const
	{
		return profiles_.begin();
	}


	ProfileManager::const_iterator ProfileManager::end() const
	{
		return profiles_.end();
	}


	ProfileManager::string_view_type ProfileManager::GetClientId() const
	{
		return clientId_;
	}


	bool ProfileManager::IsProfileId(string_view_type id) const
	{
		return find(profiles_.begin(), profiles_.end(), id) != profiles_.end();
	}


	bool ProfileManager::IsClientOrProfileId(string_view_type id) const
	{
		return id == GetClientId() || IsProfileId(id);
	}





}}}
