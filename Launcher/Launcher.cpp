//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include <YIMPatch/public.h>
#include <iostream>
#include <string>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")


void InjectDLL(std::string targetDLLPath, HANDLE targetProcess)
{
	targetDLLPath.push_back('\0');

	// 1. Allocate memory in the remote process for szLibPath
	// 2. Write szLibPath to the allocated memory
	auto remoteBuffer = ::VirtualAllocEx(targetProcess, nullptr, targetDLLPath.size(), MEM_COMMIT, PAGE_READWRITE);
	::WriteProcessMemory(targetProcess, remoteBuffer, targetDLLPath.c_str(), targetDLLPath.size(), nullptr);


	// Load "LibSpy.dll" into the remote process
	// (via CreateRemoteThread & LoadLibrary)
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

int main()
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

	char filenameBuffer[_MAX_PATH];
	::GetModuleFileNameA(nullptr, filenameBuffer, sizeof(filenameBuffer));
	::PathRemoveFileSpecA(filenameBuffer);

	InjectDLL(std::string(filenameBuffer) + "/YIMPatch.dll", processInformation.hProcess);

	ResumeThread(processInformation.hThread);
	::WaitForSingleObject(processInformation.hThread, INFINITE);

	return 0;
}

