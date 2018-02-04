//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Payloads/ChallengeRequest.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	bool ChallengeRequest::OnKeyPair(key_type key, string_view_type value)
	{
		switch(key)
		{
		case Keys::ClientId:
			clientId = value;
			break;

		default:
			return false;
		}

		return true;
	}

}}}}}
