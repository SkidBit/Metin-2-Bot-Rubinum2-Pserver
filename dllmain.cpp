#include "stdafx.h"
#include "console.h"
#include "mem.h"
#include "offsets.h"
#include "game.h"

using namespace std;
#define show_console 1 //1 = show console ~ 0 = don't show console


typedef void(__thiscall* __pickupCloseFunc)(void* classPointer);
__pickupCloseFunc pickupCloseFunc;

bool botRunning = false;
bool firstLoop = true;

uintptr_t baseAdressMainMod;

DWORD WINAPI MainThread(LPVOID param) {
	baseAdressMainMod = (uintptr_t)GetModuleHandle(NULL);

	cout << "Main module base address: 0x" << hex << baseAdressMainMod << endl;

	pickupCloseFunc = (__pickupCloseFunc)(baseAdressMainMod + 0xCAD10);
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
				pickupCloseFunc(*(void**)0x020F8528);
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
