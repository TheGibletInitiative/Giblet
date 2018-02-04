//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class ContactGroupRenamed : public Builder
	{
	public:

		virtual void Build(
			session_type& session,
			string_view_type clientId,
			string_view_type oldGroupName,
			string_view_type newGroupName)
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::ClientId, clientId);
			Append(Keys::Unknown66, "0");	//	TODO: Verify what these values do. Everywhere key 66 shows up appears to be ralted to online status.
			Append(Keys::NewGroupName, newGroupName);
			Append(Keys::OldGroupName, oldGroupName);
		}


	protected:

		struct Keys
		{
			static const key_type ClientId = 1;
			static const key_type OldGroupName = 65;
			static const key_type NewGroupName = 67;
			static const key_type Unknown66 = 66;
		};

		static const serviceid_type ServiceId = 137;
		static const attribute_type AttributeId = 1;
	};

}}}}}
