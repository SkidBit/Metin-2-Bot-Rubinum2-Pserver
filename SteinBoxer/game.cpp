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

void game::flushEntityArray() {
	for (int i = 0; i < 254; i++) {
		entities[i] = 0;
	}
}

Entity* game::getPlayerEntity() {
	for (int i = 0; i < 254; i++) {
		if (entities[i] != 0) {
			if (entities[i]->getIsOwnPlayer() == 1 && entities[i]->getIsPlayerCharacter() == playerIdentifier) {
				//cout << "found own player: " << hex << entities[i] << endl;
				return entities[i];
			}
		}
	}
	return 0;
}


bool game::areOtherPlayersPresent() {

	int playerCount = 0;

	for (int i = 0; i < 255; i++) {
		if (entities[i] != 0) {
			if (entities[i]->getIsPlayerCharacter() == playerIdentifier && entities[i]->getIsOwnPlayer() == 0) {
				playerCount++;
				//cout << "some player found at: 0x" << hex << entities[i] << endl;
			}
		}
	}
	return playerCount > 0;
}

Entity* game::getClosestMetinStone(Vector3 anchorPosition) {
	cout << "Searching for metin stones now..." << endl;

	Entity* closestMetinStone = nullptr;
	float distanceToClosestStone = INFINITY;
	int tempMobId;
	float tempDistanceToClosestStone;
	int stoneCount = 0;

	for (int i = 0; i < 255; i++) {
		if (entities[i] != 0) {
			
			tempMobId = entities[i]->getMobId();

			// check if mob is metin stone
			if (tempMobId >= metinIdStart && tempMobId <= metinIdEnd || tempMobId >= oreIdStart && tempMobId <= oreIdEnd) {
				//cout << "Stone found, mobID: " << dec << tempMobId << endl;
				//cout << "Stone found, mobAddress: 0x" << hex << entities[i] << endl;
				stoneCount++;

				tempDistanceToClosestStone = game::getDistanceBetweenEntityAndVec3(entities[i], anchorPosition);
				cout << "Distance to stone/ore: " << dec << tempDistanceToClosestStone << endl;
				if (tempDistanceToClosestStone < distanceToClosestStone) {
					distanceToClosestStone = tempDistanceToClosestStone;
					closestMetinStone = entities[i];
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
