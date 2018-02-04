//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include <CodeHook/cpuIA32.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>


namespace
{
	typedef
		HANDLE(WINAPI* LPFN_CREATEFILEA)(
			LPCSTR lpFileName,
			DWORD dwDesiredAccess,
			DWORD dwShareMode,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			DWORD dwCreationDisposition,
			DWORD dwFlagsAndAttributes,
			HANDLE hTemplateFile);

	typedef
		HANDLE(WINAPI* LPFN_CREATEFILEW)(
			LPCWSTR lpFileName,
			DWORD dwDesiredAccess,
			DWORD dwShareMode,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			DWORD dwCreationDisposition,
			DWORD dwFlagsAndAttributes,
			HANDLE hTemplateFile);


	LPFN_CREATEFILEA original_CreateFileA = nullptr;
	LPFN_CREATEFILEW original_CreateFileW = nullptr;


	HANDLE WINAPI proxy_CreateFileA(
		LPCSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile)
	{
		std::cerr << (std::string("Opening file - ") + lpFileName + "\n");

		return original_CreateFileA(
			lpFileName,
			dwDesiredAccess,
			dwShareMode,
			lpSecurityAttributes,
			dwCreationDisposition,
			dwFlagsAndAttributes,
			hTemplateFile);
	}


	HANDLE WINAPI proxy_CreateFileW(
		LPCWSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile)
	{
		std::wcerr << (std::wstring(L"Opening file - ") + lpFileName + L"\n");

		return original_CreateFileW(
			lpFileName,
			dwDesiredAccess,
			dwShareMode,
			lpSecurityAttributes,
			dwCreationDisposition,
			dwFlagsAndAttributes,
			hTemplateFile);
	}

}


void Hook_CreateFile()
{
	Giblet::CodeHook::x86::InstallHook(
		"kernel32.dll",
		"CreateFileA",
		proxy_CreateFileA,
		(void**)&original_CreateFileA);

	Giblet::CodeHook::x86::InstallHook(
		"kernel32.dll",
		"CreateFileW",
		proxy_CreateFileW,
		(void**)&original_CreateFileW);
}
