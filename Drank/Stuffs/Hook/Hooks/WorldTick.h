#pragma once
#include "../Hook.h"
#include "../../Utils.hpp"
#include "../../Manager.hpp"

Manager* mgrWorld;
SafetyHookInline worldTick_hook;

class WorldTick : public Hook {
public:
	static void worldTickHk(uintptr_t* _this) {
		auto worldPtr = Offset::world;
		auto localPlayerPtr = Offset::base;

		Offset::entities.clear();

		if (*(std::byte**)worldPtr && *(std::byte**)localPlayerPtr) {
			int entityStride = *(int*)(*(std::byte**)worldPtr + 0x80);
			int entitySize = *(int*)(*(std::byte**)worldPtr + 0x84);
			int entityBasePtr = *(int*)(*(std::byte**)worldPtr + 0x7C);

			for (int i = 0; i < entitySize; i++) {
				auto entity = (std::byte*)(entityBasePtr + i * entityStride + 0x10);
				if (entity != nullptr) {
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
										auto name2 = *(const char**)(*(std::byte**)entity + 0x64);

										auto entityPos = (vec3*)(ptr2 + 0x80);

										vec3 temp = { 0.0f, 0.5f, 0.0f };
										std::string nameString = name2;

										if (entityPos->isNotZero() && Utils::isASCII(nameString))
										{
											Offset::entities.insert((uintptr_t*)entity);
										}
									}
								}
							}
						}
					}
				}
			}
		}

		if (mgrWorld) {
			for (auto& module : mgrWorld->modules) {
				if (module->enabled) {
					module->onWorldTick();
				}
			}
		}

		worldTick_hook.thiscall<void, uintptr_t*>(_this);
	}

	WorldTick(Manager* mgr) : Hook(mgr) {
		mgrWorld = manager;
		auto addr = Utils::FindSignature("55 8B EC 83 EC 20 53 8B D9 C7 45 F8");
		if (addr) {
			CreateInline(worldTick_hook, (void*)addr, &WorldTick::worldTickHk);
		}
	}
};
