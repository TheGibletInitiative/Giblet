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

	PresenceManager::PresenceManager(std::shared_ptr<ClientConnection> connection)
		:
		connection_(connection),
		availability_(availability_type::Offline),
		busy_(false)
	{}


	void PresenceManager::BeginSession(availability_type initialAvailability)
	{
		availability_ = initialAvailability;
		busy_ = false;
		message_ = string_type();
	}




	void PresenceManager::SetAvailable()
	{
		availability_ = availability_type::Available;
		busy_ = false;
		message_ = string_type();
	}


	void PresenceManager::SetCustomStatus(string_view_type message, bool isBusy)
	{
		availability_ = availability_type::Custom;
		busy_ = isBusy;
		message_ = message;
	}


	void PresenceManager::SetIdle(string_view_type message, bool isBusy, string_view_type idleUnknown)
	{
		availability_ = availability_type::Idle;
		busy_ = isBusy;
		message_ = message;
	}


	void PresenceManager::SetAway(availability_type availability, bool isBusy)
	{
		availability_ = availability;
		busy_ = isBusy;
		message_ = string_type();
	}


	void PresenceManager::SetInvisible()
	{
		availability_ = availability_type::Invisible;
		busy_ = false;	//	FIXME: Not sure we should change this!
		message_ = string_type();
	}

}}}
