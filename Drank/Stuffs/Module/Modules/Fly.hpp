#pragma once
#include "../Module.hpp"
#include <iostream>
#include "../../../Utils.hpp"

class Fly : public Module {
public:
	float speed = 0.1f;
	int i = 0;

	Fly(Manager* mgr) : Module(mgr, "Fly") {
	}

	virtual void onWorldTick() override { 
		auto worldPtr = *(int*)(Utils::FindSignature("A1 ?? ?? ?? ?? 85 C0 74 07 C6 80 59 01 00 00 01 5D C2 04 00") + 1);

		auto localPlayerPtr = *(int*)(Utils::FindSignature("A1 ?? ?? ?? ?? 8B 40 ?? 85 C0 74 ?? 0F 28 ?? ?? EB 07 0F 28 05 ?? ?? ?? ?? 80") + 1);
	}
};