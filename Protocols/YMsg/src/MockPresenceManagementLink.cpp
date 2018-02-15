//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/MockPresenceManagementLink.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	MockPresenceManagementLink::MockPresenceManagementLink(
		std::shared_ptr<ProfileManager> profileManager,
		std::shared_ptr<ContactManager> contactManager,
		std::shared_ptr<PresenceEvents> presenceEvents)
		:
		profileManager_(profileManager),
		contactManager_(contactManager),
		presenceEvents_(presenceEvents)
	{
		if (!profileManager_)
		{
			throw std::invalid_argument("profileManager_ cannot be null");
		}

		if (!contactManager_)
		{
			throw std::invalid_argument("contactManager_ cannot be null");
		}

		if (!presenceEvents_)
		{
			throw std::invalid_argument("presenceEvents_ cannot be null");
		}
	}


	void MockPresenceManagementLink::BeginSession(availability_type initialAvailability)
	{
		properties_.BeginSession(initialAvailability);
	}
	
	
	void MockPresenceManagementLink::SetAvailable()
	{
		properties_.SetAvailable();

		for (const auto& profile : *profileManager_)
		{
			if (contactManager_->IsContact(profile))
			{
				presenceEvents_->OnContactAvailable(profile);
			}
		}
	}


	void MockPresenceManagementLink::SetIdle(string_view_type message, bool isBusy, string_view_type idleUnknown)
	{
		properties_.SetIdle(message, isBusy, idleUnknown);

		//	user online
		for (const auto& profile : *profileManager_)
		{
			if (contactManager_->IsContact(profile))
			{
				presenceEvents_->OnContactIdle(profile, idleUnknown, message, isBusy);
			}
		}
	}


	void MockPresenceManagementLink::SetAway(availability_type availability, bool isBusy)
	{
		properties_.SetAway(availability, isBusy);

		//	user online
		for (const auto& profile : *profileManager_)
		{
			if (contactManager_->IsContact(profile))
			{
				presenceEvents_->OnContactAway(profile, availability, isBusy);
			}
		}
	}


	void MockPresenceManagementLink::SetCustomStatus(string_view_type message, bool isBusy)
	{
		properties_.SetCustomStatus(message, isBusy);

		//	user online
		for (const auto& profile : *profileManager_)
		{
			if (contactManager_->IsContact(profile))
			{
				presenceEvents_->OnContactStatusMessage(profile, message, isBusy);
			}
		}
	}


	void MockPresenceManagementLink::SetInvisible()
	{
		properties_.SetInvisible();

		for (const auto& profile : *profileManager_)
		{
			if (contactManager_->IsContact(profile))
			{
				presenceEvents_->OnContactOffline(profile);
			}
		}
	}

}}}
