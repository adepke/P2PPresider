#include "PresiderServer.h"

#if OS_WINDOWS
	#pragma comment(lib, "Ws2_32.lib")

	#include <Ws2tcpip.h>
#else
	#define INVALID_SOCKET -1
#endif

#include <iostream>

bool PresiderServer::Initialize()
{
#if OS_WINDOWS
	if (WSAStartup(0x0202, &Data) == SOCKET_ERROR)
	{
		return false;
	}
#endif
	
	Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (Socket == INVALID_SOCKET)
	{
#if OS_WINDOWS
		WSACleanup();
#endif

		return false;
	}

	int ValueA = 1;

	if (setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&ValueA, sizeof(ValueA)) != 0)
	{
		Shutdown();

		return false;
	}

	sockaddr_in SocketAddress;
	ZeroMemory(&SocketAddress, sizeof(SocketAddress));
	SocketAddress.sin_family = AF_INET;
	SocketAddress.sin_addr.s_addr = INADDR_ANY;
	SocketAddress.sin_port = htons(735);

	if (bind(Socket, (sockaddr*)&SocketAddress, sizeof(SocketAddress)) != 0)
	{
		Shutdown();

		return false;
	}

	WaitingClient.sin_addr.s_addr = 0;

	return true;
}

bool PresiderServer::Update()
{
	unsigned long ValueB = 0;

	sockaddr_in ClientAddress;
	int Size = sizeof(ClientAddress);

	// Data sent from the client must match the key to confirm that the client actually wants to be linked with a peer.
	char* DataReceived = new char[32];

	// Blocking call to recvfrom(), so we wait here until some data is received from a connecting client.
#if OS_WINDOWS
	int SizeReceived = recvfrom(Socket, DataReceived, 32, 0, (sockaddr*)&ClientAddress, &Size);
#else
	int SizeReceived = recvfrom(Socket, DataReceived, 32, 0, (sockaddr*)&ClientAddress, (socklen_t*)&Size);
#endif

	std::string DataString(DataReceived);

	delete[] DataReceived;

	DataString = DataString.substr(0, SizeReceived);

	if (DataString != "CONNECT")
	{
		// This is not a failure, just a client that does not want to be paired.
		return true;
	}

	// Confirm this isn't the same client reconnecting.
	if (WaitingClient.sin_addr.s_addr != 0)
	{
		if (WaitingClient.sin_addr.s_addr == ClientAddress.sin_addr.s_addr && WaitingClient.sin_port == ClientAddress.sin_port)
		{
			return true;
		}
	}

	char ClientAddressBuffer[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &(ClientAddress.sin_addr.s_addr), ClientAddressBuffer, INET_ADDRSTRLEN);

	std::cout << "Client Queued: " << ClientAddressBuffer << ":" << ntohs(ClientAddress.sin_port) << "\n";

	// If we have a peer that has already connected with the presider.
	if (WaitingClient.sin_addr.s_addr != 0)
	{
		char WaitingClientAddressBuffer[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &(WaitingClient.sin_addr.s_addr), WaitingClientAddressBuffer, INET_ADDRSTRLEN);

		std::cout << "Matching Clients: " << ClientAddressBuffer << ":" << ntohs(ClientAddress.sin_port) << " | " << WaitingClientAddressBuffer << ":" << ntohs(WaitingClient.sin_port) << "\n";

		std::string MessageA;
		MessageA = "PEER";
		MessageA.append(ClientAddressBuffer);
		MessageA += ":";
		MessageA.append(std::to_string(ntohs(ClientAddress.sin_port)));

		if (sendto(Socket, MessageA.c_str(), strlen(MessageA.c_str()) + 1, 0, (const sockaddr*)&WaitingClient, sizeof(WaitingClient)) <= 0)
		{
			Shutdown();

			return false;
		}

		std::string MessageB;
		MessageB = "PEER";
		MessageB.append(WaitingClientAddressBuffer);
		MessageB += ":";
		MessageB.append(std::to_string(ntohs(WaitingClient.sin_port)));

		if (sendto(Socket, MessageB.c_str(), strlen(MessageB.c_str()) + 1, 0, (const sockaddr*)&ClientAddress, sizeof(ClientAddress)) <= 0)
		{
			Shutdown();

			return false;
		}

		WaitingClient.sin_addr.s_addr = 0;
	}

	else
	{
		WaitingClient = ClientAddress;
	}

	return true;
}

void PresiderServer::Shutdown()
{
	shutdown(Socket, 2);
	
#if OS_WINDOWS
	closesocket(Socket);
	WSACleanup();
#else
	close(Socket);
#endif
}

std::string PresiderServer::GetHostName()
{
	char NameBuffer[256];

	gethostname(NameBuffer, 256);

	return std::string(NameBuffer);
}
