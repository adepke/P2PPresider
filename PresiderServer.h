#pragma once

#include <string>

#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>

class PresiderServer
{
protected:
	WSAData Data;
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