//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/ProfileManager.h>
#include <Protocols/YMsg/Server/Builders/ProfileActivated.h>
#include <Protocols/YMsg/Server/Builders/ProfileDeactivated.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	ProfileManager::ProfileManager(std::shared_ptr<ClientConnection> connection)
		: connection_(connection)
	{
		if (!connection_)
		{
			throw std::invalid_argument("connection cannot be null");
		}
	}

	
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


	void ProfileManager::OnAdded(string_view_type profileId)
	{
		//	TODO: Report error if found but continue just in case
		if (!IsClientOrProfileId(profileId))
		{
			profiles_.push_back(std::string(profileId));
		}

		//	TODO: Tell messenger the profile id was added
	}


	void ProfileManager::OnActivated(string_view_type profileId)
	{
		//	Tell messenger the add request was sent
		Server::Builders::ProfileActivated builder;
		builder.Build(*connection_, profileId);
		builder.Send(*connection_);
	}


	void ProfileManager::OnDeactivated(string_view_type profileId)
	{
		//	Tell messenger the add request was sent
		Server::Builders::ProfileDeactivated builder;
		builder.Build(*connection_, profileId);
		builder.Send(*connection_);
	}

}}}
