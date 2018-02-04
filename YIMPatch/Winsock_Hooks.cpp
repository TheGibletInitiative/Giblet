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

#pragma comment(lib, "ws2_32.lib")


namespace
{
	LPFN_CONNECT original_connect = NULL;

	int WSAAPI proxy_connect(SOCKET s, const struct sockaddr* name, int namelen)
	{
		std::cerr << "[connect] -> " << name->sa_family << "\n";

		if (AF_INET == name->sa_family)
		{
			auto address = *reinterpret_cast<const sockaddr_in*>(name);

			std::cerr << 
				("Processing connection for Yahoo! Messenger to "
					+ std::to_string(address.sin_addr.S_un.S_un_b.s_b1)
					+ '.' + std::to_string(address.sin_addr.S_un.S_un_b.s_b2)
					+ '.' + std::to_string(address.sin_addr.S_un.S_un_b.s_b3)
					+ '.' + std::to_string(address.sin_addr.S_un.S_un_b.s_b4)
					+ ':' + std::to_string(ntohs(address.sin_port))
					+ '\n');

			if (INADDR_LOOPBACK != ntohl(address.sin_addr.s_addr))
			{
				if (address.sin_addr.S_un.S_un_b.s_b1 == 101
					&& address.sin_addr.S_un.S_un_b.s_b2 == 101
					&& address.sin_addr.S_un.S_un_b.s_b3 == 101
					&& address.sin_addr.S_un.S_un_b.s_b4 == 101)
				{
					std::cerr << "Redirecting connection to localhost\n";

					address.sin_addr.S_un.S_un_b.s_b1 = 127;
					address.sin_addr.S_un.S_un_b.s_b2 = 0;
					address.sin_addr.S_un.S_un_b.s_b3 = 0;
					address.sin_addr.S_un.S_un_b.s_b4 = 1;
				}
			}

			return original_connect(s, reinterpret_cast<const sockaddr*>(&address), sizeof(address));
		}

		return original_connect(s, name, namelen);
	}

}



void Hook_Winsock()
{
	Giblet::CodeHook::x86::InstallHook(
		"ws2_32.dll",
		"connect",
		proxy_connect,
		(void**)&original_connect);
}

