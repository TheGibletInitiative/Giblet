//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Payloads/ServiceOperation.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	bool ServiceOperation::OnKeyPair(key_type key, string_view_type value)
	{
		switch(key)
		{
		case Keys::Operation:
			operation = value;
			break;

		case Keys::ClientId:
			clientId = value;
			break;

		case Keys::Status:
			status = value;
			break;

		case Keys::Action:
			action = value;
			break;

		case Keys::ContactId:
			contactId = value;
			break;

		case Keys::Unknown:
			break;

		default:
			return false;
		}

		return true;
	}

}}}}}
