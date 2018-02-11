//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class NewMailNotification : public Builder
	{
	public:

		virtual void Build(connection_type& connection, unsigned int count)
		{
			Initialize(connection, ServiceId, AttributeId);
			Append(Keys::Count, count);	
		}


		virtual void Build(
			connection_type& connection,
			string_view_type address,
			string_view_type name,
			string_view_type subject)
		{
			Initialize(connection, ServiceId, AttributeId);
			Append(Keys::Count, 1);	
			Append(Keys::Name, name);	
			Append(Keys::Address, address);	
			Append(Keys::Subject, subject);	
		}


	protected:

		struct Keys
		{
			static const key_type Count = 9;
			static const key_type Name = 43;
			static const key_type Address = 42;
			static const key_type Subject = 18;
		};

		static const serviceid_type ServiceId = 11;
		static const attribute_type AttributeId = 1;
	};

}}}}}
