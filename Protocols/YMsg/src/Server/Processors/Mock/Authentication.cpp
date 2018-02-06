//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Processors/Mock/Authentication.h>
#include <Protocols/YMsg/Server/Builders/PingConfiguration.h>
#include <Protocols/YMsg/Server/Builders/ClientProfile.h>
#include <Protocols/YMsg/Server/Builders/ContactOnline.h>
#include <Protocols/YMsg/Server/Builders/OfflineMessage.h>
#include <Protocols/YMsg/Server/Builders/PrivateMessage.h>
#include <Protocols/YMsg/Server/Builders/NewMailNotification.h>
#include <Protocols/YMsg/Server/Builders/YahooAlert.h>
#include <Protocols/YMsg/Server/Builders/PrivateMessageError.h>
#include <algorithm>
#include <utility>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Processors { namespace Mock
{

	void Authentication::Process(session_type& session, const header_type& header, payload_type& payload)
	{
		//	FIXME: Need to define attributes the same way we do keys
		const auto initialAvailability(
			header.attribute == static_cast<std::underlying_type<detail::Availability>::type>(detail::Availability::Invisible) 
			? detail::Availability::Offline
			: detail::Availability::Available);

		//	FIXME: Check for already being logged in
		session.BeginSession(rand(), payload.clientId1_, initialAvailability);

		session.AddProfile("winky");
		session.AddProfile("dinky");
		session.AddProfile("blinky");
		session.AddProfile("brad");
		session.AddProfile("greg");

		session.AddContact("Friends", ContactInfo(ContactInfo::Linked, "jay"));
		session.AddContact("Friends", ContactInfo(ContactInfo::Linked, "donna", ContactInfo::availability_type::Custom, false, "Listening to Bewiz - Booters Paradise"));
		session.AddContact("Friends", ContactInfo(ContactInfo::Linked, "david", ContactInfo::availability_type::Busy, true));
		session.AddContact("Myself", ContactInfo(ContactInfo::Linked, "winky", initialAvailability));
		session.AddContact("Myself", ContactInfo(ContactInfo::Linked, "dinky", initialAvailability));
		session.AddContact("Myself", ContactInfo(ContactInfo::Linked, "blinky", initialAvailability));
		session.AddContact("Myself", ContactInfo(ContactInfo::Linked, "brad", initialAvailability));

		session.AddBlockedId("wierdo");
		session.AddBlockedId("jerk");
		session.AddBlockedId("germinator");

		{
			//	NOTE: We originally passed payload.clientId1_ as the name field but we need to
			//	add more management of what the actual login id is if we want to FULLY support profile
			//	id's since you could log in with them as well as your primary id.
			Builders::ClientProfile	clientProfileBuilder;
			clientProfileBuilder.Build(session);
			clientProfileBuilder.Send(session);
		}

		{
			auto isContactOnline = [](const ContactInfo& contact) -> bool
			{
				return contact.availability != ContactInfo::availability_type::Offline;
			};

			//	user online
			std::vector<ContactInfo> onlineContacts;
			copy_if(session.contacts_begin(), session.contacts_end(), back_inserter(onlineContacts), isContactOnline);

			Builders::SetContactStatus contactOnlineBuilder;
			contactOnlineBuilder.Build(session, payload.clientId1_, onlineContacts);
			contactOnlineBuilder.Send(session);
		}


		{
			//	Ping configuration
			Builders::PingConfiguration pingConfigBuilder;
			pingConfigBuilder.Build(session);
			pingConfigBuilder.Send(session);
		}

		if(session.GetClientId().find('@') != string_type::npos)
		{
			//	Example yahoo alert
			Builders::YahooAlert yahooAlsertBuilder;
			yahooAlsertBuilder.Build(
				session,
				"Yahoo! Alerts",
				"http://beta.alerts.yahoo.com/main.php?view=my_alerts",
				"n - BreakingNews\nabstract Id: d0300002215e2\nhttp://news.yahoo.com",
				"Breaking News (AP, Reuters)",
				"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n<HTML>\n<HEAD><TITLE>Yahoo! Alerts</TITLE>\n<META http-equiv=Content-Type \ncontent=\"text/html; charset=windows-1252\"><BASEFONT face=arial&#44;sans-serif \nsize=2>\n</HEAD>\n<BODY><B>Yahoo! Alerts</B> \n<TABLE cellSpacing=0 cellPadding=0 width=\"100%\" border=0>\n  <TBODY>\n  <TR>\n    <TD noWrap bgColor=#acb59a height=1><SPACER height=\"1\" width=\"1\" \n      type=\"block\"></TD></TR>\n  <TR>\n    <TD bgColor=#effada height=24><FONT size=2><FONT color=#ec661f><B>Breaking\n      News</B></FONT> Sunday&#44; September 26&#44; 2004&#44; 11:06 PM PDT </FONT></TD></TR>\n  <TR>\n    <TD noWrap bgColor=#acb59a height=1><SPACER height=\"1\" width=\"1\" \n      type=\"block\"></TD></TR></TBODY></TABLE>\n<P>\nMOSUL&#44; Iraq (AP) Police say a car bomb has exploded near an Iraqi National Guard patrol in the northeastern city of Mosul. </P>\n  <BR>\n  <p>\n    Visit <a href=\"http://news.yahoo.com\">Yahoo! News</a> for more details or search for this story on <a href=\"http://news.search.yahoo.com/search/news/?fr=sfp&ei=UTF-8&p=MOSUL&#44; Iraq (AP) Police say a car bomb has exploded near an Iraqi National Guard patrol in the northeastern city of Mosul.\">Yahoo! News Search</a>.  \n  </p>\n  <br>\n</td></tr></table>\n</BODY></BASEFONT></HTML>");
			yahooAlsertBuilder.Send(session);
		}

		if(session.GetClientId().find('-') != string_type::npos)
		{
			Builders::NewMailNotification newMailNotifyBuilder;
			newMailNotifyBuilder.Build(session, 5);
			newMailNotifyBuilder.Send(session);
		}

		if(session.GetClientId().find('_') != string_type::npos)
		{
			Builders::NewMailNotification newMailNotifyBuilder;
			newMailNotifyBuilder.Build(session, "me@yahoo.com", "Some Guy", "Some subject");
			newMailNotifyBuilder.Send(session);
		}

		if(session.GetClientId().find('.') != string_type::npos)
		{
			Builders::OfflineMessage messageBuilder;
			messageBuilder.Build(session, session.GetClientId(), "Ed_", "Hello!", Builders::OfflineMessage::encoding_type::None);
			messageBuilder.Send(session);
		}

		//{
		//	Builders::PrivateMessage messageBuilder;
		//	messageBuilder.Build(session, session.GetClientId(), "Ed_", "Hello!", TextEncoding::None);
		//	messageBuilder.Send(session);
		//}
	}

}}}}}}
