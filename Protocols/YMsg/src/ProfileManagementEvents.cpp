//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/ProfileManagementEvents.h>
#include <Protocols/YMsg/Server/Builders/ProfileActivated.h>
#include <Protocols/YMsg/Server/Builders/ProfileDeactivated.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	ProfileManagementEvents::ProfileManagementEvents(
		std::shared_ptr<ClientConnection> connection,
		std::shared_ptr<ProfileManager> profileManager)
		:
		connection_(connection),
		profileManager_(profileManager)
	{
		if (!connection_)
		{
			throw std::invalid_argument("connection cannot be null");
		}
		if (!profileManager_)
		{
			throw std::invalid_argument("profileManager_ cannot be null");
		}
	}


	void ProfileManagementEvents::OnAdded(string_view_type profileId)
	{
		//	TODO: Report error if found but continue just in case
		if (!profileManager_->IsClientOrProfileId(profileId))
		{
			//	FIXME
			//profileManager_->Add(profileId);
		}

		//	TODO: Tell messenger the profile id was added
	}


	void ProfileManagementEvents::OnActivated(string_view_type profileId)
	{
		//	Tell messenger the add request was sent
		Server::Builders::ProfileActivated builder;
		builder.Build(*connection_, profileId);
		builder.Send(*connection_);
	}


	void ProfileManagementEvents::OnDeactivated(string_view_type profileId)
	{
		//	Tell messenger the add request was sent
		Server::Builders::ProfileDeactivated builder;
		builder.Build(*connection_, profileId);
		builder.Send(*connection_);
	}

}}}
