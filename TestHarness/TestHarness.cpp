//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <WinSock2.h>
#include <Protocols/YMsg/ProtocolStream.h>
#include <YahooWeb/MultilHostWebServer.h>
#include <YahooWeb/YahooInsiderWebServer.h>
#include <YahooWeb/MyYahooWebServer.h>
#include <YahooWeb/Utilities.h>
#include <Protocols/YMsg/Server/Processors/ProtocolSync.h>
#include <Protocols/YMsg/Server/Processors/ChallengeRequest.h>
#include <Protocols/YMsg/Server/Processors/Authentication.h>
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
#include <Protocols/YMsg/Server/Builders/ProtocolAck.h>
#include <Protocols/YMsg/Server/Builders/ChallengeResponse.h>
#include <Protocols/YMsg/Server/Payloads/Authentication.h>
#include <Protocols/YMsg/MockBlockedContactManagementLink.h>
#include <Protocols/YMsg/MockContactManagementLink.h>
#include <Protocols/YMsg/MockPresenceManagementLink.h>
#include <Protocols/YMsg/MockProfileManagementLink.h>
#include <Protocols/YMsg/MockSessionContext.h>
#include <Protocols/YMsg/PacketDispatcher.h>
#include <mongoose.h>
#include <iostream>
#include <map>
#include <thread>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "ws2_32.lib")

void HTTPServerThread();

void InjectDLL(std::string targetDLLPath, HANDLE targetProcess)
{
	targetDLLPath.push_back('\0');

	// 1. Allocate memory in the remote process for szLibPath
	// 2. Write szLibPath to the allocated memory
	auto remoteBuffer = ::VirtualAllocEx(targetProcess, nullptr, targetDLLPath.size(), MEM_COMMIT, PAGE_READWRITE);
	::WriteProcessMemory(targetProcess, remoteBuffer, targetDLLPath.c_str(), targetDLLPath.size(), nullptr);


	// Load DLL into the remote process (via CreateRemoteThread & LoadLibrary)
	auto remoteThread = ::CreateRemoteThread(
		targetProcess,
		nullptr,
		0,
		(LPTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandleA("Kernel32"), "LoadLibraryA"),
		remoteBuffer,
		0,
		nullptr);

	::WaitForSingleObject(remoteThread, INFINITE);

	// Get handle of the loaded module
	DWORD hLibModule;   // Base address of loaded module (==HMODULE);
	::GetExitCodeThread(remoteThread, &hLibModule);

	// Clean up
	::CloseHandle(remoteThread);
	::VirtualFreeEx(targetProcess, remoteBuffer, targetDLLPath.size(), MEM_RELEASE);
}


HANDLE LaunchMessenger()
{
	const std::wstring applicationPath1(L"C:\\Program Files (x86)\\Yahoo!\\Messenger\\YahooMessenger.exe");
	const std::wstring applicationPath2(L"C:\\Program Files (x86)\\Yahoo!\\Messenger\\ypager.exe");
	STARTUPINFO startupInfo{ 0 };
	PROCESS_INFORMATION processInformation{ 0 };

	startupInfo.cb = sizeof(startupInfo);
	BOOL createResult = CreateProcess(
		applicationPath1.c_str(),
		nullptr,
		nullptr,
		nullptr,
		FALSE,
		CREATE_SUSPENDED,
		nullptr,
		nullptr,
		&startupInfo,
		&processInformation);
	if (createResult == FALSE)
	{
		createResult = CreateProcess(
			applicationPath2.c_str(),
			nullptr,
			nullptr,
			nullptr,
			FALSE,
			CREATE_SUSPENDED,
			nullptr,
			nullptr,
			&startupInfo,
			&processInformation);
	}


	if (createResult == FALSE)
	{
		std::cerr << "Unable to start process\n";
		return nullptr;
	}

	char filenameBuffer[_MAX_PATH];
	::GetModuleFileNameA(nullptr, filenameBuffer, sizeof(filenameBuffer));
	::PathRemoveFileSpecA(filenameBuffer);

	InjectDLL(std::string(filenameBuffer) + "/YIMPatch.dll", processInformation.hProcess);

	ResumeThread(processInformation.hThread);

	return processInformation.hProcess;
}



class MockingPacketDispatcher : public Giblet::Protocols::YMsg::PacketDispatcher<Giblet::Protocols::YMsg::SessionContext>
{
public:

	const std::shared_ptr<Giblet::Protocols::YMsg::MockSessionContext> session_;

