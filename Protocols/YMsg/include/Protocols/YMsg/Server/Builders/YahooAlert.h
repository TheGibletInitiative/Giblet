//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	class YahooAlert : public Builder
	{
	public:

		virtual void Build(
			session_type& session,
			string_view_type publisher,
			string_view_type url,
			string_view_type articleMeta,
			string_view_type title,
			string_view_type content)
		{
			Initialize(session, ServiceId, AttributeId);
			Append(Keys::ClientId, session.GetClientId());	
			Append(Keys::Url, url);		
			Append(Keys::PublisherId, publisher);
			Append(Keys::ArticleMeta, articleMeta);
			Append(Keys::Title, title);	
			Append(Keys::Content, content);	
		}


	protected:

		struct Keys
		{
			static const key_type ClientId = 5;
			static const key_type Url = 132;
			static const key_type PublisherId = 4;
			static const key_type ArticleMeta = 14;
			static const key_type Title = 83;
			static const key_type Content = 20;		//	Observed as "0". Related to IMVironments.
		};

		static const serviceid_type ServiceId = 300;
		static const attribute_type AttributeId = 1;
	};

}}}}}
