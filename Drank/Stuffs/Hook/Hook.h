#pragma once
#include <safetyhook.hpp>

class Manager;

class Hook {
public:
	Manager* manager;

	Hook(Manager* mgr);
};