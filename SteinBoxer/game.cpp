#include "stdafx.h"
#include "game.h"
#include "constants.h"
using namespace std;

typedef void(__thiscall* __pickupCloseFunc)(void* classPointer);
bool pickupFunctionAddressesInitialized = false;
__pickupCloseFunc pickupCloseFunc;

bool cPythonPlayerSingletonInitialized = false;
uintptr_t cPythonPlayerSingletonPointer;

bool attackEntityAddressInitialized = false;
typedef void(__thiscall* __attackEntityFunc)(void* classPointer, DWORD VID);
__attackEntityFunc attackEntityFunc;

bool playerEntityFunctionsInitialized = false;
typedef uintptr_t(__thiscall* __getMainInstancePtr)(void* classPointer);
__getMainInstancePtr getMainInstancePtr;

uintptr_t getMainInstancePtrClassPointer;
uintptr_t tCharacterInstanceMapAddress;
bool tCharacterInstanceMapInitialized = false;

uintptr_t cPythonNetworkStreamPointer;
bool sellStonesInitialized = false;
typedef void(__thiscall* __sellItemFunc)(void* classPointer, int16_t slot, int amount, char sellWithoutNpc);
__sellItemFunc sellItemFunc;


void game::playerSellFirstStonePage() {
	for (int i = 0; i < 45; i++) {
		// following the 0xB, 0x10B, 0x20B... pattern
		game::playerSellItem(i*256+11,500);
		// maybe this is a bit too fast
		Sleep(100);
	}
}

void game::playerSellItem(int16_t slot, int amount) {

	if (!sellStonesInitialized) {
		uintptr_t cPythonNetworkStreamPointerAddress;
		uintptr_t sendShopSellPacketAddress;

		cPythonNetworkStreamPointerAddress = (uintptr_t)mem::ScanModIn((char*)cPythonNetworkStreamPointerPattern, (char*)cPythonNetworkStreamPointerMask, "rbclient.exe");
		cPythonNetworkStreamPointer = *(uintptr_t*)(cPythonNetworkStreamPointerAddress + cPythonNetworkStreamPointerOfset);
		//cout << "cPythonNetworkStreamPointer " << hex << cPythonNetworkStreamPointer << endl;
		sendShopSellPacketAddress = (uintptr_t)mem::ScanModIn((char*)sendShopSellPacketPattern, (char*)sendShopSellPacketMask, "rbclient.exe");
		//cout << "sendShopSellPacketAddress " << hex << sendShopSellPacketAddress << endl;
		sellItemFunc = __sellItemFunc(sendShopSellPacketAddress);

		sellStonesInitialized = true;
	}
	sellItemFunc(*(void**)cPythonNetworkStreamPointer, slot, amount, 0x1);
}

TCharacterInstanceMap game::getEntityMap() {
	if (!tCharacterInstanceMapInitialized) {
		uintptr_t recvDeadPacketFunctionAddress;
		uintptr_t* tCharacterInstanceMapPointer;

		recvDeadPacketFunctionAddress = (uintptr_t)mem::ScanModIn((char*)recvDeadPacketFunctionPattern, (char*)recvDeadPacketFunctionMask, "rbclient.exe");
		tCharacterInstanceMapPointer = *(uintptr_t**)(recvDeadPacketFunctionAddress + offsetTocPythonCharacterManagerPointer);
		tCharacterInstanceMapAddress = *tCharacterInstanceMapPointer;
		tCharacterInstanceMapAddress += offsetToTCharacterInstanceMap;
		tCharacterInstanceMapInitialized = true;
	}
	return *(TCharacterInstanceMap*)tCharacterInstanceMapAddress;
}

Entity* game::getPlayerEntity() {
	if (!playerEntityFunctionsInitialized) {
		uintptr_t getMainInstancePtrClassPointerFunctionAddress;
		uintptr_t getMainInstancePtrFunctionAddress;

		getMainInstancePtrClassPointerFunctionAddress = (uintptr_t)mem::ScanModIn((char*)getMainInstancePtrClassPointerPattern, (char*)getMainInstancePtrClassPointerMask, "rbclient.exe");
		getMainInstancePtrClassPointer = *(uintptr_t*)(getMainInstancePtrClassPointerFunctionAddress + getMainInstancePtrClassPointerOffset);
		getMainInstancePtrFunctionAddress = (uintptr_t)mem::ScanModIn((char*)getMainInstancePtrPattern, (char*)getMainInstancePtrMask, "rbclient.exe");
		getMainInstancePtr = (__getMainInstancePtr)(getMainInstancePtrFunctionAddress);
		playerEntityFunctionsInitialized = true;
	}
	return (Entity*)getMainInstancePtr(*(void**)getMainInstancePtrClassPointer);
}

void game::initializeCpythonPlayerSingleton() {
	uintptr_t cPythonPlayerSingletonPointerFunctionAddress;

	cPythonPlayerSingletonPointerFunctionAddress = (uintptr_t)mem::ScanModIn((char*)cPythonPlayerSingletonPointerFunctionPattern, (char*)cPythonPlayerSingletonPointerFunctionMask, "rbclient.exe");
	cPythonPlayerSingletonPointer = *(uintptr_t*)(cPythonPlayerSingletonPointerFunctionAddress + cPythonPlayerSingletonPointerOffsetToClassPointer);
	cPythonPlayerSingletonInitialized = true;
}

