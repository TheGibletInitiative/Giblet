//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Payloads/SetAway.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	bool SetAway::OnKeyPair(key_type key, string_view_type value)
	{
		switch(key)
		{
		case Keys::StatusId:
			statusId = value;
			break;

		case Keys::CustomMessage:
			customMessage = value;
			break;

		case Keys::BusyState:
			busyState = value;
			break;

		case Keys::IdleUnknown138:
			idleUnknown = value;
			break;


		default:
			return false;
		}

		return true;
	}

}}}}}
