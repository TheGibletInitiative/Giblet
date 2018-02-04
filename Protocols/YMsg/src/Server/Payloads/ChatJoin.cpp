//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Payloads/ChatJoin.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	bool ChatJoin::OnKeyPair(key_type key, string_view_type value)
	{
		switch(key)
		{
		case Keys::ClientId:
			clientId = value;
			break;

		case Keys::Category:
			category = value;
			break;

		case Keys::Room:
			room = value;
			break;

		case Keys::Unknown:
			break;

		default:
			return false;
		}

		return true;
	}

}}}}}
