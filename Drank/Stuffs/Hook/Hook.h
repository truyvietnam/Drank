#pragma once
#include <safetyhook.hpp>

class Manager;

class Hook {
public:
	Manager* manager;

	Hook(Manager*);

	void CreateInline(SafetyHookInline& inlineH, void* addr, void* callback);
	void CreateMid(SafetyHookMid& midH, void* addr, safetyhook::MidHookFn callback);
};