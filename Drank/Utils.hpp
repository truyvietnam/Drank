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

	static inline void nopBytes(void* dst, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memset(dst, 0x90, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	};
	static inline void copyBytes(void* src, void* dst, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(dst, src, size);
		VirtualProtect(src, size, oldprotect, &oldprotect);
	};
	static inline void patchBytes(void* dst, void* src, unsigned int size) {
		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(dst, src, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);
	};
};