#pragma once
#include "stdafx.h"
#include "mem.h"

namespace game {
	bool isPlayerAttackingMob();
	uintptr_t getPointerOfClosestMetinStone(std::vector<float> anchorPosition);
	void enableWallhack();
	void disableWallhack();
	void playerAttackMobWithUid(uintptr_t pointerToMob);
	int getMobUid(uintptr_t pointerToMob);
	std::vector<float> getMobPos(uintptr_t pointerToMob);
	std::vector<float> getPlayerPos();
	void resetPlayerAtatck();
}