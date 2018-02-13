//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/PresenceEvents.h>
#include <Protocols/YMsg/Server/Builders/ContactOnline.h>
#include <Protocols/YMsg/Server/Builders/ContactOffline.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	PresenceEvents::PresenceEvents(
		std::shared_ptr<ClientConnection> connection,
		std::shared_ptr<ContactManager> contactManager)
		:
		connection_(connection),
		contactManager_(contactManager)
	{
		if (!connection_)
		{
			throw std::invalid_argument("connection cannot be null");
		}

		if (!contactManager_)
		{
			throw std::invalid_argument("contacts cannot be null");
		}
	}




	void PresenceEvents::OnContactAvailable(string_view_type contactId)
	{
		auto contactInfo(contactManager_->SetContactAvailable(contactId));
		if (!contactInfo)
		{
			throw std::runtime_error("Don't know how to handle idle contact not in list");
		}

		Server::Builders::UpdateContactStatus builder;
		builder.Build(*connection_, *contactInfo);
		builder.Send(*connection_);
	}


	void PresenceEvents::OnContactIdle(string_view_type contactId, string_view_type idleUnknown, string_view_type message, bool isBusy)
	{
		auto contactInfo(contactManager_->SetContactIdle(contactId, idleUnknown, message, isBusy));
		if (!contactInfo)
		{
			throw std::runtime_error("Don't know how to handle idle contact not in list");
		}

		Server::Builders::UpdateContactStatus builder;
		builder.Build(*connection_, *contactInfo);
		builder.Send(*connection_);
	}


	void PresenceEvents::OnContactAway(string_view_type contactId, availability_type availability, bool isBusy)
	{
		auto contactInfo(contactManager_->SetContactAway(contactId, availability, isBusy));
		if (!contactInfo)
		{
			throw std::runtime_error("Don't know how to handle idle contact not in list");
		}

		Server::Builders::UpdateContactStatus builder;
		builder.Build(*connection_, *contactInfo);
		builder.Send(*connection_);
	}


	void PresenceEvents::OnContactStatusMessage(string_view_type contactId, string_view_type message, bool isBusy)
	{
		auto contactInfo(contactManager_->SetContactStatusMessage(contactId, message, isBusy));
		if (!contactInfo)
		{
			throw std::runtime_error("Don't know how to handle idle contact not in list");
		}

		Server::Builders::UpdateContactStatus builder;
		builder.Build(*connection_, *contactInfo);
		builder.Send(*connection_);
	}


	void PresenceEvents::OnContactOffline(string_view_type contactId)
	{
		auto contactInfo(contactManager_->SetContactOffline(contactId));
		if (!contactInfo)
		{
			throw std::runtime_error("Don't know how to handle idle contact not in list");
		}

		Server::Builders::ContactOffline builder;
		builder.Build(*connection_, contactId);
		builder.Send(*connection_);
	}

}}}
