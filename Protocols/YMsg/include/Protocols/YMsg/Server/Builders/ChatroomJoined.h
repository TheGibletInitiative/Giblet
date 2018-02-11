//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Builder.h>


namespace Giblet { namespace Protocols { namespace YMsg { namespace Server { namespace Builders
{

	//	Split out into ChatroomJoinSuceeded/ChatroomJoined, ChatroomJoinFailed
	class ChatroomJoined : public Builder
	{
	public:

		virtual void Build(
			connection_type& connection,
			string_view_type category,
			string_view_type roomName,
			string_view_type topic,
			string_view_type voiceCookie,
			string_view_type otherCookie,
			const std::vector<string_view_type>& participants)
		{
			Initialize(connection, ServiceId, AttributeId);
			Append(Keys::RoomName, roomName);
			Append(Keys::Topic, topic);
			Append(Keys::Category, category);
			Append(Keys::ParticipantCount, participants.size());
			Append(Keys::RoomSpace, "0");
			Append(Keys::VoiceCookie, voiceCookie);
			Append(Keys::RoomConfiguration, "0");
			Append(Keys::JoinedRoomFlag, "1");
			Append(Keys::OtherCookie, otherCookie);

			for (const auto& participant : participants)
			{
				Append(Keys::Participant::Id, participant);
			}
		}


	protected:

		struct Keys
		{
			static const key_type RoomName = 104;
			static const key_type Topic = 105;
			static const key_type Category = 128;
			static const key_type ParticipantCount = 108;
			static const key_type RoomSpace = 129;			//	Similar to the room category
			static const key_type VoiceCookie = 130;		//	FIXME: This may be switched with the other cookie.
			static const key_type RoomConfiguration = 126;	//	TODO: Need bitmasks for settings
			static const key_type JoinedRoomFlag = 13;		//	Only present on first entering a room and always 1 when present
			static const key_type StatusCode = 114;
			static const key_type OtherCookie = 61;			//	FIXME: This may be switched with the voice cookie. ?Webcam? cookie
			struct Participant
			{
				static const key_type Id = 109;
				static const key_type Age = 110;
				static const key_type Unknown112 = 112;		//	FIXME: Observed as different values entering a room and when another user enters (same packet)
				static const key_type Flags = 113;
				static const key_type Nickname = 141;
				static const key_type Location = 142;
			};
		};

		//	FIXME: These are actually from the client create room settings. Need to validate them
		//	against this packet. These just don't seem right anyway
		//	258		  102	00000000100000010	Private, no voice, no invite required
		//	256		  100	00000000100000000	Public, no voice, no invite required
		//	260		  104	00000000100000100	Public, no voice, invite required
		//	65792	10100	10000000100000000	Public, enabled voice, no invite required
		struct SettingMasks
		{
			static const uint32_t IsPrivate = 0x2;			
			static const uint32_t IsPublic = 0x100;			
			static const uint32_t IsInviteRequired = 0x04;	
			static const uint32_t IsVoiceEnabled = 0x10000;	
		};

		struct FlagMasks
		{
			static const uint32_t HasWebcam = 0x00000010;
			static const uint32_t Gender = 0x00018000;
		};

		struct Gender
		{
			static const uint32_t Unspeficied = 0x00000000;
			static const uint32_t Male = 0x00008000;
			static const uint32_t Female= 0x00010000;
		};

		enum StatusCode
		{
			TooManyJoins =		-36,	//	You are trying to join too many rooms at the same time
			RoomFull =			-35,	//	Room is full
			RoomIsFull =		-34,	//	This room is full
			PrivateRoom =		-33,	//	You cannot join a private room. You must be invited to this room
			UserRefusedJoin =	-32,	//	User refused to join
			AuditoriumOnly =	-16,	//	Command can only use used in an auditorium
			UserDoesNotExist =	-15,	//	Specified user does not exist
			UsernameTooLong =	-14,	//	Username too long
			RoomnameTooLong =	-13,	//	The room name is too long, please choose a shorter one
			ContactNotInRoom =	-11,	//	User is not in the chatroom
			RequestTooLong =	-10,	//	Request too long
			VoiceNotAvailable =	 -9,	//	Voice chat not available
			ClientNotInRoom =	 -7,	//	You have to be in the room to talk
			InvalidCategory =	 -6,	//	Invalid category or room
			CreateRoom =		 -5,	//	Create room
			RequestRefused =	 -4,	//	The request was refused
			CommProblem =		 -3,	//	Communications problem
			Success =			  0,	//	Success
			AlreadyInRoom =		 16,	//	Already in the room
		};

		static const serviceid_type ServiceId = 152;
		//	FIXME: There are two other attributes.
		//		5 - data is split across multiple packets.
		//		-1 - Failed. See status codes above
		static const attribute_type AttributeId = 1;
	};

}}}}}
