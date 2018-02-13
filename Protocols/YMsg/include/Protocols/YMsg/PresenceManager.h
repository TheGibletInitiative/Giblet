//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Types.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class PresenceProperties
	{
	public:

		using string_type = std::string;			//	should be a UTF8 string
		using string_view_type = std::string_view;	//	should be a UTF8 string
		using availability_type = detail::Availability;


	public:

		PresenceProperties() = default;
		PresenceProperties(const PresenceProperties&) = delete;
		virtual ~PresenceProperties() = default;

		virtual void BeginSession(availability_type initialAvailability);

		virtual void SetAvailable();
		virtual void SetIdle(string_view_type message, bool isBusy, string_view_type idleUnknown);
		virtual void SetAway(availability_type availability, bool isBusy);
		virtual void SetCustomStatus(string_view_type message, bool isBusy);
		virtual void SetInvisible();

	protected:

		availability_type	availability_ = availability_type::Offline;
		bool				busy_ = false;
		string_type			message_;
	};

}}}
