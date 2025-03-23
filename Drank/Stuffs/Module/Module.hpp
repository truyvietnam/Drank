#pragma once
#include <vector>
#include <memory>
#include <string>

class Manager;

class Module {
public:
	Manager* manager;

	std::string name;
	bool enabled = false;
	int keybind = 0;

	Module(Manager*, std::string);

	virtual void onEnable() {};
	virtual void onDisable() {};
	virtual void onWorldTick() {};
	virtual void onBaseTick() {};
	virtual void onImgui() {};
};