#pragma once
#include "../Hook.h"
#include "../../Utils.hpp"
#include "../../Offsets.h"
#include <xmmintrin.h>

Manager* mgrRaycast;

SafetyHookMid raycastMidY_hook;
SafetyHookMid raycastMidX_hook;
SafetyHookMid raycastMidZ_hook;

Aimbot* aimbotMod;

void raycastMidY(safetyhook::Context& ctx) { //y
	aimbotMod = mgrRaycast->getModule<Aimbot>();

	if (aimbotMod)
	{
		if (aimbotMod->enabled && aimbotMod->targeting)
		{
			ctx.xmm2.f32[0] = -aimbotMod->forwardVec.y;
			//ctx.xmm0.f32[0] = aimbotMod->forwardVec.y;
		}
	}
}

void raycastMidX(safetyhook::Context& ctx) { //x
	aimbotMod = mgrRaycast->getModule<Aimbot>();

	if (aimbotMod)
	{
		if (aimbotMod->enabled && aimbotMod->targeting)
		{
			ctx.xmm7.f32[0] = -aimbotMod->forwardVec.x;
			//ctx.xmm0.f32[0] = aimbotMod->forwardVec.x;
		}
	}
	else {
		printf("Aimbot module not found\n");
	}
}

void raycastMidZ(safetyhook::Context& ctx) { //z
	aimbotMod = mgrRaycast->getModule<Aimbot>();

	if (aimbotMod)
	{
		if (aimbotMod->enabled && aimbotMod->targeting)
		{
			ctx.xmm3.f32[0] = -aimbotMod->forwardVec.z;
			//ctx.xmm0.f32[0] = aimbotMod->forwardVec.z;
		}
	}
}

float temp1;
float temp2;
float temp3;

void test(safetyhook::Context& ctx) {
	//std::cout << "esi: " << ctx.esi << " " << &ctx.esi << std::endl;
	__m128* a = (__m128*)ctx.esi;

	aimbotMod = mgrRaycast->getModule<Aimbot>();

	if (aimbotMod)
	{
		if (aimbotMod->enabled && aimbotMod->targeting)
		{
			a->m128_f32[0] = aimbotMod->forwardVec.x;
			a->m128_f32[1] = aimbotMod->forwardVec.y;
			a->m128_f32[2] = aimbotMod->forwardVec.z;
		}
	}

	//printf("esi context: %f %f %f\n", temp1, temp2, temp3);

	/*
	if (temp1 != a->m128_f32[0] || temp2 != a->m128_f32[1] || temp3 != a->m128_f32[2])
	{
		temp1 = a->m128_f32[0];
		temp2 = a->m128_f32[1];
		temp3 = a->m128_f32[2];

		std::cout << "esi: " << (std::byte*)ctx.esi << " " << a->m128_f32[0] << " " << a->m128_f32[1] << " " << a->m128_f32[2] << std::endl;
	}
	*/

	//std::cout << "esi: " << (std::byte*)ctx.esi << " " << a->m128_f32[0] << " " << a->m128_f32[1] << " " << a->m128_f32[2] << std::endl;
}

SafetyHookMid a;
SafetyHookMid b;

void test2(safetyhook::Context& ctx) {
	//__m128* a = (__m128*)ctx.eax;
	//std::cout << "eax: " << (std::byte*)ctx.eax << " " << a->m128_f32[0] << " " << a->m128_f32[1] << " " << a->m128_f32[2] << std::endl;
}

class RaycastHook : public Hook {
public:

	RaycastHook(Manager* mgr) : Hook(mgr) {
		mgrRaycast = mgr;
		auto raycastFuncAddr = Utils::FindSignature("55 8B EC 83 E4 ? 83 EC ? F3 0F ? ? ? F3 0F ? ? ? ? ? ? 8B 55");

		//a = safetyhook::create_mid((void*)(BASE + 0x2F0D1D), &test);
		//b = safetyhook::create_mid((void*)(BASE + 0xA6A5BE), &test2);

		if (raycastFuncAddr) {
			CreateMid(raycastMidX_hook, reinterpret_cast<void*>(raycastFuncAddr + 0x163), &raycastMidX);
			//CreateMid(raycastMidX_hook, reinterpret_cast<void*>(BASE + 0x2f0d17), &raycastMidX);

			CreateMid(raycastMidY_hook, reinterpret_cast<void*>(raycastFuncAddr + 0x199), &raycastMidY);
			//CreateMid(raycastMidY_hook, reinterpret_cast<void*>(BASE + 0x2f0d27), &raycastMidY);

			CreateMid(raycastMidZ_hook, reinterpret_cast<void*>(raycastFuncAddr + 0x1D0), &raycastMidZ);
			//CreateMid(raycastMidZ_hook, reinterpret_cast<void*>(BASE + 0x2f0d36), &raycastMidZ);
		}
	}
};
