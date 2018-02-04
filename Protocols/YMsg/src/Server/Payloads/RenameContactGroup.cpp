//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Payloads/RenameContactGroup.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Payloads
{

	bool RenameContactGroup::OnKeyPair(key_type key, string_view_type value)
	{
		switch(key)
		{
		case Keys::ClientId:
			clientId = value;
			break;

		case Keys::CurrentGroupName:
			currentGroupName = value;
			break;

		case Keys::NewGroupName:
			newGroupName = value;
			break;

		default:
			return false;
		}

		return true;
	}

}}}}}
