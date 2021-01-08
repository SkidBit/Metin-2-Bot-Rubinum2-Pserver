#pragma once
#include "stdafx.h"
#include "mem.h"
#include "constants.h"

namespace game {
	Entity* getPlayerEntity();
	Entity* getClosestMetinStone(Vector3 anchorPosition);
	bool playerHasWallhackEnabled();
	void playerEnableWallhack();
	void playerDisableWallhack();
	bool areOtherPlayersPresent();
	void playerPickupItems();
	void initializePickupFunctionAddresses();
	float getDistanceBetweenEntities(Entity* firstEntity, Entity* secondEntity);
	float getDistanceBetweenEntityAndVec3(Entity* entity, Vector3 anchorPosition);
	void initializeAttackEntityFunctionAddresses();
	void playerAttackEntity(Entity* entity);
	void playerAttackVID(DWORD VID);
	void initializeCpythonPlayerSingleton();
	TCharacterInstanceMap getEntityMap();
	void playerSellFirstStonePage();
	void playerSellItem(int16_t slot, int amount);
	Vector3 getPlayerPosition();
	bool isPlayerAttacking();
}