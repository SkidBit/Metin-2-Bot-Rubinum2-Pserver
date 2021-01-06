#include "stdafx.h"
#include "console.h"
#include "mem.h"
#include "constants.h"
#include "game.h"
#include "whisperHook.h"

using namespace std;
#define show_console 1 //1 = show console ~ 0 = don't show console

uintptr_t baseAdressMainMod;

uintptr_t originalStartRecvWhisperPacketFunction = 0x0;
uintptr_t recvWhisperPacketFunctionAddress = 0x0;
vector<BYTE> originalBytesRecvWhisperPacketFunction;

bool botRunning = false;
bool freezeWhenPlayersPresent = true;
bool firstLoop = true;
bool shutdown = false;
bool pickupSpam = false;
bool wallhack = false;

Vector3 anchorPosition = Vector3{ 0,0,0 };
Entity* attackedStone = 0;

vector<int> blacklistedUids = {};

chrono::steady_clock::time_point startTime;
chrono::steady_clock::time_point currentTime;
int elapsedTime;




DWORD WINAPI MainThread(LPVOID param) {

	baseAdressMainMod = (uintptr_t)GetModuleHandle(NULL);
	cout << "-----DEBUGGING-----" << endl;
	cout << "Main module base address: 0x" << hex << baseAdressMainMod << endl;

	recvWhisperPacketFunctionAddress = (uintptr_t)mem::ScanModIn((char*)recvWhisperPacketFunctionPattern, (char*)recvWhisperPacketFunctionMask, "rbclient.exe");

	cout << "RecvWhisperPacket function address: 0x" << hex << recvWhisperPacketFunctionAddress << endl;

	originalStartRecvWhisperPacketFunction = recvWhisperPacketFunctionAddress + 5;
	originalBytesRecvWhisperPacketFunction = mem::detour32((void*)recvWhisperPacketFunctionAddress, whisperHook, 5);

	cout << "-----DEBUGGING-----" << endl;

	Entity* closestStoneToAnchor;
	Entity* closestStoneToPlayer;

	cout << "" << endl;
	cout << "[i] F1 to toggle bot" << endl;
	cout << "[i] F2 to toggle freeze option, default value is ON" << endl;
	cout << "[i] F3 to toggle pickup spam, default value is OFF" << endl;
	cout << "[i] F4 to manually toggle wallhack" << endl;
	cout << "[i] INSERT to shutdown bot and eject DLL" << endl;

	while (!shutdown) {

		// bot loop
		// check if bot enabled
		if (botRunning) {
			// we check if player attributes are accessible (playerobject fully loaded)
			if (game::getPlayerEntity()->getPosition().x != 0) {
				if (firstLoop) {
					cout << "[i] FirstLoop setup is run..." << endl;
					// enable wallhack
					game::enableWallhack();
					wallhack = true;
					cout << "[i] WH enabled" << endl;
					// get Anchor position
					anchorPosition = game::getPlayerEntity()->getPosition();
					cout << "[i] Anchor position set to " << anchorPosition.x << " / " << anchorPosition.y << endl;

					firstLoop = false;
				}

				// TODO: pickup loot if freeze is activated during stone killing
				if (!(freezeWhenPlayersPresent && game::areOtherPlayersPresent())) {
					// check if we selected a stone to attack
					if (attackedStone != 0) {
						// time-critical check!
						// will only be 1 for a short time until object is removed and replaced
						if (attackedStone->getMobDiedRecently() == 1) {
							Sleep(1000);
							game::pickupItems();
							cout << "[i] Stone died - Picked up loot" << endl;
							// dead entities are not instantly remove from the enity list so we need to wait
							// some time for the game to update, so we don't spam attack the same (dead) entity
							Sleep(3000);
							attackedStone = 0;
						}
						else {
							// mob alive and we don't attack or we are not at stone yet
							currentTime = chrono::steady_clock::now();
							elapsedTime = chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

							// have been attacking the same stone for 2 minutes -> we are stuck
							if (elapsedTime > 60) {
								// add uid to blacklist and reset the stone pointer
								blacklistedUids.push_back(attackedStone->getUid());
								cout << "[i] Added stone with UID: " << dec << attackedStone->getUid() << " to blacklist." << endl;
								attackedStone = 0;
							}

							if (game::getPlayerEntity()->getAttackStance() == 0) {
								game::attackEntity(attackedStone);
								//cout << "[i] Attacking again because we got reset..." << endl;
							}
						}
					}
					else {
						closestStoneToAnchor = game::getClosestMetinStone(anchorPosition);
						closestStoneToPlayer = game::getClosestMetinStone(game::getPlayerEntity()->getPosition());

						cout << "CLOSEST TO PLAYER " << hex << closestStoneToPlayer << endl;

						if (game::getDistanceBetweenEntityAndPlayer(game::getPlayerEntity(), closestStoneToPlayer) < distanceToPreferClosestStone) {
							attackedStone = closestStoneToPlayer;
							// if very close to stone attack that one instead of closest to anchor
							//game::playerAttackMobWithUid(closestStoneToPlayer);
							game::attackEntity(closestStoneToPlayer);
							cout << "[i] Player is close to a stone, prefering that one over the one closest to anchor." << endl;
							// start timer for blacklist
							startTime = chrono::steady_clock::now();
						}
						else {
							attackedStone = closestStoneToAnchor;
							// attack closest to anchor
							//game::playerAttackMobWithUid(closestStoneToAnchor);
							game::attackEntity(closestStoneToAnchor);
							// start timer for blacklist
							startTime = chrono::steady_clock::now();
						}
					}
				}
				else {
					cout << "[i] Freeze active and players present. Waiting..." << endl;
					// reset attacked stone to be safe
					attackedStone = 0;
					Sleep(1000);
				}
			}
		}
		Sleep(25);
	}
	cout << "[i] Main thread exiting" << endl;
	return 0;
}

