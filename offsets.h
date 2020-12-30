#pragma once
#include "stdafx.h"

//MOB STRUCT
//pointer structure: game + base offset, index(4*i), value to read
//uintptr_t offsetToMobList = 0x1CF8650;
// 0x195EE38
const uintptr_t offsetToMobList = 0x1C16B78;
//int
const uintptr_t offsetToMobId = 0x244;
//floats
const uintptr_t offsetToMobXPos = 0x2DC;
const uintptr_t offsetToMobYPos = 0x2E0;
const uintptr_t offsetToMobZPos = 0x2E4;
const uintptr_t offsetToMobUniqueId = 0x1C54;
//2 if alive 1 if dead
const uintptr_t offsetToMobIsAlive = 0x730;
const uintptr_t offsetToMobIsVisible = 0x340;

//ATTACK MOB WITH ID
//base offset
const uintptr_t offsetToPlayerControlObject = 0x1CF8528;
//int
const uintptr_t offsetToAttackUID = 0x50;

//WALLHACK
//base offset
const uintptr_t offsetWallHackBase = 0x1CF853C;
//byte
const uintptr_t offsetWallHackOne = 0xC;
const uintptr_t offsetWallHackTwo = 0x1C5C;

//PLAYER
const uintptr_t offsetToPlayerBase = 0x1C6C354;
const uintptr_t offsetToPlayerOne = 0xC;
//float
const uintptr_t offsetToPlayerXPos = 0x2DC;
const uintptr_t offsetToPlayerYPos = 0x2E0;
const uintptr_t offsetToPlayerZPos = 0x2E4;

//ids
const int metinIdStart = 8000;
const int metinIdEnd = 8112;


extern uintptr_t baseAdressMainMod;