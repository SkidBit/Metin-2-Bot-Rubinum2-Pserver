#include "stdafx.h"
#include "console.h"
#include "mem.h"
#include "constants.h"
#include "game.h"
#include "entityHook.h"

using namespace std;
#define show_console 1 //1 = show console ~ 0 = don't show console

uintptr_t baseAdressMainMod;
vector<BYTE> originalBytesEntiyEditFunction;
uintptr_t originalStartEntiyEditFunction = 0x0;
Entity* entities[255];
Entity* entityPointer;
uintptr_t editEntityFunctionAddress;

bool botRunning = false;
bool freezeWhenPlayersPresent = true;
bool firstLoop = true;
bool shutdown = false;

DWORD WINAPI MainThread(LPVOID param) {

	typedef void(__thiscall* __pickupCloseFunc)(void* classPointer);
	__pickupCloseFunc pickupCloseFunc;

	baseAdressMainMod = (uintptr_t)GetModuleHandle(NULL);
	cout << "-----DEBUGGING-----" << endl;
	cout << "Main module base address: 0x" << hex << baseAdressMainMod << endl;

	uintptr_t pickupFunctionAddress = (uintptr_t)mem::ScanModIn((char*)pickupFunctionPattern, (char*)pickupFunctionMask, "rbclient.exe");
	uintptr_t pickupFunctionClassPointerFunctionAddress = (uintptr_t)mem::ScanModIn((char*)pickupFunctionClassPointerFunctionPattern, (char*)pickupFunctionClassPointerFunctionMask, "rbclient.exe");

	cout << "Pickup function address: 0x" << hex << pickupFunctionAddress << endl;
	cout << "Pickup classpointer function address: 0x" << hex << pickupFunctionClassPointerFunctionAddress << endl;

	uintptr_t pickupFunctionClassPointer = *(uintptr_t*)(pickupFunctionClassPointerFunctionAddress + pickupFunctionOffsetToClassPointer);

	cout << "Pickup classpointer address: 0x" << hex << pickupFunctionClassPointer << endl;

	editEntityFunctionAddress = (uintptr_t)mem::ScanModIn((char*)editEntityFunctionPattern, (char*)editEntityFunctionMask, "rbclient.exe");

	cout << "Edit entity function address: 0x" << hex << editEntityFunctionAddress << endl;

	originalStartEntiyEditFunction = editEntityFunctionAddress + 5;
	originalBytesEntiyEditFunction = mem::detour32((void*)editEntityFunctionAddress, entityHook, 8);

	cout << "-----DEBUGGING-----" << endl;
	pickupCloseFunc = (__pickupCloseFunc)(pickupFunctionAddress);
	// pickupCloseFunc(*(void**)0x020F8528);

	Vector3 anchorPosition = Vector3{ 0,0,0 };
	Entity* closestStone;
	chrono::steady_clock::time_point timerStart;
	chrono::steady_clock::time_point timerRound;

	cout << "" << endl;
	cout << "[i] F1 to toggle bot" << endl;
	cout << "[i] F2 to toggle freeze option, default value is ON" << endl;
	cout << "[i] INSERT to shutdown bot and eject DLL" << endl;

	while (!shutdown) {

		// bot loop
		// check if bot enabled
		if (botRunning) {

			if (firstLoop) {
				cout << "[i] FirstLoop setup is run..." << endl;
				// enable wallhack
				game::enableWallhack();
				cout << "[i] WH enabled" << endl;
				// get Anchor position
				anchorPosition = game::getPlayerEntity()->getPosition();
				cout << "[i] Anchor position set to " << anchorPosition.x << " / " << anchorPosition.y << endl;

				firstLoop = false;
			}

			if (!(freezeWhenPlayersPresent && game::areOtherPlayersPresent())) {

				if (!game::isPlayerAttackingMob()) {
					Sleep(1000);
					// pickup and wait for short amount
					pickupCloseFunc(*(void**)pickupFunctionClassPointer);
					cout << "[i] Picked up loot" << endl;
					// sleeps are neccessary because stone will be read as alive short time after death
					Sleep(3000);

					// get closest stone
					closestStone = game::getClosestMetinStone(anchorPosition);
					// attack stone
					game::playerAttackMobWithUid(closestStone);
					// start timing to detect attackid bug
					timerStart = chrono::steady_clock::now();
					cout << "[i] Attacking Stone with mobID: " << dec << closestStone->getMobId() << " and Uid of: " << closestStone->getUid() << endl;
				}

				timerRound = chrono::steady_clock::now();

				if (chrono::duration_cast<chrono::seconds>(timerRound - timerStart).count() > 60) {
					// 60 seconds ago we started attacking and stone still lives, game is buggy
					game::resetPlayerAtatck();
					cout << "[!] Game bugged out, reset issued" << endl;
				}
			}
			else {
				cout << "[i] Freeze active and players present. Waiting..." << endl;
				Sleep(1000);
			}
		}

		Sleep(1);
	}
	cout << "main thread exiting" << endl;
	return 0;
}

DWORD WINAPI ControlsThread(LPVOID param) {
	while (!shutdown) {

		if (GetAsyncKeyState(VK_F1) & 1) {
			botRunning = !botRunning;
			if (botRunning) {
				cout << "[->] BOT ON" << endl;
			}
			else {
				cout << "[->] BOT OFF" << endl;
				game::disableWallhack();
				cout << "[i] WH disabled" << endl;
				firstLoop = true;
			}

			Sleep(200);
		}

		if (GetAsyncKeyState(VK_F2) & 1) {
			freezeWhenPlayersPresent = !freezeWhenPlayersPresent;
			if (freezeWhenPlayersPresent) {
				cout << "[->] FREEZE WHEN PLAYERS PRESENT ON" << endl;
			}
			else {
				cout << "[->] FREEZE WHEN PLAYERS PRESENT OFF" << endl;
			}

			Sleep(200);
		}

		if (GetAsyncKeyState(VK_INSERT) & 1) {

			shutdown = true;
			Sleep(5000);
		}

		Sleep(1);
	}

	// unhooking so we don't crash the game
	mem::restoreBytes((void*)editEntityFunctionAddress, originalBytesEntiyEditFunction);
	// disable wallhack
	game::disableWallhack();

	cout << "[i] GOODBYE, BOT TERMINATING NOW" << endl;
	Sleep(1000);

	if (show_console) {
		auto temp = GetConsoleWindow();
		FreeConsole();
		PostMessage(temp, WM_QUIT, 0, 0);
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
		DisableThreadLibraryCalls((HMODULE)hModule);
		CreateThread(nullptr, 0, MainThread, hModule, 0, 0);
		CreateThread(nullptr, 0, ControlsThread, hModule, 0, 0);
		if (show_console) console(hModule);
	}

	case DLL_PROCESS_DETACH: {
		break;
	}
	}

	return TRUE;
}