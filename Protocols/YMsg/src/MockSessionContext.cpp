//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/MockSessionContext.h>
#include <Protocols/YMsg/Server/Builders/PingConfiguration.h>
#include <Protocols/YMsg/Server/Builders/ContactOnline.h>
#include <Protocols/YMsg/Server/Builders/NewMailNotification.h>
#include <Protocols/YMsg/Server/Builders/OfflineMessage.h>
#include <Protocols/YMsg/Server/Builders/YahooAlert.h>


namespace Giblet { namespace Protocols { namespace YMsg
{

	void MockSessionContext::BeginSession(sessionid_type id, string_view_type clientId, availability_type initialAvailability)
	{
		SessionContext::BeginSession(id, clientId, initialAvailability);

		profileManager_->Load(clientId, { "winky", "dinky", "blinky", "brad", "greg" });
		contactManager_->LoadContact(ContactInfo("jay", "Friends", ContactInfo::Linked));
		contactManager_->LoadContact(ContactInfo("donna", "Friends", ContactInfo::Linked, ContactInfo::availability_type::Custom, false, "Listening to Bewiz - Booters Paradise"));
		contactManager_->LoadContact(ContactInfo("david", "Friends", ContactInfo::Linked, ContactInfo::availability_type::Busy, true));
		contactManager_->LoadContact(ContactInfo("winky", "Myself", ContactInfo::Linked, initialAvailability));
		contactManager_->LoadContact(ContactInfo("dinky", "Myself", ContactInfo::Linked, initialAvailability));
		contactManager_->LoadContact(ContactInfo("blinky", "Myself", ContactInfo::Linked, initialAvailability));
		contactManager_->LoadContact(ContactInfo("brad", "Myself", ContactInfo::Linked, initialAvailability));
		blockedContactManager_->Load({ "wierdo", "jerk", "germinator" });


		OnAuthenticationComplete(clientId);
	}

	void MockSessionContext::OnAuthenticationComplete(string_view_type clientId)
	{
		SessionContext::OnAuthenticationComplete(clientId);


		if(clientId.find('@') != string_type::npos)
		{
			//	Example yahoo alert
			Server::Builders::YahooAlert yahooAlertBuilder;
			yahooAlertBuilder.Build(
				*connection_,
				clientId,
				"Yahoo! Alerts",
				"http://beta.alerts.yahoo.com/main.php?view=my_alerts",
				"n - BreakingNews\nabstract Id: d0300002215e2\nhttp://news.yahoo.com",
				"Breaking News (AP, Reuters)",
				"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n<HTML>\n<HEAD><TITLE>Yahoo! Alerts</TITLE>\n<META http-equiv=Content-Type \ncontent=\"text/html; charset=windows-1252\"><BASEFONT face=arial&#44;sans-serif \nsize=2>\n</HEAD>\n<BODY><B>Yahoo! Alerts</B> \n<TABLE cellSpacing=0 cellPadding=0 width=\"100%\" border=0>\n  <TBODY>\n  <TR>\n    <TD noWrap bgColor=#acb59a height=1><SPACER height=\"1\" width=\"1\" \n      type=\"block\"></TD></TR>\n  <TR>\n    <TD bgColor=#effada height=24><FONT size=2><FONT color=#ec661f><B>Breaking\n      News</B></FONT> Sunday&#44; September 26&#44; 2004&#44; 11:06 PM PDT </FONT></TD></TR>\n  <TR>\n    <TD noWrap bgColor=#acb59a height=1><SPACER height=\"1\" width=\"1\" \n      type=\"block\"></TD></TR></TBODY></TABLE>\n<P>\nMOSUL&#44; Iraq (AP) Police say a car bomb has exploded near an Iraqi National Guard patrol in the northeastern city of Mosul. </P>\n  <BR>\n  <p>\n    Visit <a href=\"http://news.yahoo.com\">Yahoo! News</a> for more details or search for this story on <a href=\"http://news.search.yahoo.com/search/news/?fr=sfp&ei=UTF-8&p=MOSUL&#44; Iraq (AP) Police say a car bomb has exploded near an Iraqi National Guard patrol in the northeastern city of Mosul.\">Yahoo! News Search</a>.  \n  </p>\n  <br>\n</td></tr></table>\n</BODY></BASEFONT></HTML>");
			yahooAlertBuilder.Send(*connection_);
		}

		if(clientId.find('-') != string_type::npos)
		{
			Server::Builders::NewMailNotification newMailNotifyBuilder;
			newMailNotifyBuilder.Build(*connection_, 5);
			newMailNotifyBuilder.Send(*connection_);
		}

		if(clientId.find('_') != string_type::npos)
		{
			Server::Builders::NewMailNotification newMailNotifyBuilder;
			newMailNotifyBuilder.Build(*connection_, "me@yahoo.com", "Some Guy", "Some subject");
			newMailNotifyBuilder.Send(*connection_);
		}

		if(clientId.find('.') != string_type::npos)
		{
			Server::Builders::OfflineMessage messageBuilder;
			messageBuilder.Build(*connection_, clientId, "Ed_", "Hello!", Server::Builders::OfflineMessage::encoding_type::None);
			messageBuilder.Send(*connection_);
		}

		//{
		//	Builders::PrivateMessage messageBuilder;
		//	messageBuilder.Build(session, clientId, "Ed_", "Hello!", TextEncoding::None);
		//	messageBuilder.Send(session);
		//}
	}



}}}
