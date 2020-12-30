#pragma once
#include "stdafx.h"
#include "mem.h"

namespace game {
	bool isPlayerAttackingMob();
	uintptr_t getOffsetOfClosestMetinStone(int scansize, std::vector<float> anchorPosition);
	void enableWallhack();
	void disableWallhack();
	void playerAttackMobWithUid(uintptr_t offsetInMobList);
	int getMobUid(uintptr_t offsetInMoblist);
	std::vector<float> getMobPos(uintptr_t offsetInMoblist);
	std::vector<float> getPlayerPos();
	void resetPlayerAtatck();
}