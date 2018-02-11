//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class ChallengeResponse : public Builder
	{
	public:

		using authentication_type = detail::AuthenticationMethod;

		virtual void Build(connection_type& connection, string_view_type clientId, string_view_type challenge)
		{
			Initialize(connection, ServiceId, AttributeId);
			Append(Keys::ClientId, clientId);
			Append(Keys::Challenger, challenge);
			Append(Keys::Method, authentication_type::InApp);
		}


		virtual void Build(connection_type& connection, string_view_type clientId)
		{
			Initialize(connection, ServiceId, AttributeId);
			Append(Keys::ClientId, clientId);
			Append(Keys::Method, authentication_type::None);
		}


	protected:

		struct Keys
		{
			static const key_type ClientId = 1;
			static const key_type Challenger = 94;
			static const key_type Method = 13;
		};

		static const serviceid_type ServiceId = 87;
		static const attribute_type AttributeId = 1;
	};

}}}}}
