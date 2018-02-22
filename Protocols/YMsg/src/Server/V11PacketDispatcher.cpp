//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/V11PacketDispatcher.h>
#include <Protocols/YMsg/Server/Processors/ChatLogin.h>
#include <Protocols/YMsg/Server/Processors/ChatJoin.h>
#include <Protocols/YMsg/Server/Processors/AddContact.h>
#include <Protocols/YMsg/Server/Processors/ServiceOperation.h>
#include <Protocols/YMsg/Server/Processors/PrivateMessage.h>
#include <Protocols/YMsg/Server/Processors/ChangeBlockedUser.h>
#include <Protocols/YMsg/Server/Processors/RenameContactGroup.h>
#include <Protocols/YMsg/Server/Processors/SetAway.h>
#include <Protocols/YMsg/Server/Processors/SetAvailable.h>
#include <Protocols/YMsg/Server/Processors/ActivateProfile.h>
#include <Protocols/YMsg/Server/Processors/DeactivateProfile.h>
#include <Protocols/YMsg/Server/Processors/DenyContactAddRequest.h>
#include <Protocols/YMsg/Server/Processors/RemoveContact.h>
#include <Protocols/YMsg/Server/Processors/RequestProfile.h>
#include <Protocols/YMsg/Server/Processors/MassPrivateMessage.h>



namespace Giblet { namespace Protocols { namespace YMsg
{

	V11PacketDispatcher::V11PacketDispatcher(std::shared_ptr<SessionContext> session)
		: session_(session)
	{
		container_type processors;

		namespace Processors = Server::Processors;

		//make_pair(static_cast<uint16_t>(21), bind(&IPacketProcessor::Parse, PacketProcessor<NullPacketParser>>(processors);		//	Set IMVironment configuration/settings
		//make_pair(static_cast<uint16_t>(22), bind(&IPacketProcessor::Parse, PacketProcessor<NullPacketParser>>(processors);		//	Set configuration/settings
		AddProcessor<PacketProcessor<Processors::RequestProfile>>(processors);
		//{10, std::make_shared<NullPacketParser>();		//	Add identify/update profile/something
		//{138, std::make_shared<NullPacketParser>();	//	Ping
		//	TODO: System message (Builder only)

		AddProcessor<PacketProcessor<Processors::SetAway>>(processors);
		AddProcessor<PacketProcessor<Processors::SetAvailable>>(processors);
		//	TODO: set idle status (the set idle service id is no longer used and idle is handled in setaway
		AddProcessor<PacketProcessor<Processors::ActivateProfile>>(processors);
		AddProcessor<PacketProcessor<Processors::DeactivateProfile>>(processors);
		AddProcessor<PacketProcessor<Processors::AddContact>>(processors);
		AddProcessor<PacketProcessor<Processors::RenameContactGroup>>(processors);
		//	TODO: Move contact to group
		AddProcessor<PacketProcessor<Processors::RemoveContact>>(processors);
		//	TODO: Accept contact add
		AddProcessor<PacketProcessor<Processors::DenyContactAddRequest>>(processors);
		AddProcessor<PacketProcessor<Processors::ChangeBlockedUser>>(processors);

		AddProcessor<PacketProcessor<Processors::PrivateMessage>>(processors);
		AddProcessor<PacketProcessor<Processors::MassPrivateMessage>>(processors);

		//	TODO: Voice chat request (defer)
		//	TODO: voice chat request deny (defer)
		AddProcessor<PacketProcessor<Processors::ServiceOperation>>(processors);
		AddProcessor<PacketProcessor<Processors::ChatLogin>>(processors);
		AddProcessor<PacketProcessor<Processors::ChatJoin>>(processors);
		//	TODO: Log out of chat
		//	TODO: Create chat room
		//	TODO: Join user in chat room
		//	TODO: Leave chat room
		//	TODO: Invite user to chat
		//	TODO: Decline invite to chat
		//	TODO: Chat message
		//	TODO: Chat ping

		//	TODO: Send contact details (need to be able to edit/retrieve them via insider.msg.yahoo.com)
		//	TODO: Receive contact details (need to be able to edit/retrieve them via insider.msg.yahoo.com)

		processors_ = container_type(begin(processors), end(processors));
	}


}}}

