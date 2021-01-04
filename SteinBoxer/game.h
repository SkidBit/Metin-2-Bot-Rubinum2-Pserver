#pragma once
#include "stdafx.h"
#include "mem.h"
#include "constants.h"

namespace game {
	Player* getPlayerEntity();
	float getDistanceBetweenEntityAndPlayer(Player* player, Entity* secondEntity);
	Entity* getClosestMetinStone(Vector3 anchorPosition);
	void enableWallhack();
	void disableWallhack();
	void resetPlayerAtatck();
	bool areOtherPlayersPresent();
	void pickupItems();
	void initializePickupFunctionAddresses();
	void flushEntityArray();
	float getDistanceBetweenEntities(Entity* firstEntity, Entity* secondEntity);
	float getDistanceBetweenEntityAndVec3(Entity* entity, Vector3 anchorPosition);
	void initializeAttackEntityFunctionAddresses();
	void attackEntity(Entity* entity);
	void attackVID(DWORD VID);
	void initializeCpythonPlayerSingleton();
}