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

		printf("%d/%d\n", x, y);

		if (*(std::byte**)worldPtr && *(std::byte**)localPlayerPtr)
		{
			//int x = *(int*)(BASE + 0x1201E68);
			//int y = *(int*)(BASE + 0x1201E6C);

			auto camera2 = Utils::FindMultiLevelPtr((std::byte*)localPlayerPtr, { 0x10, 0x120 });

			matrix* w2sMatrix = (matrix*)(camera2 + 0x1F0);

			int entityStride = *(int*)(*(std::byte**)worldPtr + 0x80);
			int entitySize = *(int*)(*(std::byte**)worldPtr + 0x84);
			int entityBasePtr = *(int*)(*(std::byte**)worldPtr + 0x7C);

			for (int i = 0; i < entitySize; i++) {
				auto entity = (std::byte*)(entityBasePtr + i * entityStride + 0x10);

				if (entity != nullptr) {
					auto test2 = (int*)(*(std::byte**)entity);

					if (!IsBadReadPtr(test2, sizeof(void*))) {
						auto name = *(std::byte**)(*(std::byte**)entity + 0x64);
						if (name != 0x0) {
							if (!IsBadReadPtr(name, sizeof(void*))) {
								auto ptr1 = *(std::byte**)(*(std::byte**)entity + 0xC4);
								if (!IsBadReadPtr(ptr1, sizeof(void*))) {
									auto ptr2 = *(std::byte**)(ptr1 + 0x04);
									if (!IsBadReadPtr(ptr2, sizeof(void*)))
									{
										auto name2 = *(const char**)(*(std::byte**)entity + 0x64);
										auto entityPos = (vec3*)(ptr2 + 0x80);
										std::string nameString = name2;

										vec2 screen{};
										if (w2sMatrix->WorldToScreen(
											*entityPos,
											screen,
											(float)x,
											(float)y
										) && Utils::isASCII(nameString) && entityPos->isNotZero()) {
											ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 16.f, { screen.x, screen.y }, IM_COL32(255, 255, 255, 255), name2);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
};