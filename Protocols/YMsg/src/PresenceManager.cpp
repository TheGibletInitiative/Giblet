//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/PresenceManager.h>
#include <Protocols/YMsg/Server/Builders/ContactOnline.h>
#include <Protocols/YMsg/Server/Builders/ContactOffline.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	PresenceProperties::PresenceProperties(availability_type initialAvailability)
		:
		availability_(initialAvailability),
		busy_(false)
	{}


	void PresenceProperties::SetAvailable()
	{
		availability_ = availability_type::Available;
		busy_ = false;
		message_ = string_type();
	}


	void PresenceProperties::SetCustomStatus(string_view_type message, bool isBusy)
	{
		availability_ = availability_type::Custom;
		busy_ = isBusy;
		message_ = message;
	}


	void PresenceProperties::SetIdle(string_view_type message, bool isBusy, string_view_type idleUnknown)
	{
		((void)idleUnknown);

		availability_ = availability_type::Idle;
		busy_ = isBusy;
		message_ = message;
	}


	void PresenceProperties::SetAway(availability_type availability, bool isBusy)
	{
		availability_ = availability;
		busy_ = isBusy;
		message_ = string_type();
	}


	void PresenceProperties::SetInvisible()
	{
		availability_ = availability_type::Invisible;
		busy_ = false;	//	FIXME: Not sure we should change this!
		message_ = string_type();
	}

}}}
