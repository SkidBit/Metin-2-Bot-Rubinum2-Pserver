#include "stdafx.h"
#include "constants.h"
#include "game.h"
using namespace std;

inline void __declspec(naked)whisperHook() {
	__asm {
		mov[gotWhispered], 1;
		push ebp;
		mov ebp, esp;
		push 0xFF;
		jmp[originalStartRecvWhisperPacketFunction];
	}
}