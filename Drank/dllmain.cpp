#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <safetyhook.hpp>
#include "Utils.hpp"

struct vec3 {
	float x;
	float y;
	float z;
};

bool running = false;

SafetyHookInline worldTick_hook;

void worldTick(uintptr_t* _this) {
	printf("worldTick\n");
	worldTick_hook.thiscall<void, uintptr_t*>(_this);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	if (!running) {
		running = true;

		AllocConsole();
		FILE* fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);

		auto game = Utils::FindSignature("A1 ?? ?? ?? ?? 8B 40 ?? 85 C0 74 ?? 0F 28 ?? ?? EB 07 0F 28 05 ?? ?? ?? ?? 80");
		uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));

		auto basePtr = *(int*)(game + 1);

		auto test = Utils::FindMultiLevelPtr((std::byte*)basePtr, { 0xC, 0x28, 0x54, 0x88, 0xAC, 0x4 });

		std::cout << test << std::endl;

		float* yVel = reinterpret_cast<float*>(test + 0xB4);

		auto addr = BASE + 0x51CB80;

		worldTick_hook = safetyhook::create_inline(reinterpret_cast<void*>(addr), &worldTick);

		while (true) {

			if (GetAsyncKeyState(VK_SPACE)) {
				(*yVel)++;
			}

			Sleep(10);

		}
	}
        
    return TRUE;
}

