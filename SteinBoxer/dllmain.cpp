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

Vector3 anchorPosition = Vector3{ 0,0,0 };

DWORD WINAPI MainThread(LPVOID param) {

	baseAdressMainMod = (uintptr_t)GetModuleHandle(NULL);
	cout << "-----DEBUGGING-----" << endl;
	cout << "Main module base address: 0x" << hex << baseAdressMainMod << endl;

	editEntityFunctionAddress = (uintptr_t)mem::ScanModIn((char*)editEntityFunctionPattern, (char*)editEntityFunctionMask, "rbclient.exe");

	cout << "Edit entity function address: 0x" << hex << editEntityFunctionAddress << endl;

	originalStartEntiyEditFunction = editEntityFunctionAddress + 5;
	originalBytesEntiyEditFunction = mem::detour32((void*)editEntityFunctionAddress, entityHook, 8);

	cout << "-----DEBUGGING-----" << endl;

	// pickupCloseFunc(*(void**)0x020F8528);

	Entity* closestStoneToAnchor;
	Entity* closestStoneToPlayer;
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

			// check if game is fully loaded and player entity is accessible
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


			// TODO: pickup loot if freeze is activated during stone killing
			if (!(freezeWhenPlayersPresent && game::areOtherPlayersPresent())) {

				if (!game::isPlayerAttackingMob()) {
					// short sleep before collecting items so we don't insta-pickup
					Sleep(1000);
					game::pickupItems();
					cout << "[i] Picked up loot" << endl;
					// dead entities are not instantly remove from the enity list so we need to wait
					// some time for the game to update, so we don't spam attack the same (dead) entity
					Sleep(3000);

					// this is a small hack to avoid invalid anchors if the player ported during the sleep above
					if (firstLoop) {
						while (game::getPlayerEntity() == 0) {
							Sleep(500);
						}
						// set new anchor after porting
						anchorPosition = game::getPlayerEntity()->getPosition();
						cout << "[i] Anchor position >after loadscreen< set to " << anchorPosition.x << " / " << anchorPosition.y << endl;
					}

					closestStoneToAnchor = game::getClosestMetinStone(anchorPosition);
					closestStoneToPlayer = game::getClosestMetinStone(game::getPlayerEntity()->getPosition());

					if (game::getDistanceBetweenEntities(game::getPlayerEntity(), closestStoneToPlayer) < distanceToPreferClosestStone) {
						// if very close to stone attack that one instead of closest to anchor
						game::playerAttackMobWithUid(closestStoneToPlayer);
						cout << "[i] Player is close to a stone, prefering that one over the one closest to anchor." << endl;
					}
					else {
						// attack closest to anchor
						game::playerAttackMobWithUid(closestStoneToAnchor);
					}

					// start timing to detect attackid bug
					timerStart = chrono::steady_clock::now();
					cout << "[i] Attacking Stone with mobID: " << dec << closestStoneToAnchor->getMobId() << " and Uid of: " << closestStoneToAnchor->getUid() << endl;
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
		Sleep(25);
	}
	cout << "[i] Main thread exiting" << endl;
	return 0;
}

DWORD WINAPI FlushThread(LPVOID param) {

	while (!shutdown) {
		if (game::getPlayerEntity() == 0) {
			// we need to run firstloop again for setup after playerent was null
			firstLoop = true;
			cout << "[i] PlayerEnt is NULL" << endl;
			// flush entity array to get rid of broken entity entries after teleporting etc.
			game::flushEntityArray();
			// Sleep to not spam the function
			Sleep(500);
		}
		Sleep(25);

	}
	cout << "[i] Flush thread exiting" << endl;
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
			// give other Threads some time to shutdown
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
		CreateThread(nullptr, 0, FlushThread, hModule, 0, 0);
		if (show_console) console(hModule);
	}

	case DLL_PROCESS_DETACH: {
		break;
	}
	}

	return TRUE;
}