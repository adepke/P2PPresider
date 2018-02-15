#pragma once

#include <string>

// API Specific Includes
#if OS_WINDOWS
	#include <WinSock2.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <sys/ioctl.h>

	typedef int SOCKET;
#endif

class PresiderServer
{
protected:
	
#if OS_WINDOWS
	WSAData Data;
#endif
	SOCKET Socket;

	sockaddr_in WaitingClient;

public:
	PresiderServer() {}
	virtual ~PresiderServer() {}

	virtual bool Initialize();
	virtual bool Update();
	virtual void Shutdown();

	virtual std::string GetHostName();
};
