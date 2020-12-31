#pragma once
#include "stdafx.h"

namespace mem {
	uintptr_t findDMAAddy(uintptr_t ptr, std::vector<uintptr_t> offsets);
	bool IsBadReadPtr(void* p);
	char* ScanBasic(char* pattern, char* mask, char* begin, intptr_t size);
	char* ScanInternal(char* pattern, char* mask, char* begin, intptr_t size);
	char* ScanModIn(char* pattern, char* mask, std::string modName);
}
