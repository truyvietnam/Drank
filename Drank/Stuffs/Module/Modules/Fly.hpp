#pragma once
#include "../Module.hpp"
#include <iostream>
#include "../../../Utils.hpp"

class Fly : public Module {
public:
	float speed = 0.1f;
	int i = 0;

	void* NoGravity;
	void* Noclip;
	void* MovementUpdateLimit;

	Fly(Manager* mgr) : Module(mgr, "Fly") {
		//this->keybind = 'F';

		NoGravity = (void*)Utils::FindSignature("F3 0F 10 05 ? ? ? ? F3 0F ? ? ? D9 45 ? 8B E5 5D C3 D9");
		Noclip = (void*)Utils::FindSignature("74 31 FF 73 14");
		MovementUpdateLimit = (void*)Utils::FindSignature("DC 67 68 C6");
	}

	virtual void onEnable() override {
		BYTE* patch2 = (BYTE*)"\xEB\x31\xFF\x73\x14\x8B\x47\x04\x2B\x07";
		Utils::patchBytes((BYTE*)((uintptr_t)Noclip), patch2, 10);

		BYTE* patch1 = (BYTE*)"\xF3\x0F\x10\x0D";
		Utils::patchBytes((BYTE*)((uintptr_t)NoGravity), patch1, 4);

		BYTE* patch3 = (BYTE*)"\xDC\x47\x68\xC6";
		Utils::patchBytes((BYTE*)((uintptr_t)MovementUpdateLimit), patch3, 4);
	}

	virtual void onDisable() {
		BYTE* patch2 = (BYTE*)"\x74\x31\xFF\x73\x14\x8B\x47\x04\x2B\x07";
		Utils::patchBytes((BYTE*)((uintptr_t)Noclip), patch2, 10);

		BYTE* patch1 = (BYTE*)"\xF3\x0F\x10\x05";
		Utils::patchBytes((BYTE*)((uintptr_t)NoGravity), patch1, 4);

		BYTE* patch3 = (BYTE*)"\xDC\x67\x68\xC6";
		Utils::patchBytes((BYTE*)((uintptr_t)MovementUpdateLimit), patch3, 4);
	}
};