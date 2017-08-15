#include "PresiderServer.h"

bool PresiderServer::Initialize()
{
	memset(&WaitingClient, 0, sizeof(WaitingClient));

	if (WSAStartup(0x0202, &Data) == SOCKET_ERROR)
	{
		return false;
	}

	Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (Socket == INVALID_SOCKET)
	{
		WSACleanup();

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
	int SizeReceived = recvfrom(Socket, DataReceived, 4096, 0, (sockaddr*)&ClientAddress, &Size);

	std::string DataString(DataReceived);

	delete[] DataReceived;

	if (DataString != "CONNECT")
	{
		// This is not a failure, just a client that does not want to be paired.
		return true;
	}

	// If we have a peer that has already connected with the presider.
	if (WaitingClient.sin_addr.s_addr != 0)
	{
		std::string MessageA;
		MessageA = "PEER";
		MessageA += std::to_string(ClientAddress.sin_addr.s_addr);
		MessageA += ":";
		MessageA += std::to_string(ClientAddress.sin_port);

		if (sendto(Socket, MessageA.c_str(), strlen(MessageA.c_str()) + 1, 0, (const sockaddr*)&WaitingClient, sizeof(WaitingClient)) <= 0)
		{
			Shutdown();

			return false;
		}

		std::string MessageB;
		MessageB = "PEER";
		MessageB += std::to_string(WaitingClient.sin_addr.s_addr);
		MessageB += ":";
		MessageB += std::to_string(WaitingClient.sin_port);

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
	closesocket(Socket);
	WSACleanup();
}

std::string PresiderServer::GetHostName()
{
	char NameBuffer[256];

	gethostname(NameBuffer, 256);

	return std::string(NameBuffer);
}
