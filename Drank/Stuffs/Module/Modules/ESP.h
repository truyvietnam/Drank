#pragma once
#include "../Module.hpp"
#include <iostream>
#include "../../../Math.hpp"
#include "../../../Utils.hpp"
#include "imgui/imgui.h"
#include "../../Offsets.h"

class ESP : public Module {
public:
	int x = 0;
	int y = 0;

	ESP(Manager* mgr) : Module(mgr, "ESP") {
		this->keybind = VK_OEM_7;
	}

	virtual void onImgui() override {
		auto localPlayerPtr = Offset::base;
		auto worldPtr = Offset::world;

		if (*(std::byte**)worldPtr && *(std::byte**)localPlayerPtr)
		{
			auto camera2 = Utils::FindMultiLevelPtr((std::byte*)localPlayerPtr, { 0x10, 0x120 });

			matrix* w2sMatrix = (matrix*)(camera2 + 0x1F0);

			for (auto& entity : Offset::entities)
			{
				if (!entity) continue;
				auto name2 = *(const char**)(*(std::byte**)entity + 0x64);
				auto entityPos = (vec3*)(Utils::FindMultiLevelPtr((std::byte*)entity, { 0xC4, 0x4 }) + 0x80);

				vec2 screen{};
				if (w2sMatrix->WorldToScreen(
					*entityPos,
					screen,
					(float)x,
					(float)y
				)) {
					ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 16.f, { screen.x, screen.y }, IM_COL32(255, 255, 255, 255), name2);
				}
			}
		}
	}
};