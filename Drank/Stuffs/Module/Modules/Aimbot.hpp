#pragma once
#include "../Module.hpp"
#include "../../Offsets.h"
#include "../../../Math.hpp"
#include <xmmintrin.h>
#include <string>
#include <iostream>

class Aimbot : public Module {
public:
	float dist = FLT_MAX;
	bool targeting = false;
	vec3 forwardVec{};

	Aimbot(Manager* mgr) : Module(mgr, "Aimbot") {
		this->keybind = VK_OEM_1;
	}

	virtual void onBaseTick() override {
		auto worldPtr = Offset::world;
		auto localPlayerPtr = Offset::base;

		if (*(std::byte**)worldPtr && *(std::byte**)localPlayerPtr) {
			int entityStride = *(int*)(*(std::byte**)worldPtr + 0x80);
			int entitySize = *(int*)(*(std::byte**)worldPtr + 0x84);
			int entityBasePtr = *(int*)(*(std::byte**)worldPtr + 0x7C);

			auto test = Utils::FindMultiLevelPtr((std::byte*)localPlayerPtr, { 0xC, 0x28, 0x54, 0x88, 0xAC, 0x4 });
			vec3* playerPos = (vec3*)(test + 0x80);

			int a = 0;

			dist = FLT_MAX;
			targeting = false;

			vec3 storedForward{};

			for (int i = 0; i < entitySize; i++) {
				auto entity = (std::byte*)(entityBasePtr + i * entityStride + 0x10);

				if (entity != nullptr) {
					//auto base2 = Utils::FindMultiLevelPtr(entity, { 0xC4, 0x4 });
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
										//std::cout << i << " Entity: " << entity << "|" << *(std::byte**)entity << "|" << test2 << std::endl;
										auto name2 = *(const char**)(*(std::byte**)entity + 0x64);
										//std::cout << "name: " << name2 << std::endl;

										auto entityPos = (vec3*)(ptr2 + 0x80);

										//std::cout << "pos: " << entityPos->x << " | " << entityPos->y << " | " << entityPos->z << std::endl;
										a++;

										vec3 temp = { 0.0f, 0.5f, 0.0f };

										if (entityPos && playerPos)
										{
											auto delta = *playerPos - *entityPos;
											auto dist2 = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

											std::string nameString = name2;


											if (dist2 < dist
												&& dist2 < 40.f
												&& !nameString.contains("abilities")
												&& !nameString.contains("plant")
												&& !nameString.contains("collections")
												&& !nameString.contains("portal")
												&& !nameString.empty()
												&& !nameString.contains("trigger")
												&& !nameString.contains("/deco/")
												&& !nameString.contains("/crafting/")
												&& entityPos->isNotZero()) {

												if (nameString.contains("chest_quest") || Utils::isASCII(nameString))
												{
													dist = dist;
													targeting = true;

													auto camera2 = Utils::FindMultiLevelPtr((std::byte*)localPlayerPtr, { 0x10, 0x120 });

													vec3* cameraPos = (vec3*)(camera2 + 0x70);

													storedForward = Math::GetForwardVector(*entityPos, *cameraPos);
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
			forwardVec = storedForward;
		}
	}
};