#include <iostream>
#include <Windows.h>
#include <Psapi.h>

#include <cstdint>
#include "Utils.hpp"
#include <vector>
#include <algorithm>
#include <optional>

uintptr_t Utils::FindSignature(const char* signature) {
    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<std::optional<uint8_t>>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + strlen(pattern);
        bytes.reserve(strlen(pattern) / 2);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(std::nullopt);
            }
            else bytes.push_back((uint8_t)strtoul(current, &current, 16));
        }
        return bytes;
        };
    auto gameModule = BASE;
    auto* const scanBytes = reinterpret_cast<uint8_t*>(gameModule);
    auto* const dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(gameModule);
    auto* const ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(scanBytes + dosHeader->e_lfanew);
    const auto sizeOfCode = ntHeaders->OptionalHeader.SizeOfImage;

    const auto pattern = pattern_to_byte(signature);
    const auto end = scanBytes + sizeOfCode;
    auto it = std::search(
        scanBytes, end,
        pattern.cbegin(), pattern.cend(),
        [](auto byte, auto opt) {
            return !opt.has_value() || *opt == byte;
        });
    auto ret = it != end ? (uintptr_t)it : 0u;
    if (!ret)
        std::printf("Signature Dead: %s", signature);

    return ret;
}

std::byte* Utils::FindMultiLevelPtr(std::byte* baseAddr, std::vector<ptrdiff_t> offsets) {
	if (!baseAddr)
		return baseAddr;

	auto addr = baseAddr;
	auto offset = offsets.begin();
	do {
		addr = *reinterpret_cast<std::byte**>(addr);
		if (!addr || offset == offsets.end()) {
			break;
		}
		addr += *offset;
		offset++;
	} while (true);
	return addr;
};