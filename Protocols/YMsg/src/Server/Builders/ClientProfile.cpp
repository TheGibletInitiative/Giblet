//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/Server/Builders/ClientProfile.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	void ClientProfile::Build(session_type& session)
	{
		const std::vector<string_view_type> profileNames(session.profiles_begin(), session.profiles_end());
		const std::vector<string_view_type> ignoreList(session.blockedusers_begin(), session.blockedusers_end());

		//	TODO: This could be better!
		std::map<string_type, std::vector<string_type>> contactList;
		for (auto group = session.grouped_contacts_begin(); group != session.grouped_contacts_end(); ++group)
		{
			contactList[group->first] = group->second;
		}

		Build(session, session.GetClientId(), contactList, profileNames, ignoreList);
	}


	void ClientProfile::Build(
		session_type& session,
		string_view_type clientId,
		const std::map<string_type, std::vector<string_type>>& contactList,
		const std::vector<string_view_type>& profileNames,
		const std::vector<string_view_type>& ignoreList)
	{
		//	Convert the list of profile names
		string_type profileNamesString(clientId);
		for (const auto& profileName : profileNames)
		{
			profileNamesString += ',';
			profileNamesString += profileName;
		}

		//	Convert the contact list
		string_type contactListString;
		for (const auto& group : contactList)
		{
			contactListString += group.first + ':';
			if (!group.second.empty())
			{
				for (const auto& contact : group.second)
				{
					contactListString += contact;
					contactListString += ',';
				}
				contactListString.pop_back();
			}
			contactListString += '\n';
		}

		//	Convert the list of ignored users
		string_type ignoreListString;
		if (!ignoreList.empty())
		{
			for (const auto ignoredUser : ignoreList)
			{
				ignoreListString += ignoredUser;
				ignoreListString += ',';
			}

			ignoreListString.pop_back();
		}

		//	TODO: Figure out how to handle cookies
#if 0
		const auto cookie_Y("Y\x09v=1&n=1se330uvmkl5n&l=oi784b3_1ej_t/o&p=m262mtn613000800&jb=36|21|&r=ba&lg=us&intl=us; expires=Thu, 15 Apr 2020 20:00:00 GMT; path=/; domain=.yahoo.com.localhost");
		const auto cookie_T("T\x09z=lePuABlkkuABb0sWHN/HhV5NTYyBjU2NjFPNDVOMk4-&a=QAE&sk=DAA.MG42ljjnST&d=c2wBTWpFMUFUSXhNVFk0TXpJNU5Uay0BYQFRQUUBdGlwAUVOR2hWQgF6egFsZVB1QUJnV0E-; expires=Thu, 15 Apr 2020 20:00:00 GMT; path=/; domain=.yahoo.com.localhost");
		const auto cookie_C("C\x09mg=1");
#else
		const auto cookie_Y("");
		const auto cookie_T("");
		const auto cookie_C("");
#endif
		//	TODO: Probably won't need the cookies. See about removing them
		//	TODO: Cross check these with older versions of libyahoo and libyahoo2
		//	TODO: verify attribute
		Initialize(session, ServiceId, AttributeId);
		Append(Keys::ContactList, contactListString);
		Append(Keys::BlockedList, ignoreListString);
		Append(Keys::ClientProfiles, profileNamesString);
		Append(Keys::Cookie, cookie_Y);
		Append(Keys::Cookie, cookie_T);
		Append(Keys::Cookie, cookie_C);
		Append(Keys::Unknown_90, "0");
		Append(Keys::ClientId, clientId);
		Append(Keys::Unknown_100, "0");
		Append(Keys::Unknown_101, "");
		Append(Keys::Unknown_102, "");
		Append(Keys::Unknown_93, "1440");	//	Settings (86400 disables chat)
		Append(Keys::Unknown_149, "zyeAQ1BXFCy3bxoFj6jgTg--");
		Append(Keys::Unknown_150, "78zL3ked8mbTjdBp8tCDCw--");
		Append(Keys::Unknown_151, "u301TRymraydeTDywSe_7Q--");
	}

}}}}}