void game::initializeAttackEntityFunctionAddresses() {
	uintptr_t attackEntityAddress;

	attackEntityAddress = (uintptr_t)mem::ScanModIn((char*)attackEntityPattern, (char*)attackEntityMask, "rbclient.exe");
	attackEntityFunc = (__attackEntityFunc)(attackEntityAddress);
	attackEntityAddressInitialized = true;
}

void game::playerAttackVID(DWORD VID) {

	if (!attackEntityAddressInitialized) {
		game::initializeAttackEntityFunctionAddresses();
	}
	if (!cPythonPlayerSingletonInitialized) {
		game::initializeCpythonPlayerSingleton();
	}

	attackEntityFunc(*(void**)cPythonPlayerSingletonPointer, VID);
}

void game::playerAttackEntity(Entity* entity) {

	if (!attackEntityAddressInitialized) {
		game::initializeAttackEntityFunctionAddresses();
	}
	if (!cPythonPlayerSingletonInitialized) {
		game::initializeCpythonPlayerSingleton();
	}

	// otherwise we can't attack mobs outside of our vision
	entity->setIsRendered(0x1);

	attackEntityFunc(*(void**)cPythonPlayerSingletonPointer, entity->getUid());
}

void game::initializePickupFunctionAddresses() {
	uintptr_t pickupFunctionAddress;

	pickupFunctionAddress = (uintptr_t)mem::ScanModIn((char*)pickupFunctionPattern, (char*)pickupFunctionMask, "rbclient.exe");
	pickupCloseFunc = (__pickupCloseFunc)(pickupFunctionAddress);
	pickupFunctionAddressesInitialized = true;
}

void game::playerPickupItems() {

	if (!pickupFunctionAddressesInitialized) {
		game::initializePickupFunctionAddresses();
	}
	if (!cPythonPlayerSingletonInitialized) {
		game::initializeCpythonPlayerSingleton();
	}
	pickupCloseFunc(*(void**)cPythonPlayerSingletonPointer);
}

bool game::areOtherPlayersPresent() {
	int playerCount = 0;
	TCharacterInstanceMap entities = game::getEntityMap();

	for (TCharacterInstanceMap::iterator itor = entities.begin(); itor != entities.end(); itor++) {
		if (itor->second->getIsPlayerCharacter() == playerIdentifier) {
			playerCount++;
			//cout << "some player found at: 0x" << hex << entities[i] << endl;
		}
	}

	return playerCount > 1;
}

Entity* game::getClosestMetinStone(Vector3 anchorPosition) {
	cout << "Searching for metin stones now..." << endl;

	TCharacterInstanceMap entities = game::getEntityMap();
	Entity* closestMetinStone = nullptr;
	float distanceToClosestStone = INFINITY;
	int tempMobId;
	float tempDistanceToClosestStone;
	int stoneCount = 0;

	for (TCharacterInstanceMap::iterator itor = entities.begin(); itor != entities.end(); itor++) {
		tempMobId = itor->second->getMobId();

		// check if mob is metin stone
		if (tempMobId >= metinIdStart && tempMobId <= metinIdEnd || tempMobId >= oreIdStart && tempMobId <= oreIdEnd) {
			stoneCount++;

			// check if stone is not on blacklist
			if (find(blacklistedUids.begin(), blacklistedUids.end(), itor->first) != blacklistedUids.end()) {
				cout << "[i] Blacklisted stone gets ignored..." << endl;
			}
			else {
				tempDistanceToClosestStone = game::getDistanceBetweenEntityAndVec3(itor->second, anchorPosition);
				cout << "Distance to stone/ore: " << dec << tempDistanceToClosestStone << endl;

				if (tempDistanceToClosestStone < distanceToClosestStone) {
					distanceToClosestStone = tempDistanceToClosestStone;
					closestMetinStone = itor->second;
					//cout << "Stone found, mobID: " << dec << tempMobId << endl;
					//cout << "Stone found, mobAddress: 0x" << hex << entities[i] << endl;
				}
			}
		}
	}

	cout << "[i] I found " << dec << stoneCount << " stones." << endl;
	cout << "[i] Closest stone is " << dec << distanceToClosestStone << " units away from us." << endl;

	return closestMetinStone;
}

float game::getDistanceBetweenEntityAndVec3(Entity* entity, Vector3 anchorPosition) {
	return sqrt(pow(entity->getPosition().x - anchorPosition.x, 2) + pow(entity->getPosition().y - anchorPosition.y, 2));
}

float game::getDistanceBetweenEntities(Entity* firstEntity, Entity* secondEntity) {
	return sqrt(pow(firstEntity->getPosition().x - secondEntity->getPosition().x, 2) + pow(firstEntity->getPosition().y - secondEntity->getPosition().y, 2));
}

void game::playerEnableWallhack() {
	game::getPlayerEntity()->enableSkipCollision();
}

void game::playerDisableWallhack() {
	game::getPlayerEntity()->disableSkipCollision();
}

bool game::playerHasWallhackEnabled() {
	return game::getPlayerEntity()->getSkipCollision() == 1;
}

Vector3 game::getPlayerPosition() {
	return game::getPlayerEntity()->getPosition();
}

bool game::isPlayerAttacking() {
	return game::getPlayerEntity()->getAttackStance() != 0;
}