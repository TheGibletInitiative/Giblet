//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>
#include <map>

namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class ClientProfile : public Builder
	{
	public:

		virtual void Build(session_type& session);

		virtual void Build(
			session_type& session,
			string_view_type clientId,
			const std::map<string_type, std::vector<string_type>>& contactList,
			const std::vector<string_view_type>& profileNames,
			const std::vector<string_view_type>& ignoreList);


	protected:

		struct Keys
		{
			static const key_type ContactList = 87;		//	List of contacts. EXAMPLE: "Group1:user1,user2\nGroup2:user3,user4\n" (always terminate with new line)
			static const key_type BlockedList = 88;
			static const key_type ClientProfiles = 89;	//	Comma separated list of profile names
			static const key_type Cookie = 59;
			static const key_type Unknown_90 = 90;		//	TODO: Observed as "0"
			static const key_type ClientId = 3;
			static const key_type Unknown_100 = 100;	//	TODO: Observed as "0"
			static const key_type Unknown_101 = 101;	//	TODO: Observed as ""
			static const key_type Unknown_102 = 102;	//	TODO: Observed as ""
			static const key_type Unknown_93 = 93;		//	TODO: Observed as "1440" and "86400" on 5.5/5.6
			static const key_type Unknown_149 = 149;	//	TODO: Observed as possibly Mac64 encoded.
			static const key_type Unknown_150 = 150;	//	TODO: Observed as possibly Mac64 encoded.
			static const key_type Unknown_151 = 151;	//	TODO: Observed as possibly Mac64 encoded.
		};

		static const serviceid_type ServiceId = 85;
		static const attribute_type AttributeId = 0;
	};

}}}}}
