#pragma once
#include "../Module.hpp"
#include "imgui/imgui.h"
#include "../../../Utils.hpp"
#include "../../Manager.hpp"

class ModList : public Module {
public:

	ModList(Manager* mgr) : Module(mgr, "Mod List") {
		this->enabled = true;
	}

	virtual void onImgui() override {
		if (ImGui::Begin("Mod List")) {
			for (auto& mod : manager->modules) {
				ImGui::Text("%s: %d", mod->name.c_str(), mod->enabled);
			}
			ImGui::End();
		}
	}
};