DWORD WINAPI PickupSpamThread(LPVOID param) {
	while (!shutdown) {
		if (pickupSpam) {
			if (!(freezeWhenPlayersPresent && game::areOtherPlayersPresent())) {
				game::pickupItems();
			}
		}
		Sleep(100);
	}
	cout << "[i] PickupSpam thread exiting" << endl;
	return 0;
}

DWORD WINAPI WallhackTestThread(LPVOID param) {

	while (!shutdown) {
		if (wallhack) {
			if (game::getPlayerEntity()->getSkipCollision() == 0) {
				game::getPlayerEntity()->enableSkipCollision();
			}
		}
		Sleep(100);
	}
	cout << "[i] WallhackTest Thread exiting" << endl;
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
				attackedStone = 0;
				game::disableWallhack();
				wallhack = false;
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

		if (GetAsyncKeyState(VK_F3) & 1) {
			pickupSpam = !pickupSpam;
			if (pickupSpam) {
				cout << "[->] PICKUP SPAM ON" << endl;
			}
			else {
				cout << "[->] PICKUP SPAM OFF" << endl;
			}
			Sleep(200);
		}

		if (GetAsyncKeyState(VK_F4) & 1) {
			wallhack = !wallhack;
			if (wallhack) {
				cout << "[->] WALLHACK ON" << endl;
				game::enableWallhack();
			}
			else {
				cout << "[->] WALLHACK OFF" << endl;
				game::disableWallhack();
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
	mem::restoreBytes((void*)recvWhisperPacketFunctionAddress, originalBytesRecvWhisperPacketFunction);
	// disable wallhack
	game::disableWallhack();
	cout << "[i] WH disabled" << endl;

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
		CreateThread(nullptr, 0, PickupSpamThread, hModule, 0, 0);
		CreateThread(nullptr, 0, WallhackTestThread, hModule, 0, 0);
		if (show_console) console(hModule);
	}

	case DLL_PROCESS_DETACH: {
		break;
	}
	}

	return TRUE;
}