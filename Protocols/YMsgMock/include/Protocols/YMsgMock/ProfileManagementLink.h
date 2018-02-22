//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/ProfileManagementLink.h>
#include <Protocols/YMsg/ProfileManagementEvents.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class MockProfileManagementLink : public ProfileManagementLink
	{
	public:

		MockProfileManagementLink(
			std::shared_ptr<ProfileManager> profileManager,
			std::shared_ptr<ProfileManagementEvents> profileManagementEvents);


		void Add(string_view_type profileId) override;
		void Activate(string_view_type id) override;
		void Deactivate(string_view_type id) override;


	protected:

		const std::shared_ptr<ProfileManager>			profileManager_;
		const std::shared_ptr<ProfileManagementEvents>	profileManagementEvents_;
	};

}}}
