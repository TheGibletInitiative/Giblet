//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/PresenceManager.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class PresenceManagementLink
	{
	public:

		using string_type = std::string;			//	should be a UTF8 string
		using string_view_type = std::string_view;	//	should be a UTF8 string
		using availability_type = detail::Availability;


	public:

		PresenceManagementLink() = default;
		PresenceManagementLink(const PresenceManagementLink&) = delete;
		virtual ~PresenceManagementLink() = default;

		virtual void BeginSession(availability_type initialAvailability) = 0;

		virtual void SetAvailable() = 0;
		virtual void SetIdle(string_view_type message, bool isBusy, string_view_type idleUnknown) = 0;
		virtual void SetAway(availability_type availability, bool isBusy) = 0;
		virtual void SetCustomStatus(string_view_type message, bool isBusy) = 0;
		virtual void SetInvisible() = 0;
	};

}}}