	explicit MockingPacketDispatcher(std::shared_ptr<Giblet::Protocols::YMsg::MockSessionContext> session)
		: session_(session)
	{
		using namespace Giblet::Protocols::YMsg;
		using namespace Giblet::Protocols::YMsg::Server;

		container_type processors;

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

	template<class ProcessorType_>
	static void AddProcessor(container_type& processors)
	{
		using namespace std::placeholders;

		processors.emplace(make_pair(
			typename ProcessorType_::payload_type::ServiceId,
			bind(&ProcessorType_::Parse, ProcessorType_(), _1, _2, _3, _4)));
	}

};



class AuthenticationContext
{
public:
	using string_type = std::string;
	using availability_type = Giblet::Protocols::YMsg::detail::Availability;

	string_type			clientId_;
	availability_type	initialAvailability_ = availability_type::Offline;
};


class AuthenticationPacketDispatcher : public Giblet::Protocols::YMsg::PacketDispatcher<AuthenticationContext>
{
public:

	AuthenticationPacketDispatcher(
		std::shared_ptr<Giblet::Protocols::YMsg::ClientConnection> connection,
		std::weak_ptr<Giblet::Protocols::YMsg::ProtocolStream> protocolStream)
		:
		PacketDispatcher(CreateProcessorMap()),
		connection_(connection),
		protocolStream_(protocolStream)
	{}


protected:

	virtual void OnProtocolSync(
		context_type& context,
		const header_type& header,
		const_iterator payloadBegin,
		const_iterator payloadEnd)
	{
		Giblet::Protocols::YMsg::Server::Payloads::ProtocolSync payload;
		Giblet::Protocols::YMsg::PacketParser parser;
		parser.Parse(payload, header, payloadBegin, payloadEnd);

		connection_->SetProtocolVersion(header.protoVersion);

		//	Acknowledge the sync
		Giblet::Protocols::YMsg::Server::Builders::ProtocolAck builder;
		builder.Build(*connection_);
		builder.Send(*connection_);
	}


	virtual void OnChallengeRequest(
		context_type& context,
		const header_type& header,
		const_iterator payloadBegin,
		const_iterator payloadEnd)
	{
		Giblet::Protocols::YMsg::Server::Payloads::ChallengeRequest payload;
		Giblet::Protocols::YMsg::PacketParser parser;
		parser.Parse(payload, header, payloadBegin, payloadEnd);

		context.clientId_ = payload.clientId;
		//	We don't use any authentication at the moment so we just use method 0
		//	which causes messenger to omit credentials
		Giblet::Protocols::YMsg::Server::Builders::ChallengeResponse builder;
		builder.Build(*connection_, payload.clientId);
		builder.Send(*connection_);
	}


	virtual void OnAuthenticate(
		context_type& context,
		const header_type& header,
		const_iterator payloadBegin,
		const_iterator payloadEnd)
	{
		Giblet::Protocols::YMsg::Server::Payloads::Authentication payload;
		Giblet::Protocols::YMsg::PacketParser parser;
		parser.Parse(payload, header, payloadBegin, payloadEnd);

		if (payload.clientId1_ != context.clientId_)
		{
			//	FIXME: send error and disconnect
			//	connection_->close();
		}

		using availability_type = context_type::availability_type;

		//	FIXME: Need to define attributes the same way we do keys
		context.initialAvailability_ =
			header.attribute == static_cast<std::underlying_type<availability_type>::type>(availability_type::Invisible) 
			? availability_type::Offline
			: availability_type::Available;

		OnClientAuthenticated(context);
	}


	virtual void OnClientAuthenticated(context_type& context)
	{
		//	Get rid of the processors so no incoming packets from the client are processed
		//	until authentication has completed.
		processors_.clear();

		//	connect to [XMPP] server

		//	authenticate with server

		//	retrieve profile

		//	retrieve roster/buddy list

		//	Everything complete
		OnSessionAuthenticated(context);
	}

	virtual void OnSessionAuthenticated(context_type& context) = 0;

private:

	container_type CreateProcessorMap()
	{
		using namespace Giblet::Protocols::YMsg::Server;
		using namespace std::placeholders;

		return
		{
			{Payloads::ProtocolSync::ServiceId, bind(&AuthenticationPacketDispatcher::OnProtocolSync, this, _1, _2, _3, _4)},
			{Payloads::ChallengeRequest::ServiceId, bind(&AuthenticationPacketDispatcher::OnChallengeRequest, this, _1, _2, _3, _4)},
			{Payloads::Authentication::ServiceId, bind(&AuthenticationPacketDispatcher::OnAuthenticate, this, _1, _2, _3, _4)},
		};
		;
	}


protected:

	const std::shared_ptr<Giblet::Protocols::YMsg::ClientConnection> connection_;
	const std::weak_ptr<Giblet::Protocols::YMsg::ProtocolStream> protocolStream_;
};


class MockAuthenticationPacketDispatcher : public AuthenticationPacketDispatcher
{
public:

	using AuthenticationPacketDispatcher::AuthenticationPacketDispatcher;

	void OnSessionAuthenticated(context_type& context) override
	{
		const auto profileManager(std::make_shared<Giblet::Protocols::YMsg::ProfileManager>());
		const auto profileManagementEvents(std::make_shared<Giblet::Protocols::YMsg::ProfileManagementEvents>(connection_, profileManager));
		const auto profileManagement(std::make_shared<Giblet::Protocols::YMsg::MockProfileManagementLink>(profileManager, profileManagementEvents));
		const auto blockedContactManager(std::make_shared<Giblet::Protocols::YMsg::BlockedContactManager>());
		const auto blockedContactManagementEvents(std::make_shared<Giblet::Protocols::YMsg::BlockedContactManagementEvents>(connection_, blockedContactManager));
		const auto blockedContactManagementLink(std::make_shared<Giblet::Protocols::YMsg::MockBlockedContactManagementLink>(blockedContactManager, blockedContactManagementEvents));
		const auto contactManager(std::make_shared<Giblet::Protocols::YMsg::ContactManager>());
		const auto contactManagementEvents(std::make_shared<Giblet::Protocols::YMsg::ContactManagementEvents>(connection_, blockedContactManager, profileManager, contactManager));
		const auto contactManagement(std::make_shared<Giblet::Protocols::YMsg::MockContactManagementLink>(blockedContactManager, profileManager, contactManager, contactManagementEvents));
		const auto presenceEvents(std::make_shared<Giblet::Protocols::YMsg::PresenceEvents>(connection_, contactManager));
		const auto presenceManagementLink(std::make_shared<Giblet::Protocols::YMsg::MockPresenceManagementLink>(profileManager, contactManager, presenceEvents, context.initialAvailability_));
		const auto session(std::make_shared<Giblet::Protocols::YMsg::MockSessionContext>(connection_, blockedContactManager, blockedContactManagementLink, profileManager, profileManagement, contactManager, contactManagement, presenceManagementLink));

		using ContactInfo = Giblet::Protocols::YMsg::ContactInfo;
		profileManager->Load(context.clientId_, { "winky", "dinky", "blinky", "brad", "greg" });
		contactManager->LoadContact(ContactInfo("jay", "Friends", ContactInfo::Linked));
		contactManager->LoadContact(ContactInfo("donna", "Friends", ContactInfo::Linked, ContactInfo::availability_type::Custom, false, "Listening to Bewiz - Booters Paradise"));
		contactManager->LoadContact(ContactInfo("david", "Friends", ContactInfo::Linked, ContactInfo::availability_type::Busy, true));
		contactManager->LoadContact(ContactInfo("winky", "Myself", ContactInfo::Linked, context.initialAvailability_));
		contactManager->LoadContact(ContactInfo("dinky", "Myself", ContactInfo::Linked, context.initialAvailability_));
		contactManager->LoadContact(ContactInfo("blinky", "Myself", ContactInfo::Linked, context.initialAvailability_));
		contactManager->LoadContact(ContactInfo("brad", "Myself", ContactInfo::Linked, context.initialAvailability_));
		blockedContactManager->Load({ "wierdo", "jerk", "germinator" });


		//	Start the session
		connection_->SetSessionId(rand());
		session->BeginSession();

		//	Bind up our dispatcher
		auto dispatcherFunc(std::bind(
			&MockingPacketDispatcher::Dispatch,
			std::make_shared<MockingPacketDispatcher>(session),
			std::reference_wrapper<MockingPacketDispatcher::context_type>(*session),
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3));

		//	FIXME: We will be destroyed (along with context) during the call to SetDispatcher. Not ideal!
		auto stream(protocolStream_.lock());
		if (stream)
		{
			stream->SetDispatcher(dispatcherFunc);
		}
	}

};



void YMSGSessionThread(SOCKET clientSocket)
{
	const auto connection(std::make_shared<Giblet::Protocols::YMsg::ClientConnection>(clientSocket));

	const auto inputStream(std::make_shared<Giblet::Protocols::YMsg::ProtocolStream>(connection));

	inputStream->SetDispatcher(std::bind(
		&AuthenticationPacketDispatcher::Dispatch,
		std::make_shared<MockAuthenticationPacketDispatcher>(connection, inputStream),
		AuthenticationPacketDispatcher::context_type(),
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3));

	for (bool run(true); run;)
	{
		fd_set readfds;
		timeval timeout;

		FD_ZERO(&readfds);
		FD_SET(clientSocket, &readfds);

		timeout.tv_sec = 0;
		timeout.tv_usec = 50000;

		auto result = select(1, &readfds, nullptr, nullptr, &timeout);
		if (result > 0)
		{
			if (!inputStream->ProcessPendingData())
			{
				std::cerr << "Unable to retrieve number of bytes available on socket\n";
				return;
			}
		}
	}
}




void YMSGListenerThread(SOCKET listener)
{
	do
	{
		auto clientSocket = accept(listener, nullptr, nullptr);
		if (clientSocket == INVALID_SOCKET)
		{
			break;
		}

		std::thread(YMSGSessionThread, clientSocket).detach();
	}
	while (true);

	if (closesocket(listener) == SOCKET_ERROR)
	{
		std::cerr << "closesocket function failed with error " << WSAGetLastError() << '\n';
	}
}




void HTTPServerThread()
{
	using namespace std::placeholders;

	auto gateway(std::make_shared<Giblet::Protocols::YMsg::SessionGateway>());
	Giblet::YahooWeb::MultilHostWebServer serverHost(gateway);

	//	Add our generic handler for the lcoal host
	serverHost.Add("localhost", std::bind(Giblet::YahooWeb::GenericHandler, _1, "htdoc"));

	//	Add our handler for the chat content server
	serverHost.Add("chat.yahoo.com.localhost", std::bind(Giblet::YahooWeb::GenericHandler, _1, "htdoc/chat.yahoo.com"));

	//	Add our handler for the yahoo insider
	Giblet::YahooWeb::YahooInsiderWebServer yahooInsiderWebServer(gateway);
	serverHost.Add("insider.msg.yahoo.com.localhost", yahooInsiderWebServer);

	//
	Giblet::YahooWeb::MyYahooWebServer myYahooWebServer(gateway, "htdoc/my.yahoo.com");
	serverHost.Add("my.yahoo.com.localhost", myYahooWebServer);


	struct mg_server *server = mg_create_server(&serverHost);
	mg_set_option(server, "document_root", ".");
	mg_set_option(server, "listening_port", "41337");
	mg_add_uri_handler(server, "/", Giblet::YahooWeb::MultilHostWebServer::ConnectionCallback);
	for (;;) mg_poll_server(server, 1000);  // Infinite loop, Ctrl-C to stop
	mg_destroy_server(&server);
}




int main()
{
	WSADATA wsaData{ 0 };
	auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		std::cerr << "WSAStartup() failed with error: " << result << '\n';
		return EXIT_FAILURE;
	}

