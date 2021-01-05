#include "stdafx.h"
#include "constants.h"
#include "game.h"
using namespace std;

void playWhisperTune() {
	Beep(587, 100);
	Beep(698, 100);
	Beep(880, 100);
	Beep(698, 100);
}

inline void __declspec(naked)whisperHook() {
	__asm {
		call playWhisperTune;
		push ebp;
		mov ebp, esp;
		push 0xFF;
		jmp[originalStartRecvWhisperPacketFunction];
	}
}