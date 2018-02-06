//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class ContactRemoved : public Builder
	{
	public:

		using status_type = detail::RemoveContactStatus;


		virtual void Build(
			session_type& session,
			string_view_type clientId,
			string_view_type contactId,
			string_view_type group,
			status_type status)
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::ClientId, clientId);
			Append(Keys::Status, status);
			Append(Keys::ContactId, contactId);
			Append(Keys::Group, group);
		}


	protected:

		struct Keys
		{
			static const key_type ClientId = 1;
			static const key_type Status = 66;
			static const key_type ContactId = 7;
			static const key_type Group = 65;
		};

		static const serviceid_type ServiceId = 132;
		static const attribute_type AttributeId = 1;
	};

}}}}}
