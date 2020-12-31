#include "stdafx.h"
#include "constants.h"
using namespace std;

inline int counter;
inline bool alreadyThere = false;
inline DWORD EntityObjStart = 0x0;

inline void __declspec(naked)entityHook() {
	__asm {
		movss[esi + 0x264], xmm2
		mov EntityObjStart, esi
		pushad
	}
	__asm {
		mov eax, EntityObjStart
		mov[entityPointer], eax
	}

	if (entityPointer == 0)
	{
		goto GIVE_UP;
	}
	alreadyThere = false;

	for (counter = 0; counter < 254; counter++)
	{
		if (entities[counter] == entityPointer)
		{
			alreadyThere = true;
			break;
		}
	}

	if (alreadyThere)
	{
		goto GIVE_UP;
	}
	else
	{
		for (counter = 0; counter < 254; counter++)
		{
			if (entities[counter] == 0)
			{
				entities[counter] = entityPointer;
				break;
			}
		}
	}

GIVE_UP:
	__asm {
		popad
		jmp[originalStartEntiyEditFunction]
	}
}