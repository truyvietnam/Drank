#include "Module.hpp"
#include "../Manager.hpp"

Module::Module(Manager* mgr, std::string modName) {
	manager = mgr;
	name = modName;
}