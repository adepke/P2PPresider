#include "PresiderServer.h"

#include <iostream>

int main(int ArgumentCount, char** Arguments)
{
	PresiderServer* Server = new PresiderServer;

	if (Server->Initialize())
	{
		std::cout << "Host Name: " << Server->GetHostName() << "\n";
		std::cout << "Reserved Port: 735\n";
		std::cout << "\nServer is Online...\n";
	}

	else
	{
		std::cout << "Server Initialization Failure\n";

		system("pause");
		return -1;
	}

	while (true)
	{
		if (!Server->Update())
		{
			Server->Shutdown();

			return -1;
		}
	}

	return 0;
}