#include "stdafx.h"
#include "constants.h"
using namespace std;

inline void __declspec(naked)whisperHook() {
	__asm {
		push ebp;
		mov ebp, esp;
		push 0xFF;
		mov[gotWhispered], 1;
		jmp[originalStartRecvWhisperPacketFunction];
	}
}