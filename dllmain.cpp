#include "stdafx.h"
#include "console.h"

using namespace std;
#define show_console 1 //1 = show console ~ 0 = don't show console
#define CHECK_BAD_PTR(x) if(IsBadReadPtr(this,sizeof(x))) return nullptr
#define CHECK_BAD(x) if(IsBadReadPtr(this, sizeof(x))) return
#define CHECK_BAD_NUM(x) if(IsBadReadPtr(this, sizeof(x))) return 0

uintptr_t baseAdressMainMod;

//typedefs for functions
// slot logic 001, 101, 201 etc.
typedef void(__thiscall* __useItemFunc)(void* classPointer, WORD slot);
__useItemFunc useItemFunc;

typedef void(__thiscall* __pickupCloseFunc)(void* classPointer);
__pickupCloseFunc pickupCloseFunc;

//MOB STRUCT
//pointer structure: game + base offset, index(4*i), value to read
//uintptr_t offsetToMobList = 0x1CF8650;
// 0x195EE38
uintptr_t offsetToMobList = 0x1C16B78;
//int
uintptr_t offsetToMobId = 0x244;
//floats
uintptr_t offsetToMobXPos = 0x2DC;
uintptr_t offsetToMobYPos = 0x2E0;
uintptr_t offsetToMobZPos = 0x2E4;
uintptr_t offsetToMobUniqueId = 0x1C54;
//2 if alive 1 if dead
uintptr_t offsetToMobIsAlive = 0x730;
uintptr_t offsetToMobIsVisible = 0x340;

//ATTACK MOB WITH ID
//base offset
uintptr_t offsetToPlayerControlObject = 0x1CF8528;
//int
uintptr_t offsetToAttackUID = 0x50;

//WALLHACK
//base offset
uintptr_t offsetWallHackBase = 0x1CF853C;
//byte
uintptr_t offsetWallHackOne = 0xC;
uintptr_t offsetWallHackTwo = 0x1C5C;

//PLAYER
uintptr_t offsetToPlayerBase = 0x1C6C354;
uintptr_t offsetToPlayerOne = 0xC;
//float
uintptr_t offsetToPlayerXPos = 0x2DC;
uintptr_t offsetToPlayerYPos = 0x2E0;
uintptr_t offsetToPlayerZPos = 0x2E4;

//ids
int metinIdStart = 8000;
int metinIdEnd = 8112;

bool botRunning = false;
bool firstLoop = true;

std::vector<float> getPlayerPos();
std::vector<float> getMobPos(uintptr_t offsetInMoblist);
uintptr_t getOffsetOfClosestMetinStone(int scansize, std::vector<float> anchorPosition);
int getMobUid(uintptr_t offsetInMoblist);
void playerAttackMobWithUid(uintptr_t offsetInMobList);
void enableWallhack();
void disableWallhack();
bool isPlayerAttackingMob();
void resetPlayerAtatck();
uintptr_t findDMAAddy(uintptr_t ptr, vector<uintptr_t> offsets);
bool IsBadReadPtr(void* p);