	std::thread(HTTPServerThread).detach();

	//----------------------
	// Create a SOCKET for listening for incoming connection requests.
	auto listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listener == INVALID_SOCKET)
	{
		std::cerr << "socket function failed with error: " <<  WSAGetLastError() << '\n';
		WSACleanup();
		return EXIT_FAILURE;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in service;

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = ADDR_ANY;
	service.sin_port = htons(5050);

	result = bind(listener, reinterpret_cast<SOCKADDR*>(&service), sizeof(service));
	if (result == SOCKET_ERROR)
	{
		std::cerr << "bind function failed with error " << WSAGetLastError() << '\n';
		result = closesocket(listener);
		if (result == SOCKET_ERROR)
		{
			std::cerr << "closesocket function failed with error " << WSAGetLastError() << '\n';
		}
		WSACleanup();
		return EXIT_FAILURE;
	}
	//----------------------
	// Listen for incoming connection requests 
	// on the created socket
	if (listen(listener, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "listen function failed with error: " << WSAGetLastError() << '\n';
	}
	
	std::cerr << "Listening on socket...\n";
	std::thread(YMSGListenerThread, listener).detach();
	std::cerr << "Launching messenger...\n";
	auto messengerHandle(LaunchMessenger());
	std::cerr << "Waiting...\n";
	::WaitForSingleObject(messengerHandle, INFINITE);


	WSACleanup();

	return EXIT_SUCCESS;
}

