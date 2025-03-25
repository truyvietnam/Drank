#pragma once
#include <set>

namespace Offset {
	static int worldOffset = 0;
	static int world = 0;
	static int baseOffset = 0;
	static int base = 0;
	static std::set<uintptr_t*> entities{};
}