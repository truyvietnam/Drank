#include "Hook.h"
#include "../Manager.hpp"

Hook::Hook(Manager* mgr)
{
	manager = mgr;
}

void Hook::CreateInline(SafetyHookInline& inlineH, void* addr, void* callback)
{
	inlineH = safetyhook::create_inline(addr, callback);
	manager->inlineHooks.push_back(&inlineH);
}

void Hook::CreateMid(SafetyHookMid& midH, void* addr, safetyhook::MidHookFn callback)
{
	midH = safetyhook::create_mid(addr, callback);
	manager->midHooks.push_back(&midH);
}
