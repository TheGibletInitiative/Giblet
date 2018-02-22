//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include <Protocols/YMsg/ClientConnection.h>
#include <WinSock2.h>
#include <iostream>


namespace Giblet { namespace Protocols { namespace YMsg
{

	void ConnectionPump::AttachSocket(socket_type socket, ondatacallback_type onData)
	{
		onDateCallbacks_[socket] = onData;
	}


	void ConnectionPump::Pump()
	{
		for (bool run(true); run;)
		{
			fd_set readfds;
			timeval timeout;

			FD_ZERO(&readfds);
			for (const auto& socket : onDateCallbacks_)
			{
				FD_SET(static_cast<SOCKET>(socket.first), &readfds);
			}

			timeout.tv_sec = 0;
			timeout.tv_usec = 50000;

			auto result = select(readfds.fd_count, &readfds, nullptr, nullptr, &timeout);
			if (result > 0)
			{
				for (auto i = 0U; i < readfds.fd_count; ++i)
				{
					auto currentSocket(readfds.fd_array[i]);
					auto callbackInfo = onDateCallbacks_.find(currentSocket);
					if (callbackInfo == onDateCallbacks_.end())
					{
						throw std::runtime_error("Unable to find socket callback");
					}

					DWORD incomingSize = 0;
					if (SOCKET_ERROR == ioctlsocket(currentSocket, FIONREAD, &incomingSize))
					{
						std::cerr << "Unable to retrieve number of bytes available on socket\n";
						run = false;
						continue;
					}

					if (!callbackInfo->second(*this, callbackInfo->first, incomingSize))
					{
						onDateCallbacks_.erase(callbackInfo);
					}
				}
			}
		}
	}

}}}
