//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Payloads/Authentication.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	bool Authentication::OnKeyPair(key_type key, string_view_type value)
	{
		switch (key)
		{
			
		case Keys::AuthResponse1:
			authResponse1_ = value;
			break;

		case Keys::AuthResponse2:
			authResponse2_ = value;
			break;

		case Keys::ClientVersion:
			clientVersion_ = value;
			break;

		case Keys::Unknown148:
			unknown148_ = value;
			break;

		case Keys::ClientId1:
			clientId1_ = value;
			break;

		case Keys::ClientId2:
			clientId2_ = value;
			break;

		case Keys::ProfileId:
			profileIds_.push_back(value);
			break;

		default:
			return false;
		}

		return true;
	}

}}}}}
