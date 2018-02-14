//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/ClientConnection.h>
#include <vector>
#include <string>
#include <string_view>
#include <memory>


namespace Giblet { namespace Protocols { namespace YMsg
{

	class ProfileManager
	{
	public:

		using string_type = std::string;			//	should be a UTF8 string
		using string_view_type = std::string_view;	//	should be a UTF8 string
		using container_type = std::vector<string_type>;
		using const_iterator = container_type::const_iterator;


	public:

		ProfileManager() = default;
		ProfileManager(const ProfileManager&) = delete;
		virtual ~ProfileManager() = default;

		virtual void Load(string_view_type clientId, container_type profiles);

		virtual const_iterator begin() const;
		virtual const_iterator end() const;

		virtual string_view_type GetClientId() const;
		virtual bool IsProfileId(string_view_type id) const;
		virtual bool IsClientOrProfileId(string_view_type id) const;


	protected:

		string_type		clientId_;
		container_type	profiles_;
	};

}}}
