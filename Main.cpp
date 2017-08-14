#include "PresiderServer.h"

#include <iostream>

#if   defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
	#define OS_WINDOWS
	#define WIN32_MEAN_AND_LEAN
	#include <Windows.h>
#elif defined(__linux__) || defined(linux) || defined(__linux)
	#define OS_LINUX
	#include <unistd.h>
#else
	#error Operating System Not Supported
#endif

int main(int ArgumentCount, const char* Arguments)
{
	int SleepMs = 1000;

	if (ArgumentCount > 0)
	{
		//SleepMs = std::atoi(Arguments);
	}

	PresiderServer* Server = new PresiderServer;

	if (Server->Initialize())
	{
		std::cout << "Host Name: " << Server->GetHostName() << "\n";
		std::cout << "Reserved Port: 735\n";
		std::cout << "Update Time: " << SleepMs << "ms\n";
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

#ifdef OS_WINDOWS
		Sleep(SleepMs);
#elif defined OS_LINUX
		usleep(SleepMs * 1000)
#endif
	}

	return 0;
}