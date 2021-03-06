//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <WinSock2.h>
#include <YahooWeb/MultilHostWebServer.h>
#include <YahooWeb/YahooInsiderWebServer.h>
#include <YahooWeb/MyYahooWebServer.h>
#include <YahooWeb/Utilities.h>
#include <Protocols/YMsgMock/AuthenticationHandler.h>
#include <GlooxXMPPBridgeLibrary/AuthenticationHandler.h>
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



void YMSGSessionThread(SOCKET clientSocket)
{
	using namespace Giblet::Protocols::YMsg;
	using namespace std::placeholders;


	const auto connection(std::make_shared<ClientConnection>(clientSocket));
	const auto handler(std::make_shared<MockAuthenticationHandler>(connection));
	//const auto handler(std::make_shared<Giblet::Bridges::Gloox::AuthenticationHandler>(connection));
	auto dispatcher(std::bind(
		&PacketDispatcher::Dispatch,
		std::make_shared<PacketDispatcher>(handler->CreateDispatchMap(handler)),
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		std::placeholders::_4,
		std::placeholders::_5));
	const auto inputStream(std::make_shared<ProtocolStream>(dispatcher));


	ConnectionPump pump;
	pump.AttachSocket(clientSocket, std::bind(&ProtocolStream::ProcessPendingData, inputStream, _1, _2, _3));
	pump.Pump();
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

