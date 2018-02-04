//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	//	FIXME: When switching to invisible we get the notification but the names stay bold
	//	in the buddy list.
	class ContactOffline : public Builder
	{
	public:

		virtual void Build(session_type& session, string_view_type contactId)
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::Unknown10, "0");
			Append(Keys::Unknown13, "0");
			Append(Keys::Unknown47, "2");
			Append(Keys::ContactId, contactId);
		}


	protected:

		struct Keys
		{
			static const key_type ContactId = 7;
			static const key_type Unknown10 = 10;	//	Must be "0"?
			static const key_type Unknown13 = 13;	//	Must be "0"?
			static const key_type Unknown47 = 47;	//	Must be "2"?
		};

		static const serviceid_type ServiceId = 2;
		static const attribute_type AttributeId = 1;
	};

}}}}}
