#pragma once
#include "../Hook.h"
#include "../../Utils.hpp"

Manager* mgrRaycast;

SafetyHookMid raycastMidY_hook;
SafetyHookMid raycastMidX_hook;
SafetyHookMid raycastMidZ_hook;

Aimbot* aimbotMod;

void raycastMidY(safetyhook::Context& ctx) { //y
	aimbotMod = mgrRaycast->getModule<Aimbot>();

	if (aimbotMod)
		if (aimbotMod->enabled && aimbotMod->targeting)
			ctx.xmm2.f32[0] = aimbotMod->forwardVec.y;
}

void raycastMidX(safetyhook::Context& ctx) { //x
	aimbotMod = mgrRaycast->getModule<Aimbot>();

	if (aimbotMod)
	{
		//printf("Aimbot module found\n");
		if (aimbotMod->enabled && aimbotMod->targeting)
			ctx.xmm7.f32[0] = aimbotMod->forwardVec.x;
	}
	else {
		printf("Aimbot module not found\n");
	}
}

void raycastMidZ(safetyhook::Context& ctx) { //z
	aimbotMod = mgrRaycast->getModule<Aimbot>();

	if (aimbotMod)
		if (aimbotMod->enabled && aimbotMod->targeting)
			ctx.xmm3.f32[0] = aimbotMod->forwardVec.z;
}


class RaycastHook : public Hook {
public:

	RaycastHook(Manager* mgr) : Hook(mgr) {
		mgrRaycast = mgr;
		auto raycastFuncAddr = Utils::FindSignature("55 8B EC 83 E4 ? 83 EC ? F3 0F ? ? ? F3 0F ? ? ? ? ? ? 8B 55");

		if (raycastFuncAddr) {
			raycastMidX_hook = safetyhook::create_mid(reinterpret_cast<void*>(raycastFuncAddr + 0x163), &raycastMidX); // x
			raycastMidY_hook = safetyhook::create_mid(reinterpret_cast<void*>(raycastFuncAddr + 0x199), &raycastMidY); // y
			raycastMidZ_hook = safetyhook::create_mid(reinterpret_cast<void*>(raycastFuncAddr + 0x1D0), &raycastMidZ); // z
		}
	}
};
