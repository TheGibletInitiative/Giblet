//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsgMock/AuthenticationHandler.h>
#include <Protocols/YMsgMock/BlockedContactManagementLink.h>
#include <Protocols/YMsgMock/ContactManagementLink.h>
#include <Protocols/YMsgMock/PresenceManagementLink.h>
#include <Protocols/YMsgMock/ProfileManagementLink.h>
#include <Protocols/YMsgMock/SessionContext.h>
#include <Protocols/YMsg/DispatchMaps.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	void MockAuthenticationHandler::OnSessionAuthenticated(PacketDispatcher& dispatcher)
	{
		const auto profileManager(std::make_shared<ProfileManager>());
		const auto profileManagementEvents(std::make_shared<ProfileManagementEvents>(connection_, profileManager));
		const auto profileManagement(std::make_shared<MockProfileManagementLink>(profileManager, profileManagementEvents));
		const auto blockedContactManager(std::make_shared<BlockedContactManager>());
		const auto blockedContactManagementEvents(std::make_shared<BlockedContactManagementEvents>(connection_, blockedContactManager));
		const auto blockedContactManagementLink(std::make_shared<MockBlockedContactManagementLink>(blockedContactManager, blockedContactManagementEvents));
		const auto contactManager(std::make_shared<ContactManager>());
		const auto contactManagementEvents(std::make_shared<ContactManagementEvents>(connection_, blockedContactManager, profileManager, contactManager));
		const auto contactManagement(std::make_shared<MockContactManagementLink>(blockedContactManager, profileManager, contactManager, contactManagementEvents));
		const auto presenceEvents(std::make_shared<PresenceEvents>(connection_, contactManager));
		const auto presenceManagementLink(std::make_shared<MockPresenceManagementLink>(profileManager, contactManager, presenceEvents, initialAvailability_));
		const auto session(std::make_shared<MockSessionContext>(connection_, blockedContactManager, blockedContactManagementLink, profileManager, profileManagement, contactManager, contactManagement, presenceManagementLink));

		profileManager->Load(clientId_, { "winky", "dinky", "blinky", "brad", "greg" });
		contactManager->LoadContact(ContactInfo("jay", "Friends", ContactInfo::Linked));
		contactManager->LoadContact(ContactInfo("donna", "Friends", ContactInfo::Linked, ContactInfo::availability_type::Custom, false, "Listening to Bewiz - Booters Paradise"));
		contactManager->LoadContact(ContactInfo("david", "Friends", ContactInfo::Linked, ContactInfo::availability_type::Busy, true));
		contactManager->LoadContact(ContactInfo("winky", "Myself", ContactInfo::Linked, initialAvailability_));
		contactManager->LoadContact(ContactInfo("dinky", "Myself", ContactInfo::Linked, initialAvailability_));
		contactManager->LoadContact(ContactInfo("blinky", "Myself", ContactInfo::Linked, initialAvailability_));
		contactManager->LoadContact(ContactInfo("brad", "Myself", ContactInfo::Linked, initialAvailability_));
		blockedContactManager->Load({ "wierdo", "jerk", "germinator" });


		//	Start the session
		connection_->SetSessionId(rand());
		session->BeginSession();

		////	FIXME: We will be destroyed (along with context) during the call to SetDispatcher. Not ideal!
		dispatcher.SetDispatchMap(DispatchMaps::V11::Create(session));
	}

}}}
