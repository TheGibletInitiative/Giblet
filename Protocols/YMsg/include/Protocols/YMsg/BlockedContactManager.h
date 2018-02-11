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

	class BlockedContactManager
	{
	public:

		using string_type = std::string;
		using string_view_type = std::string_view;
		using container_type = std::vector<string_type>;
		using const_iterator = container_type::const_iterator;


	public:

		explicit BlockedContactManager(std::shared_ptr<ClientConnection> connection);

		BlockedContactManager(const BlockedContactManager&) = delete;

		virtual void Load(container_type&& blockedContacts);

		virtual const_iterator begin() const;
		virtual const_iterator end() const;

		virtual bool IsBlocked(string_view_type contactId) const;

		//
		virtual void Add(string_view_type clientId, string_view_type contactId) = 0;
		virtual void Remove(string_view_type clientId, string_view_type contactId) = 0;



	protected:

		virtual void OnNotInBlockedList(string_view_type clientId, string_view_type contactId);
		virtual void OnAlreadyBlocked(string_view_type clientId, string_view_type contactId);
		virtual void OnBlocked(string_view_type clientId, string_view_type contactId);
		virtual void OnUnblocked(string_view_type clientId, string_view_type contactId);

	protected:

		const std::shared_ptr<ClientConnection>	connection_;
		container_type	blockedUsers_;
	};

}}}
