//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Payloads/ChatLogin.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	bool ChatLogin::OnKeyPair(key_type key, string_view_type value)
	{
		switch(key)
		{
		case Keys::ClientId1:
			clientId1 = value;
			break;

		case Keys::ClientId2:
			clientId2 = value;
			break;

		case Keys::Region:
		case Keys::MessengerVersion:
		case Keys::Unknown:
			break;

		default:
			return false;
		}

		return true;
	}

}}}}}
