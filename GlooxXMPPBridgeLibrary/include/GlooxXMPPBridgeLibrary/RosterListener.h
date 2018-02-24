//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <rosterlistener.h>
#include <Protocols/YMsg/PresenceEvents.h>


namespace Giblet { namespace Bridges { namespace Gloox
{
	class RosterListener : public gloox::RosterListener
	{
	public:

		RosterListener(std::shared_ptr<Protocols::YMsg::PresenceEvents> presenceEvents);

		virtual void handleItemSubscribed(const gloox::JID& jid) override;
		virtual void handleItemAdded(const gloox::JID& jid) override;
		virtual void handleItemUnsubscribed(const gloox::JID& jid) override;
		virtual void handleItemRemoved(const gloox::JID& jid) override;
		virtual void handleItemUpdated(const gloox::JID& jid) override;
		virtual void handleRoster(const gloox::Roster& roster) override;
		virtual void handleRosterError(const gloox::IQ& iq) override;
		virtual void handleRosterPresence(
			const gloox::RosterItem& item,
			const std::string& resource,
			gloox::Presence::PresenceType presence,
			const std::string& msg) override;
		virtual void handleSelfPresence(
			const gloox::RosterItem& item,
			const std::string& resource,
			gloox::Presence::PresenceType presence,
			const std::string& msg) override;
		virtual bool handleSubscriptionRequest(const gloox::JID& jid, const std::string& msg) override;
		virtual bool handleUnsubscriptionRequest(const gloox::JID& jid, const std::string& msg) override;
		virtual void handleNonrosterPresence(const gloox::Presence& presence) override;


	protected:

		std::shared_ptr<Protocols::YMsg::PresenceEvents> presenceEvents_;
	};

}}}
