#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <safetyhook.hpp>
#include "Utils.hpp"
#include "Math.hpp"

bool running = false;

SafetyHookInline worldTick_hook;

SafetyHookMid raycastMid_hook;

SafetyHookMid raycastMidY_hook;
SafetyHookMid raycastMidX_hook;
SafetyHookMid raycastMidZ_hook;

void worldTick(uintptr_t* _this) {
	printf("worldTick\n");
	worldTick_hook.thiscall<void, uintptr_t*>(_this);
}

void raycastMid(safetyhook::Context& ctx) {
	std::cout << "x y z: " << ctx.xmm7.f32[0] << " " << ctx.xmm2.f32[0] << " " << ctx.xmm3.f32[0] << std::endl;
}

struct AimbotInfo {
	std::string entity;
	vec3 forwardVec{};
	float dist;
};

auto aimbotInfo = AimbotInfo{};

bool isASCII(const std::string& str) {
	for (char ch : str) {
		if (static_cast<unsigned char>(ch) > 127) {
			return false; // Non-ASCII character found
		}
	}
	return true; // All characters are ASCII
}

void raycastMid2(safetyhook::Context& ctx) {
	if (aimbotInfo.entity != "none")
		ctx.xmm2.f32[0] = aimbotInfo.forwardVec.y;
}

void raycastMid3(safetyhook::Context& ctx) {
	if (aimbotInfo.entity != "none")
		ctx.xmm7.f32[0] = aimbotInfo.forwardVec.x;
}

void raycastMid4(safetyhook::Context& ctx) {
	if (aimbotInfo.entity != "none")
		ctx.xmm3.f32[0] = aimbotInfo.forwardVec.z;
}

DWORD WINAPI initClient(LPVOID lpParameter) {
	if (!running) {
		running = true;

		AllocConsole();
		FILE* fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);

		auto game = Utils::FindSignature("A1 ?? ?? ?? ?? 8B 40 ?? 85 C0 74 ?? 0F 28 ?? ?? EB 07 0F 28 05 ?? ?? ?? ?? 80");

		auto worldPtr = *(int*)(Utils::FindSignature("A1 ?? ?? ?? ?? 85 C0 74 07 C6 80 59 01 00 00 01 5D C2 04 00") + 1);

		auto basePtr = *(int*)(game + 1);

		auto test = Utils::FindMultiLevelPtr((std::byte*)basePtr, { 0xC, 0x28, 0x54, 0x88, 0xAC, 0x4 });

		std::cout << test << std::endl;
		std::cout << *(std::byte**)worldPtr << std::endl;

		float* yVel = reinterpret_cast<float*>(test + 0xB4);

		vec3* playerPos = (vec3*)(test + 0x80);

		auto worldTickAddr = Utils::FindSignature("55 8B EC 83 EC 20 53 8B D9 C7 45 F8");
		auto raycastFuncAddr = Utils::FindSignature("55 8B EC 83 E4 ? 83 EC ? F3 0F ? ? ? F3 0F ? ? ? ? ? ? 8B 55");

		auto raycastMidAddr = raycastFuncAddr + 0x2D1;

		//auto cameraTickAddr = BASE + 0xAA9950 + 0xC1;

		//worldTick_hook = safetyhook::create_inline(reinterpret_cast<void*>(worldTickAddr), &worldTick);

		//raycastMid_hook = safetyhook::create_mid(reinterpret_cast<void*>(raycastMidAddr), &raycastMid);

		raycastMidX_hook = safetyhook::create_mid(reinterpret_cast<void*>(raycastFuncAddr + 0x163), &raycastMid3); // x
		raycastMidY_hook = safetyhook::create_mid(reinterpret_cast<void*>(raycastFuncAddr + 0x199), &raycastMid2); // y
		raycastMidZ_hook = safetyhook::create_mid(reinterpret_cast<void*>(raycastFuncAddr + 0x1D0), &raycastMid4); // z
		//camTick_hook = safetyhook::create_mid(reinterpret_cast<void*>(cameraTickAddr), &cameraTick);

		//std::cout << "entityStride: " << entityStride << " entity size: " << entitySize << " entity base: " << entityBasePtr << "\n" << std::endl;	

		while (true) {

			if (GetAsyncKeyState(VK_END)) {
				break;
			}

			int entityStride = *(int*)(*(std::byte**)worldPtr + 0x80);
			int entitySize = *(int*)(*(std::byte**)worldPtr + 0x84);
			int entityBasePtr = *(int*)(*(std::byte**)worldPtr + 0x7C);

			int a = 0;

			aimbotInfo.dist = FLT_MAX;
			aimbotInfo.entity = "none";

			vec3 storedForward{};

			for (int i = 0; i < entitySize; i++) {
				auto entity = (std::byte*)(entityBasePtr + i * entityStride + 0x10);

				if (entity != nullptr) {
					//auto base2 = Utils::FindMultiLevelPtr(entity, { 0xC4, 0x4 });
					auto test2 = (int*)(*(std::byte**)entity);

					if (!IsBadReadPtr(test2, sizeof(void*))) {
						auto name = *(std::byte**)(*(std::byte**)entity + 0x64);
						if (name != 0x0) {
							if (!IsBadReadPtr(name, sizeof(void*))) {
								auto ptr1 = *(std::byte**)(*(std::byte**)entity + 0xC4);
								if (!IsBadReadPtr(ptr1, sizeof(void*))) {
									auto ptr2 = *(std::byte**)(ptr1 + 0x04);
									if (!IsBadReadPtr(ptr2, sizeof(void*)))
									{
										//std::cout << i << " Entity: " << entity << "|" << *(std::byte**)entity << "|" << test2 << std::endl;
										auto name2 = *(const char**)(*(std::byte**)entity + 0x64);
										//std::cout << "name: " << name2 << std::endl;

										auto entityPos = (vec3*)(ptr2 + 0x80);

										//std::cout << "pos: " << entityPos->x << " | " << entityPos->y << " | " << entityPos->z << std::endl;
										a++;

										vec3 temp = { 0.0f, 0.9f, 0.0f };
										auto delta = *playerPos - *entityPos;
										auto dist = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

										std::string nameString = name2;

										if (dist < aimbotInfo.dist && dist < 30.f && !nameString.contains("abilities") && !nameString.contains("plant") && !nameString.contains("collections") && !nameString.contains("portal") && !nameString.empty() && !nameString.contains("trigger") && entityPos->isNotZero()) {
											if (nameString.contains("chest_quest") || isASCII(nameString))
											{
												aimbotInfo.dist = dist;
												aimbotInfo.entity = nameString;

												auto abc = CalcAngle(*playerPos, (*entityPos - temp));
												storedForward = GetForwardVector(abc);
											}
										}
									}
								}
							}
						}
					}
				}
			}

			aimbotInfo.forwardVec = storedForward;

			std::cout << "Nearest dist: " << aimbotInfo.dist << std::endl;
			std::cout << "Targeting: " << aimbotInfo.entity << std::endl;
			

			Sleep(10);

		}
	}

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

