#include "stdafx.h"
#include "console.h"


using namespace blackbone;
#define show_console 1 //1 = show console ~ 0 = don't show console

//Blackbone stuff
Process proc;
Process* procPointer;
ModuleDataPtr pMainMod;
ptr_t baseAdressMainMod;

//typedefs for functions
// slot logic 001, 101, 201 etc.
typedef void(__thiscall* __useItemFunc)(void* classPointer, WORD slot);
__useItemFunc useItemFunc;

typedef void(__thiscall* __pickupCloseFunc)(void* classPointer);
__pickupCloseFunc pickupCloseFunc;

//MOB STRUCT
//pointer structure: game + base offset, index(4*i), value to read
//ptr_t offsetToMobList = 0x1CF8650;
// 0x195EE38
ptr_t offsetToMobList = 0x1C16B78;
//int
ptr_t offsetToMobId = 0x244;
//floats
ptr_t offsetToMobXPos = 0x2DC;
ptr_t offsetToMobYPos = 0x2E0;
ptr_t offsetToMobZPos = 0x2E4;
ptr_t offsetToMobUniqueId = 0x1C54;
//2 if alive 1 if dead
ptr_t offsetToMobIsAlive = 0x730;
ptr_t offsetToMobIsVisible = 0x340;

//ATTACK MOB WITH ID
//base offset
ptr_t offsetToPlayerControlObject = 0x1CF8528;
//int
ptr_t offsetToAttackUID = 0x50;

//WALLHACK
//base offset
ptr_t offsetWallHackBase = 0x1CF853C;
//byte
ptr_t offsetWallHackOne = 0xC;
ptr_t offsetWallHackTwo = 0x1C5C;

//PLAYER
ptr_t offsetToPlayerBase = 0x1C6C354;
ptr_t offsetToPlayerOne = 0xC;
//float
ptr_t offsetToPlayerXPos = 0x2DC;
ptr_t offsetToPlayerYPos = 0x2E0;
ptr_t offsetToPlayerZPos = 0x2E4;

//ids
int metinIdStart = 8000;
int metinIdEnd = 8112;

bool botRunning = false;
bool firstLoop = true;

std::vector<float> getPlayerPos(Process* proc);
std::vector<float> getMobPos(Process* proc, ptr_t offsetInMoblist);
ptr_t getOffsetOfClosestMetinStone(Process* proc, int scansize, std::vector<float> anchorPosition);
int getMobUid(Process* proc, ptr_t offsetInMoblist);
void playerAttackMobWithUid(Process* proc, ptr_t offsetInMobList);
void enableWallhack(Process* proc);
void disableWallhack(Process* proc);
bool isPlayerAttackingMob(Process* proc);
void resetPlayerAtatck(Process* proc);


DWORD WINAPI MainThread(LPVOID param) {
	proc.Attach(GetCurrentProcessId());
	procPointer = &proc;

	pMainMod = proc.modules().GetMainModule();
	baseAdressMainMod = pMainMod->baseAddress;

	std::cout << "Main module base address: 0x" << std::hex << baseAdressMainMod << std::endl;

	// set the address of the function (relative to modbase)
	useItemFunc = (__useItemFunc)(baseAdressMainMod + 0x1695B0);
	pickupCloseFunc = (__pickupCloseFunc)(baseAdressMainMod + 0xCAD10);
	// pickupCloseFunc(*(void**)0x020F8528);
	// useItemFunc(*(void**)0x20F8560, 0x101);

	std::vector<float> anchorPosition;
	ptr_t offsetClosestStone;
	std::chrono::steady_clock::time_point timerStart;
	std::chrono::steady_clock::time_point timerRound;

	while (true) {


		// bot loop
		// check if bot enabled
		if (botRunning) {

			if (firstLoop) {
				std::cout << "FirstLoop setup is run..." << std::endl;
				// enable wallhack
				enableWallhack(procPointer);
				std::cout << "WH enabled" << std::endl;
				// get Anchor position
				anchorPosition = getPlayerPos(procPointer);
				std::cout << "Anchor position set" << std::endl;

				firstLoop = false;
			}

			if (!isPlayerAttackingMob(procPointer)) {
				Sleep(1000);
				// pickup and wait for short amount
				pickupCloseFunc(*(void**)0x020F8528);
				std::cout << "Picked up loot" << std::endl;
				// sleeps are neccessary because stone will be read as alive short time after death
				Sleep(3000);

				// get closest stone
				offsetClosestStone = getOffsetOfClosestMetinStone(procPointer, 1500, anchorPosition);
				// attack stone
				playerAttackMobWithUid(procPointer, offsetClosestStone);
				// start timing to detect attackid bug
				timerStart = std::chrono::steady_clock::now();
				std::cout << "Attacking NOW..." << std::endl;
			}

			timerRound = std::chrono::steady_clock::now();

			if (std::chrono::duration_cast<std::chrono::seconds>(timerRound - timerStart).count() > 60) {
				// 60 seconds ago we started attacking and stone still lives, game is buggy
				resetPlayerAtatck(procPointer);
				std::cout << "Game bugged out, reset issued" << std::endl;
			}

		}
		else {
			firstLoop = true;
		}

		if (GetAsyncKeyState(VK_F1) & 1) {
			botRunning = !botRunning;
			std::cout << "BOT TOGGLED" << std::endl;
			Sleep(200);
		}

		//IMPORTANT: if you close the console manually the game/programm you injected this into will crash/close down!
		if (GetAsyncKeyState(VK_INSERT) & 1) { //if Insert is pressed make a short beep and free the console (if used)
			if (show_console) {

				auto temp = GetConsoleWindow();
				FreeConsole();
				PostMessage(temp, WM_QUIT, 0, 0);
			}
			break;
		}
		Sleep(1);
	}

	FreeLibraryAndExitThread((HMODULE)param, 0); //erease traces of your dll (!)
	return 0;
}




BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
		if (show_console) console(hModule); //opens console if show_console is set to 1 (how? -> console.h)
	}

	case DLL_PROCESS_DETACH: {
		break;
	}
	}

	return TRUE;
}

bool isPlayerAttackingMob(Process* proc) {
	int mobUid;
	proc->memory().Read({ proc->modules().GetMainModule()->baseAddress + offsetToPlayerControlObject, offsetToAttackUID }, mobUid);

	//std::cout << "Checking if player is attacking something. Value: " << std::dec << mobUid << std::endl;

	return mobUid != 0;
}

ptr_t getOffsetOfClosestMetinStone(Process* proc, int scansize, std::vector<float> anchorPosition) {
	std::cout << "Searching for metin stones now..." << std::endl;
	ptr_t offsetOfClosestStone;
	float distanceToClosestStone = INFINITY;

	int tempMobId;
	std::vector<float> tempMobPos;
	float tempDistanceToClosestStone;


	for (int i = 0; i < scansize; i++) {
		ptr_t currentOffsetToEntity = i * 4;

		proc->memory().Read({ (proc->modules().GetMainModule()->baseAddress + offsetToMobList), currentOffsetToEntity, offsetToMobId }, tempMobId);

		// check if mob is metin stone
		if (tempMobId >= metinIdStart && tempMobId <= metinIdEnd) {
			std::cout << "Stone found, mobID: " << std::dec << tempMobId << std::endl;
			tempMobPos = getMobPos(proc, currentOffsetToEntity);
			//calculate distance to anchor position
			tempDistanceToClosestStone = sqrt(pow(anchorPosition.at(0) - tempMobPos.at(0), 2) + pow(anchorPosition.at(1) - tempMobPos.at(1), 2));
			std::cout << "Distance to stone: " << std::dec << tempDistanceToClosestStone << std::endl;
			if (tempDistanceToClosestStone < distanceToClosestStone) {
				distanceToClosestStone = tempDistanceToClosestStone;
				offsetOfClosestStone = currentOffsetToEntity;
			}
		}

	}

	return offsetOfClosestStone;
}

void enableWallhack(Process* proc) {
	proc->memory().Write({ proc->modules().GetMainModule()->baseAddress + offsetWallHackBase, offsetWallHackOne, offsetWallHackTwo }, (byte)0x1);
}

void disableWallhack(Process* proc) {
	proc->memory().Write({ proc->modules().GetMainModule()->baseAddress + offsetWallHackBase, offsetWallHackOne, offsetWallHackTwo }, (byte)0x0);
}

void playerAttackMobWithUid(Process* proc, ptr_t offsetInMobList) {

	//make mob visible
	proc->memory().Write({ proc->modules().GetMainModule()->baseAddress + offsetToMobList, offsetInMobList, offsetToMobIsVisible }, 0x1);

	int mobUid = getMobUid(proc, offsetInMobList);

	proc->memory().Write({ proc->modules().GetMainModule()->baseAddress + offsetToPlayerControlObject, offsetToAttackUID }, mobUid);
}

int getMobUid(Process* proc, ptr_t offsetInMoblist) {
	int mobUid;

	proc->memory().Read({ proc->modules().GetMainModule()->baseAddress + offsetToMobList, offsetInMoblist, offsetToMobUniqueId }, mobUid);

	return mobUid;
}

std::vector<float> getMobPos(Process* proc, ptr_t offsetInMoblist) {
	std::vector<float> mobPos;
	float xPos;
	float yPos;
	float zPos;

	proc->memory().Read({ proc->modules().GetMainModule()->baseAddress + offsetToMobList, offsetInMoblist, offsetToMobXPos }, xPos);
	proc->memory().Read({ proc->modules().GetMainModule()->baseAddress + offsetToMobList, offsetInMoblist, offsetToMobYPos }, yPos);
	proc->memory().Read({ proc->modules().GetMainModule()->baseAddress + offsetToMobList, offsetInMoblist, offsetToMobZPos }, zPos);

	//invert yPos since its stored as negative value
	yPos = yPos * -1;

	mobPos.push_back(xPos);
	mobPos.push_back(yPos);
	mobPos.push_back(zPos);

	return mobPos;
}

std::vector<float> getPlayerPos(Process* proc) {
	std::vector<float> playerPos;
	float xPos;
	float yPos;
	float zPos;

	proc->memory().Read({ proc->modules().GetMainModule()->baseAddress + offsetToPlayerBase, offsetToPlayerOne, offsetToPlayerXPos }, xPos);
	proc->memory().Read({ proc->modules().GetMainModule()->baseAddress + offsetToPlayerBase, offsetToPlayerOne, offsetToPlayerYPos }, yPos);
	proc->memory().Read({ proc->modules().GetMainModule()->baseAddress + offsetToPlayerBase, offsetToPlayerOne, offsetToPlayerZPos }, zPos);

	//invert yPos since its stored as negative value
	yPos = yPos * -1;

	playerPos.push_back(xPos);
	playerPos.push_back(yPos);
	playerPos.push_back(zPos);

	return playerPos;
}

void resetPlayerAtatck(Process* proc) {
	proc->memory().Write({ proc->modules().GetMainModule()->baseAddress + offsetToPlayerControlObject, offsetToAttackUID }, 0x0);
}
