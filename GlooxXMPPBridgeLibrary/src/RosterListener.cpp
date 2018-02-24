//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <GlooxXMPPBridgeLibrary/RosterListener.h>
#include <iostream>


namespace Giblet { namespace Bridges { namespace Gloox
{

	RosterListener::RosterListener(std::shared_ptr<Protocols::YMsg::PresenceEvents> presenceEvents)
		: presenceEvents_(presenceEvents)
	{}


	void RosterListener::handleItemSubscribed( const gloox::JID& jid )
	{
		std::cerr << "subscribed " << jid.bare() << '\n';
	}

	void RosterListener::handleItemAdded( const gloox::JID& jid )
	{
		std::cerr << "added " << jid.bare() << '\n';
	}

	void RosterListener::handleItemUnsubscribed( const gloox::JID& jid )
	{
		std::cerr << "unsubscribed " << jid.bare() << '\n';
	}

	void RosterListener::handleItemRemoved( const gloox::JID& jid )
	{
		std::cerr << "removed " << jid.bare() << '\n';
	}

	void RosterListener::handleItemUpdated( const gloox::JID& jid )
	{
		std::cerr << "updated " << jid.bare() << '\n';
	}

	void RosterListener::handleRoster( const gloox::Roster& roster )
	{
		std::cerr << "roster arriving\nitems:\n";
		for(const auto& rosterItem : roster)
		{
			std::cerr
				<< '\t'
				<< "jid: " << rosterItem.second->jidJID().full()
				<< ", name: %s" << rosterItem.second->name()
				<< ", subscription: " << rosterItem.second->subscription()
				<< '\n';

			for (const auto& group : rosterItem.second->groups())
			{
				std::cerr << "\t\tgroup: " << group << '\n';
			}

			for (const auto& resource : rosterItem.second->resources())
			{
				std::cerr << "\t\tresource: " << resource.first.c_str() << '\n';
			}
		}
	}

	void RosterListener::handleRosterError( const gloox::IQ& /*iq*/ )
	{
		std::cerr << "a roster-related error occured\n";
	}

	void RosterListener::handleRosterPresence(
		const gloox::RosterItem& item,
		const std::string& resource,
		gloox::Presence::PresenceType presence,
		const std::string& msg )
	{
		std::cerr << "presence received: " << item.jidJID().full() << '/' << resource << " -- " << presence << '\n';
	}

	void RosterListener::handleSelfPresence(
		const gloox::RosterItem& item,
		const std::string& resource,
		gloox::Presence::PresenceType presence,
		const std::string& msg )
	{
		std::cerr << "presence received: " << item.jidJID().full() << '/' << resource << " -- " << presence << '\n';
	}

	bool RosterListener::handleSubscriptionRequest( const gloox::JID& jid, const std::string& msg )
	{
		std::cerr << "subscription: " << jid.bare() << '\n';
		//gloox::StringList groups;
		//gloox::JID id( jid );
		//j->rosterManager()->subscribe( id, "", groups, "" );
		return true;
	}

	bool RosterListener::handleUnsubscriptionRequest( const gloox::JID& jid, const std::string& msg )
	{
		std::cerr << "unsubscription: " << jid.bare() << '\n';
		return true;
	}

	void RosterListener::handleNonrosterPresence( const gloox::Presence& presence )
	{
		std::cerr << "received presence from entity not in the roster: " << presence.from().full() << '\n';
	}

}}}
