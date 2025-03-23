#pragma once
#include <cstdint>
#include <vector>
#include <Windows.h>
#include <Psapi.h>
#include <string>

#define BASE (uintptr_t)GetModuleHandleA("Trove.exe")

class Utils {
public:
	static uintptr_t FindSignature(const char* signature);

	static std::byte* FindMultiLevelPtr(std::byte* baseAddr, std::vector<ptrdiff_t> offsets);

	static bool isASCII(const std::string& str);
};