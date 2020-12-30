#pragma once
#include "stdafx.h"

namespace mem {
	uintptr_t findDMAAddy(uintptr_t ptr, std::vector<uintptr_t> offsets);
	bool IsBadReadPtr(void* p);
}