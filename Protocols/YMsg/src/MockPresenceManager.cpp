//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/MockPresenceManager.h>


namespace Giblet { namespace Protocols { namespace YMsg
{
	MockPresenceManager::MockPresenceManager(
		std::shared_ptr<ClientConnection> connection,
		std::shared_ptr<ProfileManager> profileManager,
		std::shared_ptr<ContactManager>	contactManager)
		:
		PresenceManager(connection),
		profileManager_(profileManager),
		contactManager_(contactManager)
	{}


	void MockPresenceManager::SetAvailable()
	{
		PresenceManager::SetAvailable();

		for (const auto& profile : *profileManager_)
		{
			if (contactManager_->IsContact(profile))
			{
				contactManager_->OnContactAvailable(profile);
			}
		}
	}


	void MockPresenceManager::SetIdle(string_view_type message, bool isBusy, string_view_type idleUnknown)
	{
		PresenceManager::SetIdle(message, isBusy, idleUnknown);

		//	user online
		for (const auto& profile : *profileManager_)
		{
			if (contactManager_->IsContact(profile))
			{
				contactManager_->OnContactIdle(profile, idleUnknown, message, isBusy);
			}
		}
	}


	void MockPresenceManager::SetAway(availability_type availability, bool isBusy)
	{
		PresenceManager::SetAway(availability, isBusy);

		//	user online
		for (const auto& profile : *profileManager_)
		{
			if (contactManager_->IsContact(profile))
			{
				contactManager_->OnContactAway(profile, availability, isBusy);
			}
		}
	}


	void MockPresenceManager::SetCustomStatus(string_view_type message, bool isBusy)
	{
		PresenceManager::SetCustomStatus(message, isBusy);

		//	user online
		for (const auto& profile : *profileManager_)
		{
			if (contactManager_->IsContact(profile))
			{
				contactManager_->OnContactStatusMessage(profile, message, isBusy);
			}
		}
	}


	void MockPresenceManager::SetInvisible()
	{
		PresenceManager::SetInvisible();

		for (const auto& profile : *profileManager_)
		{
			if (contactManager_->IsContact(profile))
			{
				contactManager_->OnContactOffline(profile);
			}
		}
	}

}}}
