#include "stdafx.h"
#include "console.h"
#include "mem.h"
#include "constants.h"
#include "game.h"

using namespace std;
#define show_console 1 //1 = show console ~ 0 = don't show console

//Function
//Address of signature = rbclient.exe + 0x000CAD10
//"\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\x00\x00\x00\x00\x00\x00\x00\x00\x00\x53\x56\x57\xA1\x00\x00\x00\x00\x00\x00\x00\x00\x45\x00\x64\x00\x00\x00\x00\x00\x00\x00\x00\x00\xF0\x8D\x00\x00\x8B\x01", "xxxx?x????x?????????xxxx????????x?x?????????xx??xx"
//"55 8B EC 6A ? 68 ? ? ? ? 64 ? ? ? ? ? ? ? ? ? 53 56 57 A1 ? ? ? ? ? ? ? ? 45 ? 64 ? ? ? ? ? ? ? ? ? F0 8D ? ? 8B 01"

//Classpointer, skip 2 bytes and then copy address
//Address of signature = rbclient.exe + 0x000CD0E0
//"\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x55\x8B\xEC\x51\x8D\x45\x00\x50\x6A\x00\xFF\x75\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x84\xC0\x75\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\xFF\x75\x00\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x51\x8D\x45\x00\x50\x6A\x00\xFF\x75\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x84\xC0\x75\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\x83\x7D\xFC", "xx????x????x????xxxxxx?xx?xx?x????xx?xxx?x????xxxxxx?xx????x????x????xxxxxxxxxxxxxxxxx?xx?xx?x????xx?xxx?x????xxxxxxx"
//"8B 0D ? ? ? ? E8 ? ? ? ? E9 ? ? ? ? 55 8B EC 51 8D 45 ? 50 6A ? FF 75 ? E8 ? ? ? ? 83 C4 ? 84 C0 75 ? E8 ? ? ? ? 8B E5 5D C3 FF 75 ? 8B 0D ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 8B E5 5D C3 CC CC CC CC CC CC CC 55 8B EC 51 8D 45 ? 50 6A ? FF 75 ? E8 ? ? ? ? 83 C4 ? 84 C0 75 ? E8 ? ? ? ? 8B E5 5D C3 83 7D FC"

typedef void(__thiscall* __pickupCloseFunc)(void* classPointer);
__pickupCloseFunc pickupCloseFunc;

bool botRunning = false;
bool firstLoop = true;

uintptr_t baseAdressMainMod;

DWORD WINAPI MainThread(LPVOID param) {
	baseAdressMainMod = (uintptr_t)GetModuleHandle(NULL);
	cout << "-----DEBUGGING-----" << endl;
	cout << "Main module base address: 0x" << hex << baseAdressMainMod << endl;

	uintptr_t pickupFunctionAddress = (uintptr_t)mem::ScanModIn((char*)pickupFunctionPattern, (char*)pickupFunctionMask, "rbclient.exe");
	uintptr_t pickupFunctionClassPointerFunctionAddress = (uintptr_t)mem::ScanModIn((char*)pickupFunctionClassPointerFunctionPattern, (char*)pickupFunctionClassPointerFunctionMask, "rbclient.exe");

	cout << "Pickup function address: 0x" << hex << pickupFunctionAddress << endl;
	cout << "Pickup classpointer function address: 0x" << hex << pickupFunctionClassPointerFunctionAddress << endl;

	uintptr_t pickupFunctionClassPointer = *(uintptr_t*)(pickupFunctionClassPointerFunctionAddress + pickupFunctionOffsetToClassPointer);

	cout << "Pickup classpointer address: 0x" << hex << pickupFunctionClassPointer << endl;
	cout << "-----DEBUGGING-----" << endl;
	pickupCloseFunc = (__pickupCloseFunc)(pickupFunctionAddress);
	// pickupCloseFunc(*(void**)0x020F8528);

	vector<float> anchorPosition;
	uintptr_t offsetClosestStone;
	chrono::steady_clock::time_point timerStart;
	chrono::steady_clock::time_point timerRound;

	while (true) {

		// bot loop
		// check if bot enabled
		if (botRunning) {

			if (firstLoop) {
				cout << "[X] FirstLoop setup is run..." << endl;
				Sleep(5000);
				// enable wallhack
				game::enableWallhack();
				cout << "[X] WH enabled" << endl;
				// get Anchor position
				anchorPosition = game::getPlayerPos();
				cout << "[X] Anchor position set" << endl;

				firstLoop = false;
			}

			if (!game::isPlayerAttackingMob()) {
				Sleep(1000);
				// pickup and wait for short amount
				pickupCloseFunc(*(void**)pickupFunctionClassPointer);
				cout << "Picked up loot" << endl;
				// sleeps are neccessary because stone will be read as alive short time after death
				Sleep(3000);

				// get closest stone
				offsetClosestStone = game::getOffsetOfClosestMetinStone(1500, anchorPosition);
				// attack stone
				game::playerAttackMobWithUid(offsetClosestStone);
				// start timing to detect attackid bug
				timerStart = chrono::steady_clock::now();
				cout << "Attacking NOW..." << endl;
			}

			timerRound = chrono::steady_clock::now();

			if (chrono::duration_cast<chrono::seconds>(timerRound - timerStart).count() > 60) {
				// 60 seconds ago we started attacking and stone still lives, game is buggy
				game::resetPlayerAtatck();
				cout << "Game bugged out, reset issued" << endl;
			}

		}
		else {
			firstLoop = true;
		}

		if (GetAsyncKeyState(VK_F1) & 1) {
			botRunning = !botRunning;
			cout << "BOT TOGGLED" << endl;
			Sleep(200);
		}

		if (GetAsyncKeyState(VK_INSERT) & 1) {
			if (show_console) {

				auto temp = GetConsoleWindow();
				FreeConsole();
				PostMessage(temp, WM_QUIT, 0, 0);
			}
			break;
		}
		Sleep(1);
	}

	FreeLibraryAndExitThread((HMODULE)param, 0);
	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
		if (show_console) console(hModule);
	}

	case DLL_PROCESS_DETACH: {
		break;
	}
	}

	return TRUE;
}
