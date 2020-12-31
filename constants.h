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

inline const char* pickupFunctionPattern = "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\x00\x00\x00\x00\x00\x00\x00\x00\x00\x53\x56\x57\xA1\x00\x00\x00\x00\x00\x00\x00\x00\x45\x00\x64\x00\x00\x00\x00\x00\x00\x00\x00\x00\xF0\x8D\x00\x00\x8B\x01";
inline const char* pickupFunctionMask = "xxxx?x????x?????????xxxx????????x?x?????????xx??xx";

// address of function that moves classpointer, skip 2 bytes to get to the pointer
inline const char* pickupFunctionClassPointerFunctionPattern = "\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x55\x8B\xEC\x51\x8D\x45\x00\x50\x6A\x00\xFF\x75\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x84\xC0\x75\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\xFF\x75\x00\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x51\x8D\x45\x00\x50\x6A\x00\xFF\x75\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x84\xC0\x75\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\x83\x7D\xFC";
inline const char* pickupFunctionClassPointerFunctionMask = "xx????x????x????xxxxxx?xx?xx?x????xx?xxx?x????xxxxxx?xx????x????x????xxxxxxxxxxxxxxxxx?xx?xx?x????xx?xxx?x????xxxxxxx";
inline const BYTE pickupFunctionOffsetToClassPointer = 0x2;