#include "stdafx.h"
#include "game.h"
#include "constants.h"
using namespace std;

typedef void(__thiscall* __pickupCloseFunc)(void* classPointer);
bool pickupFunctionAddressesInitialized = false;
uintptr_t pickupFunctionAddress;
__pickupCloseFunc pickupCloseFunc;

bool cPythonPlayerSingletonInitialized = false;
uintptr_t cPythonPlayerSingletonPointerFunctionAddress;
uintptr_t cPythonPlayerSingletonPointer;

bool attackEntityAddressInitialized = false;
typedef void(__thiscall* __attackEntityFunc)(void* classPointer, DWORD VID);
__attackEntityFunc attackEntityFunc;
uintptr_t attackEntityAddress;

bool playerEntityFunctionsInitialized = false;
typedef uintptr_t(__thiscall* __getMainInstancePtr)(void* classPointer);
__getMainInstancePtr getMainInstancePtr;
uintptr_t getMainInstancePtrClassPointerFunctionAddress;
uintptr_t getMainInstancePtrClassPointer;
uintptr_t getMainInstancePtrFunctionAddress;

uintptr_t recvDeadPacketFunctionAddress;
uintptr_t* tCharacterInstanceMapPointer;
uintptr_t tCharacterInstanceMapAddress;
bool tCharacterInstanceMapInitialized = false;

TCharacterInstanceMap game::getEntityMap() {
	if (!tCharacterInstanceMapInitialized) {
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
		getMainInstancePtrClassPointerFunctionAddress = (uintptr_t)mem::ScanModIn((char*)getMainInstancePtrClassPointerPattern, (char*)getMainInstancePtrClassPointerMask, "rbclient.exe");
		getMainInstancePtrClassPointer = *(uintptr_t*)(getMainInstancePtrClassPointerFunctionAddress + getMainInstancePtrClassPointerOffset);
		getMainInstancePtrFunctionAddress = (uintptr_t)mem::ScanModIn((char*)getMainInstancePtrPattern, (char*)getMainInstancePtrMask, "rbclient.exe");
		getMainInstancePtr = (__getMainInstancePtr)(getMainInstancePtrFunctionAddress);
		playerEntityFunctionsInitialized = true;
	}
	return (Entity*)getMainInstancePtr(*(void**)getMainInstancePtrClassPointer);
}

void game::initializeCpythonPlayerSingleton() {
	cPythonPlayerSingletonPointerFunctionAddress = (uintptr_t)mem::ScanModIn((char*)cPythonPlayerSingletonPointerFunctionPattern, (char*)cPythonPlayerSingletonPointerFunctionMask, "rbclient.exe");
	cPythonPlayerSingletonPointer = *(uintptr_t*)(cPythonPlayerSingletonPointerFunctionAddress + cPythonPlayerSingletonPointerOffsetToClassPointer);
	cPythonPlayerSingletonInitialized = true;
}

void game::initializeAttackEntityFunctionAddresses() {
	attackEntityAddress = (uintptr_t)mem::ScanModIn((char*)attackEntityPattern, (char*)attackEntityMask, "rbclient.exe");
	attackEntityFunc = (__attackEntityFunc)(attackEntityAddress);
	attackEntityAddressInitialized = true;
}

void game::attackVID(DWORD VID) {

	if (!attackEntityAddressInitialized) {
		game::initializeAttackEntityFunctionAddresses();
	}
	if (!cPythonPlayerSingletonInitialized) {
		game::initializeCpythonPlayerSingleton();
	}

	attackEntityFunc(*(void**)cPythonPlayerSingletonPointer, VID);
}

void game::attackEntity(Entity* entity) {

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
	pickupFunctionAddress = (uintptr_t)mem::ScanModIn((char*)pickupFunctionPattern, (char*)pickupFunctionMask, "rbclient.exe");
	pickupCloseFunc = (__pickupCloseFunc)(pickupFunctionAddress);
	pickupFunctionAddressesInitialized = true;
}


void game::pickupItems() {

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

void game::enableWallhack() {
	game::getPlayerEntity()->enableSkipCollision();
}

void game::disableWallhack() {
	game::getPlayerEntity()->disableSkipCollision();
}

void game::resetPlayerAtatck() {
	game::attackVID(0);
}