#include <Windows.h>
#include <iostream>
#include <safetyhook.hpp>
#include <iomanip>

#include "kiero/kiero.h"
#include "Stuffs/Manager.hpp"
#include "Offsets.h"

bool running = false;

Manager* manager;

DWORD WINAPI initClient(LPVOID lpParameter) {
	if (!running) {
		running = true;

		AllocConsole();
		FILE* fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);

		manager = new Manager();
	}

	Sleep(25);

	kiero::shutdown();

	Sleep(25);

	FreeLibraryAndExitThread((HMODULE)lpParameter, 1);
}

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hDLL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)initClient, hDLL, 0, 0);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

