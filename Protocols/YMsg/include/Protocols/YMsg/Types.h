//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <string>
#include <string_view>


//	FIXME: def for SOCKET. This shouldn't be needed in this library!
extern "C"
{
	typedef unsigned int SOCKET;
}

namespace Giblet { namespace Protocols { namespace YMsg
{

	namespace detail
	{

		enum class AuthenticationMethod
		{
			None = 0,
			InApp = 1,
			HTTPS = 2
		};

		enum class TextEncoding
		{
			None = 1,
			UTF8 = 2
		};

		enum class ClientType
		{
			Messenger = 0,
			WebChat = 1,	//	Old Web interface to YCHT protocol
			Game = 2,		//	Old web based games with lobby and in-game chat.
		};

		enum class Availability
		{
			Offline =		 -1,	//	Placeholder value (not part of ymsg proto def. FIXME: would like to remove it)
			Available =		  0,
			Brb =			  1,
			Busy =			  2,
			Notathome =		  3,
			NotAtDesk =		  4,
			NotInOffice =	  5,
			OnPhone =		  6,
			OnVacation =	  7,
			OutToLunch =	  8,
			SteppedOut =	  9,
			Invisible =		 12,	//	NOTE: This is only sent by the client never to the client. Send contact offline packet instead.
			Custom =		 99,
			Idle =			999,
		};

		enum class TypingStatus
		{
			Idle = 0,
			Active = 1
		};

		enum class ContactBlockedStatus
		{
			Blocked = 1,
			Unblocked = 2
		};

		enum class ContactBlockedFailedReason
		{
			None = -1,
			Duplicate = 2,
			IsFriend = 12,
			//	 NotIgnored = 3
		};

		//	TODO: Check to see if any of these requires a special attribute
		enum class AddContactRequestResultStatus
		{
			AddPending = 0,
			GeneralFailure = 1,
			DuplicateContact = 2,			//	Already in list
			ContactDoesNotExist = 3,
			InvalidCookies = 4,
			RecursiveLockup_BORKBORKBORK = 5,			//	This REALLY borks messenger
			ContactListFull = 6,
			TooManyGroups = 7,
			GroupAlreadyExists = 8,			//	Not sure what would trigger this
			CannotDeleteContact = 9,
			BlockedListFull = 10,
			CannotDeleteReservedId = 11,	//	Can't delete reserved id. Part of parental controls maybe?
			CannotDeleteContact2 = 12,		//	Duplicate status code. Gotta have two different reasons :)
		};

		//	TODO: Determine other values
		enum class RemoveContactStatus
		{
			Success = 0,
			GeneralFailure = 1,
			ContactDoesNotExist = 3
		};

	}


	template<class Type_>
	int validate_enum_conversion(typename std::underlying_type<Type_>::type value)
	{
		//static_assert(!"validate_enum_conversion implementation missing for type");
		return value;
	}


	//	FIXME: Need to pass a validator to ensure only values we expect are converted
	template<class Type_>
	Type_ to_enum(std::string_view value)
	{
		return static_cast<Type_>(validate_enum_conversion<Type_>(std::atoi(value.data())));
	}


}}}
