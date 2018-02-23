//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/DispatchMaps.h>
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
#include <Protocols/YMsg/SharedPtrAsRefArg.h>



namespace Giblet { namespace Protocols { namespace YMsg { namespace DispatchMaps
{

	namespace
	{
		template<class ProcessorType_>
		void AddProcessor(std::shared_ptr<SessionContext> session, PacketDispatcher::container_type& processors)
		{
			using namespace std::placeholders;

			const auto serviceId(typename ProcessorType_::payload_type::ServiceId);
			processors.emplace(make_pair(
				serviceId,
				bind(&ProcessorType_::Parse, ProcessorType_(), Utility::MakeSharedPtrAsRefArg(session), _4, _5, _6)));
		}
	}



	V11::container_type V11::Create(std::shared_ptr<SessionContext> session)
	{
		container_type processors;

		namespace Processors = Server::Processors;

		//make_pair(static_cast<uint16_t>(21), bind(&IPacketProcessor::Parse, PacketProcessor<NullPacketParser>>(session, processors);		//	Set IMVironment configuration/settings
		//make_pair(static_cast<uint16_t>(22), bind(&IPacketProcessor::Parse, PacketProcessor<NullPacketParser>>(session, processors);		//	Set configuration/settings
		AddProcessor<PacketProcessor<Processors::RequestProfile>>(session, processors);
		//{10, std::make_shared<NullPacketParser>();		//	Add identify/update profile/something
		//{138, std::make_shared<NullPacketParser>();	//	Ping
		//	TODO: System message (Builder only)

		AddProcessor<PacketProcessor<Processors::SetAway>>(session, processors);
		AddProcessor<PacketProcessor<Processors::SetAvailable>>(session, processors);
		//	TODO: set idle status (the set idle service id is no longer used and idle is handled in setaway
		AddProcessor<PacketProcessor<Processors::ActivateProfile>>(session, processors);
		AddProcessor<PacketProcessor<Processors::DeactivateProfile>>(session, processors);
		AddProcessor<PacketProcessor<Processors::AddContact>>(session, processors);
		AddProcessor<PacketProcessor<Processors::RenameContactGroup>>(session, processors);
		//	TODO: Move contact to group
		AddProcessor<PacketProcessor<Processors::RemoveContact>>(session, processors);
		//	TODO: Accept contact add
		AddProcessor<PacketProcessor<Processors::DenyContactAddRequest>>(session, processors);
		AddProcessor<PacketProcessor<Processors::ChangeBlockedUser>>(session, processors);

		AddProcessor<PacketProcessor<Processors::PrivateMessage>>(session, processors);
		AddProcessor<PacketProcessor<Processors::MassPrivateMessage>>(session, processors);

		//	TODO: Voice chat request (defer)
		//	TODO: voice chat request deny (defer)
		AddProcessor<PacketProcessor<Processors::ServiceOperation>>(session, processors);
		AddProcessor<PacketProcessor<Processors::ChatLogin>>(session, processors);
		AddProcessor<PacketProcessor<Processors::ChatJoin>>(session, processors);
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

		return processors;
	}


}}}}


