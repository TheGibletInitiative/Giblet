//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class AddContactRequestResult : public Builder
	{
	public:

		using status_type = detail::AddContactRequestResultStatus;

		virtual void Build(
			connection_type& connection,
			string_view_type clientId,
			string_view_type contactId,
			string_view_type contactGroup,
			status_type status)
		{
			Initialize(connection, ServiceId, AttributeId);
			Append(Keys::ContactId, contactId);
			Append(Keys::ClientId, clientId);
			Append(Keys::Group, contactGroup);
			Append(Keys::Status, status);
		}


	protected:

		struct Keys
		{
			static const key_type ContactId = 1;
			static const key_type ClientId = 7;
			static const key_type Group = 65;
			static const key_type Status = 66;
		};

		static const serviceid_type ServiceId = 131;
		static const attribute_type AttributeId = 1;
	};

}}}}}
