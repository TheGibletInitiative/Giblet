//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/PresenceManager.h>
#include <Protocols/YMsg/ContactManager.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class MockPresenceManager : public PresenceManager
	{
	public:

		using PresenceManager::PresenceManager;

		MockPresenceManager(
			std::shared_ptr<ClientConnection> connection,
			std::shared_ptr<ProfileManager> profileManager,
			std::shared_ptr<ContactManager>	contactManager);


		void SetAvailable() override;
		void SetIdle(string_view_type message, bool isBusy, string_view_type idleUnknown) override;
		void SetAway(availability_type availability, bool isBusy) override;
		void SetCustomStatus(string_view_type message, bool isBusy) override;
		void SetInvisible() override;

	protected:

		const std::shared_ptr<ProfileManager>	profileManager_;
		const std::shared_ptr<ContactManager>	contactManager_;
	};

}}}
