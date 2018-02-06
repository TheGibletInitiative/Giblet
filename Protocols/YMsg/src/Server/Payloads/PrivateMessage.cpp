//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Payloads/PrivateMessage.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	bool PrivateMessage::OnKeyPair(key_type key, string_view_type value)
	{
		switch(key)
		{
		case Keys::ClientId:
			clientId = value;
			break;

		case Keys::ContactId:
			contactId = value;
			break;

		case Keys::Message:
			message = value;
			break;

		case Keys::Encoding:
			encoding = to_enum<encoding_type>(value);
			break;

		case Keys::IMVironment:
			break;

		case Keys::Unknown64:
		case Keys::Unknown1002:
			value = value;
			break;

		default:
			return false;
		}

		return true;
	}

}}}}}