DWORD WINAPI MainThread(LPVOID param) {
	baseAdressMainMod = (uintptr_t)GetModuleHandle(NULL);

	cout << "Main module base address: 0x" << std::hex << baseAdressMainMod << endl;

	// set the address of the function (relative to modbase)
	useItemFunc = (__useItemFunc)(baseAdressMainMod + 0x1695B0);
	pickupCloseFunc = (__pickupCloseFunc)(baseAdressMainMod + 0xCAD10);
	// pickupCloseFunc(*(void**)0x020F8528);
	// useItemFunc(*(void**)0x20F8560, 0x101);

	std::vector<float> anchorPosition;
	uintptr_t offsetClosestStone;
	std::chrono::steady_clock::time_point timerStart;
	std::chrono::steady_clock::time_point timerRound;

	while (true) {


		// bot loop
		// check if bot enabled
		if (botRunning) {

			if (firstLoop) {
				cout << "FirstLoop setup is run..." << endl;
				Sleep(5000);
				// enable wallhack
				enableWallhack();
				cout << "WH enabled" << endl;
				// get Anchor position
				anchorPosition = getPlayerPos();
				cout << "Anchor position set" << endl;

				firstLoop = false;
			}

			if (!isPlayerAttackingMob()) {
				Sleep(1000);
				// pickup and wait for short amount
				pickupCloseFunc(*(void**)0x020F8528);
				cout << "Picked up loot" << endl;
				// sleeps are neccessary because stone will be read as alive short time after death
				Sleep(3000);

				// get closest stone
				offsetClosestStone = getOffsetOfClosestMetinStone(1500, anchorPosition);
				// attack stone
				playerAttackMobWithUid(offsetClosestStone);
				// start timing to detect attackid bug
				timerStart = std::chrono::steady_clock::now();
				cout << "Attacking NOW..." << endl;
			}

			timerRound = std::chrono::steady_clock::now();

			if (std::chrono::duration_cast<std::chrono::seconds>(timerRound - timerStart).count() > 60) {
				// 60 seconds ago we started attacking and stone still lives, game is buggy
				resetPlayerAtatck();
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

bool isPlayerAttackingMob() {
	int* mobUidAddress = (int*)findDMAAddy(baseAdressMainMod + offsetToPlayerControlObject, { offsetToAttackUID });
	
	if (IsBadReadPtr(mobUidAddress)) {
		return 0;
	}

	//cout << "Checking if player is attacking something. Value: " << std::dec << mobUid << endl;
	return *mobUidAddress != 0;
}

uintptr_t getOffsetOfClosestMetinStone(int scansize, std::vector<float> anchorPosition) {
	cout << "Searching for metin stones now..." << endl;
	uintptr_t offsetOfClosestStone = 0x0;
	float distanceToClosestStone = INFINITY;

	int tempMobId;
	int * tempMobIdAddress;
	std::vector<float> tempMobPos;
	float tempDistanceToClosestStone;


	for (int i = 0; i < scansize; i++) {
		uintptr_t currentOffsetToEntity = i * 4;



		tempMobIdAddress = (int*)findDMAAddy(baseAdressMainMod + offsetToMobList, { currentOffsetToEntity, offsetToMobId });
		if (IsBadReadPtr(tempMobIdAddress)) {
			tempMobId = 0;
		}
		else {
			tempMobId = *tempMobIdAddress;
		}


		// check if mob is metin stone
		if (tempMobId >= metinIdStart && tempMobId <= metinIdEnd) {
			cout << "Stone found, mobID: " << std::dec << tempMobId << endl;
			tempMobPos = getMobPos(currentOffsetToEntity);
			//calculate distance to anchor position
			tempDistanceToClosestStone = sqrt(pow(anchorPosition.at(0) - tempMobPos.at(0), 2) + pow(anchorPosition.at(1) - tempMobPos.at(1), 2));
			cout << "Distance to stone: " << std::dec << tempDistanceToClosestStone << endl;
			if (tempDistanceToClosestStone < distanceToClosestStone) {
				distanceToClosestStone = tempDistanceToClosestStone;
				offsetOfClosestStone = currentOffsetToEntity;
			}
		}

	}

	return offsetOfClosestStone;
}

void enableWallhack() {
	BYTE* wallHackAddress = (BYTE*)findDMAAddy(baseAdressMainMod + offsetWallHackBase, { offsetWallHackOne, offsetWallHackTwo });
	*wallHackAddress = 0x1;
}

void disableWallhack() {
	BYTE* wallHackAddress = (BYTE*)findDMAAddy(baseAdressMainMod + offsetWallHackBase, { offsetWallHackOne, offsetWallHackTwo });
	*wallHackAddress = 0x0;
}

void playerAttackMobWithUid(uintptr_t offsetInMobList) {

	// make mob visible
	BYTE* isVisibleAddress = (BYTE*)findDMAAddy(baseAdressMainMod + offsetToMobList, { offsetInMobList, offsetToMobIsVisible });
	*isVisibleAddress = 0x1;

	int mobUid = getMobUid(offsetInMobList);

	// attack mob
	int* attackMobAddress = (int*)findDMAAddy(baseAdressMainMod + offsetToPlayerControlObject, { offsetToAttackUID });
	*attackMobAddress = mobUid;
}

int getMobUid(uintptr_t offsetInMoblist) {
	int* mobUidAddress = (int*)findDMAAddy(baseAdressMainMod + offsetToMobList, { offsetInMoblist,  offsetToMobUniqueId });

	return *mobUidAddress;
}

std::vector<float> getMobPos(uintptr_t offsetInMoblist) {
	std::vector<float> mobPos;
	float* xPosAddress;
	float* yPosAddress;
	float* zPosAddress;

	xPosAddress = (float*)findDMAAddy(baseAdressMainMod + offsetToMobList, { offsetInMoblist,  offsetToMobXPos });
	yPosAddress = (float*)findDMAAddy(baseAdressMainMod + offsetToMobList, { offsetInMoblist,  offsetToMobYPos });
	zPosAddress = (float*)findDMAAddy(baseAdressMainMod + offsetToMobList, { offsetInMoblist,  offsetToMobZPos });


	//invert yPos since its stored as negative value

	mobPos.push_back(*xPosAddress);
	mobPos.push_back(*yPosAddress * -1);
	mobPos.push_back(*zPosAddress);

	return mobPos;
}

std::vector<float> getPlayerPos() {
	std::vector<float> playerPos;
	float* xPosAddress;
	float* yPosAddress;
	float* zPosAddress;

	xPosAddress = (float*)findDMAAddy(baseAdressMainMod + offsetToPlayerBase, { offsetToPlayerOne,  offsetToPlayerXPos });
	yPosAddress = (float*)findDMAAddy(baseAdressMainMod + offsetToPlayerBase, { offsetToPlayerOne,  offsetToPlayerYPos });
	zPosAddress = (float*)findDMAAddy(baseAdressMainMod + offsetToPlayerBase, { offsetToPlayerOne,  offsetToPlayerZPos });


	//invert yPos since its stored as negative value

	playerPos.push_back(*xPosAddress);
	playerPos.push_back(*yPosAddress * -1);
	playerPos.push_back(*zPosAddress);

	return playerPos;
}

void resetPlayerAtatck() {

	int* playerAttackAddress = (int*)findDMAAddy(baseAdressMainMod + offsetToPlayerControlObject, { offsetToAttackUID });

	*playerAttackAddress = 0x0;
}

uintptr_t findDMAAddy(uintptr_t ptr, vector<uintptr_t> offsets)
{
	uintptr_t addr = ptr;

	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}


	return addr;
}

bool IsBadReadPtr(void* p)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (::VirtualQuery(p, &mbi, sizeof(mbi)))
	{
		DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
		bool b = !(mbi.Protect & mask);
		// check the page is not a guard page
		if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) b = true;

		return b;
	}
	return true;
}
