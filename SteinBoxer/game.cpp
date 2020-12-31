#include "stdafx.h"
#include "game.h"
#include "constants.h"
using namespace std;

bool game::isPlayerAttackingMob() {
	int* mobUidAddress = (int*)mem::findDMAAddy(baseAdressMainMod + offsetToPlayerControlObject, { offsetToAttackUID });

	if (mem::IsBadReadPtr(mobUidAddress)) {
		return 0;
	}

	//cout << "Checking if player is attacking something. Value: " << std::dec << mobUid << endl;
	return *mobUidAddress != 0;
}

uintptr_t game::getPointerOfClosestMetinStone(int scansize, vector<float> anchorPosition) {
	cout << "Searching for metin stones now..." << endl;

	uintptr_t pointerToClosestStone = 0x0;
	float distanceToClosestStone = INFINITY;
	int tempMobId;
	vector<float> tempMobPos;
	float tempDistanceToClosestStone;


	for (int i = 0; i < 255; i++) {
		if (entities[i] != 0) {
			if (mem::IsBadReadPtr((uintptr_t*)entities[i])) {
				tempMobId = 0;
			}
			else {
				//cout << "mobObj address: 0x" << hex << entities[i] << " offset: 0x" << offsetToMobId <<  " mobId address: 0x" << entities[i] + offsetToMobId << endl;
				tempMobId = *(int*)(entities[i] + offsetToMobId);

			}

			// check if mob is metin stone
			if (tempMobId >= metinIdStart && tempMobId <= metinIdEnd) {
				cout << "Stone found, mobID: " << dec << tempMobId << endl;
				
				tempMobPos = game::getMobPos(entities[i]);
				//calculate distance to anchor position
				tempDistanceToClosestStone = sqrt(pow(anchorPosition.at(0) - tempMobPos.at(0), 2) + pow(anchorPosition.at(1) - tempMobPos.at(1), 2));
				cout << "Distance to stone: " << dec << tempDistanceToClosestStone << endl;
				if (tempDistanceToClosestStone < distanceToClosestStone) {
					distanceToClosestStone = tempDistanceToClosestStone;
					pointerToClosestStone = entities[i];
				}
			}

		}
	}

	return pointerToClosestStone;
}

void game::enableWallhack() {
	BYTE* wallHackAddress = (BYTE*)mem::findDMAAddy(baseAdressMainMod + offsetWallHackBase, { offsetWallHackOne, offsetWallHackTwo });
	if (!mem::IsBadReadPtr(wallHackAddress)) {
		*wallHackAddress = 0x1;
	}

}

void game::disableWallhack() {
	BYTE* wallHackAddress = (BYTE*)mem::findDMAAddy(baseAdressMainMod + offsetWallHackBase, { offsetWallHackOne, offsetWallHackTwo });
	if (!mem::IsBadReadPtr(wallHackAddress)) {
		*wallHackAddress = 0x0;
	}
}

void game::playerAttackMobWithUid(uintptr_t pointerToMobEntity) {

	// make mob visible
	BYTE* isVisibleAddress = (BYTE*)(pointerToMobEntity + offsetToMobIsVisible);
	if (!mem::IsBadReadPtr(isVisibleAddress)) {
		*isVisibleAddress = 0x1;
	}

	int mobUid = getMobUid(pointerToMobEntity);

	// attack mob
	int* attackMobAddress = (int*)mem::findDMAAddy(baseAdressMainMod + offsetToPlayerControlObject, { offsetToAttackUID });
	if (!mem::IsBadReadPtr(attackMobAddress)) {
		*attackMobAddress = mobUid;
	}
}

int game::getMobUid(uintptr_t pointerToMob) {
	int* mobUidAddress = (int*)(pointerToMob + offsetToMobUniqueId);

	if (!mem::IsBadReadPtr(mobUidAddress)) {
		return *mobUidAddress;
	}

	return 0;
}

vector<float> game::getMobPos(uintptr_t entityPointer) {
	vector<float> mobPos;
	float* xPosAddress;
	float* yPosAddress;
	float* zPosAddress;

	xPosAddress = (float*)(entityPointer + offsetToMobXPos);
	yPosAddress = (float*)(entityPointer + offsetToMobYPos);
	zPosAddress = (float*)(entityPointer + offsetToMobZPos);

	if (!mem::IsBadReadPtr(xPosAddress) && !mem::IsBadReadPtr(yPosAddress) && !mem::IsBadReadPtr(zPosAddress)) {
		mobPos.push_back(*xPosAddress);
		mobPos.push_back(*yPosAddress * -1);
		mobPos.push_back(*zPosAddress);
	}
	else {
		mobPos.push_back(0);
		mobPos.push_back(0);
		mobPos.push_back(0);
	}

	return mobPos;
}

vector<float> game::getPlayerPos() {
	vector<float> playerPos;
	float* xPosAddress;
	float* yPosAddress;
	float* zPosAddress;

	xPosAddress = (float*)mem::findDMAAddy(baseAdressMainMod + offsetToPlayerBase, { offsetToPlayerOne,  offsetToPlayerXPos });
	yPosAddress = (float*)mem::findDMAAddy(baseAdressMainMod + offsetToPlayerBase, { offsetToPlayerOne,  offsetToPlayerYPos });
	zPosAddress = (float*)mem::findDMAAddy(baseAdressMainMod + offsetToPlayerBase, { offsetToPlayerOne,  offsetToPlayerZPos });

	if (!mem::IsBadReadPtr(xPosAddress) && !mem::IsBadReadPtr(yPosAddress) && !mem::IsBadReadPtr(zPosAddress)) {
		playerPos.push_back(*xPosAddress);
		playerPos.push_back(*yPosAddress * -1);
		playerPos.push_back(*zPosAddress);
	}

	playerPos.push_back(0);
	playerPos.push_back(0);
	playerPos.push_back(0);

	return playerPos;
}

void game::resetPlayerAtatck() {

	int* playerAttackAddress = (int*)mem::findDMAAddy(baseAdressMainMod + offsetToPlayerControlObject, { offsetToAttackUID });

	if (!mem::IsBadReadPtr(playerAttackAddress)) {
		*playerAttackAddress = 0x0;
	}
}
