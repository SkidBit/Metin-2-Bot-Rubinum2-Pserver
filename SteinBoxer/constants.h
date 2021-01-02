#pragma once
#include "stdafx.h"

#define CHECK_BAD_PTR(x) if(IsBadReadPtr(this,sizeof(x))) return nullptr
#define CHECK_BAD(x) if(IsBadReadPtr(this, sizeof(x))) return
#define CHECK_BAD_NUM(x) if(IsBadReadPtr(this, sizeof(x))) return 0
#define CHECK_BAD_VEC3(x) if(IsBadReadPtr(this, sizeof(x))) return Vector3{0,0,0}

struct Vector3 { float x, y, z; };

class Entity
{
public:
	char pad_0000[4]; //0x0000
	Vector3 position; //0x0004
	int32_t isPlayerCharacter; //0x0010 // if player 3F800000
	char pad_0014[84]; //0x0014
	int32_t isRendered; //0x0068
	char pad_006C[932]; //0x006C
	int32_t mobId; //0x0410
	char pad_0414[68]; //0x0414
	int32_t mobIsAlive; //0x0458
	char pad_045C[4]; //0x045C
	int32_t isOwnPlayer; //0x0460 1 if own 0 else
	char pad_0464[5400]; //0x0464
	int32_t uid; //0x197C

	Vector3 getPosition() { CHECK_BAD_VEC3(Entity); return position; };
	int32_t getIsPlayerCharacter() { CHECK_BAD_NUM(Entity); return isPlayerCharacter; };
	int32_t getIsRendered() { CHECK_BAD_NUM(Entity); return isRendered; };
	void setIsRendered(int32_t value) { CHECK_BAD(Entity); isRendered = value; };
	int32_t getMobIsAlive() { CHECK_BAD_NUM(Entity); return mobIsAlive; };
	int32_t getUid() { CHECK_BAD_NUM(Entity); return uid; };
	int32_t getIsOwnPlayer() { CHECK_BAD_NUM(Entity); return isOwnPlayer; };
	int32_t getMobId() { CHECK_BAD_NUM(Entity); return mobId; };
}; 


//ATTACK MOB WITH ID
//base offset
// TODO: get this address via function hook
const uintptr_t offsetToPlayerControlObject = 0x1CF8528;
//int
const uintptr_t offsetToAttackUID = 0x50;

//WALLHACK
//base offset
// TODO: get this address via function hooks
const uintptr_t offsetWallHackBase = 0x1CF853C;
//byte
const uintptr_t offsetWallHackOne = 0xC;
const uintptr_t offsetWallHackTwo = 0x1C5C;

//ids
const int metinIdStart = 8000;
const int metinIdEnd = 8112;
const int playerIdentifier = 0x3F800000;

const int distanceToPreferClosestStone = 1000;

//globals
extern uintptr_t baseAdressMainMod;
extern Entity* entities[255];
extern Entity* entityPointer;
extern uintptr_t originalStartEntiyEditFunction;
extern bool shutdown;
extern bool botRunning;
extern bool freezeWhenPlayersPresent;
extern bool firstLoop;
extern uintptr_t editEntityFunctionAddress;

inline const char* pickupFunctionPattern = "\x55\x8B\xEC\x6A\x00\x68\x00\x00\x00\x00\x64\x00\x00\x00\x00\x00\x00\x00\x00\x00\x53\x56\x57\xA1\x00\x00\x00\x00\x00\x00\x00\x00\x45\x00\x64\x00\x00\x00\x00\x00\x00\x00\x00\x00\xF0\x8D\x00\x00\x8B\x01";
inline const char* pickupFunctionMask = "xxxx?x????x?????????xxxx????????x?x?????????xx??xx";

// address of function that moves classpointer, skip 2 bytes to get to the pointer
inline const char* pickupFunctionClassPointerFunctionPattern = "\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x55\x8B\xEC\x51\x8D\x45\x00\x50\x6A\x00\xFF\x75\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x84\xC0\x75\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\xFF\x75\x00\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x51\x8D\x45\x00\x50\x6A\x00\xFF\x75\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x84\xC0\x75\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\x83\x7D\xFC";
inline const char* pickupFunctionClassPointerFunctionMask = "xx????x????x????xxxxxx?xx?xx?x????xx?xxx?x????xxxxxx?xx????x????x????xxxxxxxxxxxxxxxxx?xx?xx?x????xx?xxx?x????xxxxxxx";
inline const BYTE pickupFunctionOffsetToClassPointer = 0x2;

// F3 0F ? ? ? ? ? ? FF B6
// should be unique in the main module
inline const char* editEntityFunctionPattern = "\xF3\x0F\x00\x00\x00\x00\x00\x00\xFF\xB6";
inline const char* editEntityFunctionMask = "xx??????xx";