#include "Manager.hpp"
#include "../Utils.hpp"

void Manager::addHook(std::shared_ptr<Hook> hook)
{
	hooks.push_back(hook);
}

void Manager::addModule(std::shared_ptr<Module> module)
{
	modules.push_back(module);
}

#include "Module/Modules/Aimbot.hpp"
#include "Module/Modules/Fly.hpp"
#include "Module/Modules/ESP.h"
#include "Module/Modules/ModList.h"

void Manager::initModules() {
	addModule(std::make_shared<Aimbot>(this));
	addModule(std::make_shared<Fly>(this));
	addModule(std::make_shared<ESP>(this));
	addModule(std::make_shared<ModList>(this));
}

#include "Hook/Hooks/RaycastHook.h"
#include "Hook/Hooks/WorldTick.h"
#include "Hook/Hooks/PresentHook.h"

void Manager::initHooks() {
	addHook(std::make_shared<WorldTick>(this));
	addHook(std::make_shared<RaycastHook>(this));
	addHook(std::make_shared<PresentHook>(this));
}

Manager::Manager() {
	printf("Injected\n");

	initHooks();
	initModules();

	auto worldPtr = *(int*)(Utils::FindSignature("A1 ?? ?? ?? ?? 85 C0 74 07 C6 80 59 01 00 00 01 5D C2 04 00") + 1);
	auto localPlayerPtr = *(int*)(Utils::FindSignature("A1 ?? ?? ?? ?? 8B 40 ?? 85 C0 74 ?? 0F 28 ?? ?? EB 07 0F 28 05 ?? ?? ?? ?? 80") + 1);
	auto test = Utils::FindMultiLevelPtr((std::byte*)localPlayerPtr, { 0xC, 0x28, 0x54, 0x88, 0xAC, 0x4 });

	Offset::world = worldPtr;
	Offset::base = localPlayerPtr;

	init = true;

	while (true) {
		if (GetAsyncKeyState(VK_END)) {
			std::cout << "Ejected" << std::endl;
			break;
		}

		keyListener();

		for (auto& module : modules) {
			if (module->enabled) {
				module->onBaseTick();
			}
		}

		Sleep(10);
	}
}

void Manager::keyListener() { //this so suck lmao
	for (int keyCode = 0; keyCode < 256; ++keyCode) {
		if (GetAsyncKeyState(keyCode) & 1) {
			for (auto& module : modules) {
				if (module->keybind == keyCode) {
					module->enabled = !module->enabled;
					if (module->enabled) {
						module->onEnable();
					}
					else {
						module->onDisable();
					}
				}
			}
		}
	}
}
