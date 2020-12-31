#include "stdafx.h"
#include "mem.h"
using namespace std;

uintptr_t mem::findDMAAddy(uintptr_t ptr, vector<uintptr_t> offsets)
{
	uintptr_t addr = ptr;

	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		// not tested
		if (IsBadReadPtr((uintptr_t*)addr)) {
			return (uintptr_t)nullptr;
		}
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}

bool mem::IsBadReadPtr(void* p)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (::VirtualQuery(p, &mbi, sizeof(mbi)))
	{
		DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
		bool b = !(mbi.Protect & mask);
		// check the page is not a guard page
		if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) b = true;

		return b;
	}
	return true;
}


char* TO_CHAR(wchar_t* string)
{
	size_t len = wcslen(string) + 1;
	char* c_string = new char[len];
	size_t numCharsRead;
	wcstombs_s(&numCharsRead, c_string, len, string, _TRUNCATE);
	return c_string;
}

PEB* GetPEB()
{
#ifdef _WIN64
	PEB* peb = (PEB*)__readgsword(0x60);

#else
	PEB* peb = (PEB*)__readfsdword(0x30);
#endif

	return peb;
}

LDR_DATA_TABLE_ENTRY* GetLDREntry(std::string name)
{
	LDR_DATA_TABLE_ENTRY* ldr = nullptr;

	PEB* peb = GetPEB();

	LIST_ENTRY head = peb->Ldr->InMemoryOrderModuleList;

	LIST_ENTRY curr = head;

	while (curr.Flink != head.Blink)
	{
		LDR_DATA_TABLE_ENTRY* mod = (LDR_DATA_TABLE_ENTRY*)CONTAINING_RECORD(curr.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		if (mod->FullDllName.Buffer)
		{
			char* cName = TO_CHAR(mod->BaseDllName.Buffer);

			if (_stricmp(cName, name.c_str()) == 0)
			{
				ldr = mod;
				break;
			}
			delete[] cName;
		}
		curr = *curr.Flink;
	}
	return ldr;
}


char* mem::ScanModIn(char* pattern, char* mask, std::string modName)
{
	LDR_DATA_TABLE_ENTRY* ldr = GetLDREntry(modName);

	char* match = mem::ScanInternal(pattern, mask, (char*)ldr->DllBase, ldr->SizeOfImage);

	return match;
}


char* mem::ScanInternal(char* pattern, char* mask, char* begin, intptr_t size)
{
	char* match{ nullptr };
	MEMORY_BASIC_INFORMATION mbi{};

	for (char* curr = begin; curr < begin + size; curr += mbi.RegionSize)
	{
		if (!VirtualQuery(curr, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS) continue;

		match = ScanBasic(pattern, mask, curr, mbi.RegionSize);

		if (match != nullptr)
		{
			break;
		}
	}
	return match;
}



char* mem::ScanBasic(char* pattern, char* mask, char* begin, intptr_t size)
{
	intptr_t patternLen = strlen(mask);

	for (int i = 0; i < size; i++)
	{
		bool found = true;
		for (int j = 0; j < patternLen; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(char*)((intptr_t)begin + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return (begin + i);
		}
	}
	return nullptr;
}
