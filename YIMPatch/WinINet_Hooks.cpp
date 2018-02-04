//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include <CodeHook/cpuIA32.h>
#include <iostream>
#include <string>
#include <Wininet.h>

#pragma comment(lib, "Wininet.lib")


namespace
{

	typedef HINTERNET(STDAPICALLTYPE* LPFN_InternetConnectA)(
		_In_ HINTERNET hInternet,
		_In_ LPCSTR lpszServerName,
		_In_ INTERNET_PORT nServerPort,
		_In_opt_ LPCSTR lpszUserName,
		_In_opt_ LPCSTR lpszPassword,
		_In_ DWORD dwService,
		_In_ DWORD dwFlags,
		_In_opt_ DWORD_PTR dwContext);

	typedef HINTERNET(STDAPICALLTYPE* LPFN_InternetConnectW)(
		_In_ HINTERNET hInternet,
		_In_ LPCWSTR lpszServerName,
		_In_ INTERNET_PORT nServerPort,
		_In_opt_ LPCWSTR lpszUserName,
		_In_opt_ LPCWSTR lpszPassword,
		_In_ DWORD dwService,
		_In_ DWORD dwFlags,
		_In_opt_ DWORD_PTR dwContext);


	LPFN_InternetConnectA original_InternetConnectA = nullptr;
	LPFN_InternetConnectW original_InternetConnectW = nullptr;


	HINTERNET STDAPICALLTYPE proxy_InternetConnectA(
		_In_ HINTERNET hInternet,
		_In_ LPCSTR lpszServerName,
		_In_ INTERNET_PORT nServerPort,
		_In_opt_ LPCSTR lpszUserName,
		_In_opt_ LPCSTR lpszPassword,
		_In_ DWORD dwService,
		_In_ DWORD dwFlags,
		_In_opt_ DWORD_PTR dwContext)
	{
		const std::string addresss[] =
		{
				"insider.msg.yahoo.com",
				"pgq.yahoo.com",
				"chat.yahoo.com",
				"calendar.yahoo.com",
				"my.yahoo.com",
				"address.yahoo.com"
		};

		std::cerr << "InternetConnectA - " << lpszServerName << "\n";
		std::string newHostname;
		if (std::find(begin(addresss), end(addresss), lpszServerName) != end(addresss))
		{
			newHostname = lpszServerName;
			newHostname += ".localhost";

			std::cerr << "Redirecting " << lpszServerName << " to " << newHostname << ":41337\n";
			lpszServerName = newHostname.c_str();
			nServerPort = 41337;
		}

		return original_InternetConnectA(
			hInternet,
			lpszServerName,
			nServerPort,
			lpszUserName,
			lpszPassword,
			dwService,
			dwFlags,
			dwContext);
	}


	HINTERNET STDAPICALLTYPE proxy_InternetConnectW(
		_In_ HINTERNET hInternet,
		_In_ LPCWSTR lpszServerName,
		_In_ INTERNET_PORT nServerPort,
		_In_opt_ LPCWSTR lpszUserName,
		_In_opt_ LPCWSTR lpszPassword,
		_In_ DWORD dwService,
		_In_ DWORD dwFlags,
		_In_opt_ DWORD_PTR dwContext)
	{
		const std::wstring addresss[] =
		{
			L"insider.msg.yahoo.com",
			L"pgq.yahoo.com",
			L"chat.yahoo.com",
			L"calendar.yahoo.com",
			L"my.yahoo.com",
			L"address.yahoo.com"
		};

		std::wcerr << L"InternetConnectW - " << lpszServerName << L"\n";
		std::wstring newHostname;
		if (std::find(begin(addresss), end(addresss), lpszServerName) != end(addresss))
		{
			newHostname = lpszServerName;
			newHostname += L".localhost";

			std::wcerr << L"Redirecting " << lpszServerName << L" to " << newHostname << L":41337\n";
			lpszServerName = newHostname.c_str();
			nServerPort = 41337;
		}

		return original_InternetConnectW(
			hInternet,
			lpszServerName,
			nServerPort,
			lpszUserName,
			lpszPassword,
			dwService,
			dwFlags,
			dwContext);
	}

}

void Hook_WinINet()
{
	Giblet::CodeHook::x86::InstallHook(
		"Wininet.dll",
		"InternetConnectA",
		proxy_InternetConnectA,
		(void**)&original_InternetConnectA);

	Giblet::CodeHook::x86::InstallHook(
		"Wininet.dll",
		"InternetConnectW",
		proxy_InternetConnectW,
		(void**)&original_InternetConnectW);
}

