#pragma once
#include <vector>
#include <memory>
#include <shared_mutex>

class Hook;
class Module;

class Manager {
private:
	std::shared_mutex moduleListMutex;
public:
	std::vector<std::shared_ptr<Module>> modules;
	std::vector<std::shared_ptr<Hook>> hooks;
	bool init = false;

	Manager();
	void keyListener();

	void initModules();
	void initHooks();

	void addHook(std::shared_ptr<Hook> hook);
	void addModule(std::shared_ptr<Module> module);

	std::shared_lock<std::shared_mutex> lockModuleList() { return std::shared_lock(moduleListMutex); }
	std::unique_lock<std::shared_mutex> lockModuleListExclusive() { return std::unique_lock(moduleListMutex); }

	std::shared_mutex* getModuleListLock() { return &moduleListMutex; }

	bool isInitialized() { return init; };

	template <typename TRet>
	TRet* getModule() {
		if (!isInitialized())
			return nullptr;
		auto lock = lockModuleList();
		for (auto pMod : modules) {
			if (auto pRet = dynamic_cast<typename std::remove_pointer<TRet>::type*>(pMod.get())) {

				return pRet;
			}
		}
		return nullptr;
	};

};