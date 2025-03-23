#pragma once
#include "../Hook.h"
#include "../../Utils.hpp"
#include "../../Manager.hpp"

Manager* mgrWorld;

SafetyHookInline worldTick_hook;

class WorldTick : public Hook {
public:
	static void worldTickHk(uintptr_t* _this) {
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
		mgrWorld = mgr;
		auto addr = Utils::FindSignature("55 8B EC 83 EC 20 53 8B D9 C7 45 F8");
		if (addr) {
			worldTick_hook = safetyhook::create_inline(reinterpret_cast<void*>(addr), &WorldTick::worldTickHk);
		}
	}
};
