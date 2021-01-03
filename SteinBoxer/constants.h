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
	int32_t mobIsDead; //0x0458 1 if died recently, 0 otherwise
	char pad_045C[4]; //0x045C
	int32_t isOwnPlayer; //0x0460 1 if own 0 else
	char pad_0464[5400]; //0x0464
	int32_t uid; //0x197C

	Vector3 getPosition() { CHECK_BAD_VEC3(Entity); return position; };
	int32_t getIsPlayerCharacter() { CHECK_BAD_NUM(Entity); return isPlayerCharacter; };
	int32_t getIsRendered() { CHECK_BAD_NUM(Entity); return isRendered; };
	void setIsRendered(int32_t value) { CHECK_BAD(Entity); isRendered = value; };
	int32_t getMobIsDead() { CHECK_BAD_NUM(Entity); return mobIsDead; };
	int32_t getUid() { CHECK_BAD_NUM(Entity); return uid; };
	int32_t getIsOwnPlayer() { CHECK_BAD_NUM(Entity); return isOwnPlayer; };
	int32_t getMobId() { CHECK_BAD_NUM(Entity); return mobId; };
}; 


//ids
const int metinIdStart = 8000;
const int metinIdEnd = 8112;
const int playerIdentifier = 0x3F800000;

const int distanceToPreferClosestStone = 2000;

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
inline const char* cPythonPlayerSingletonPointerFunctionPattern = "\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x55\x8B\xEC\x51\x8D\x45\x00\x50\x6A\x00\xFF\x75\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x84\xC0\x75\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\xFF\x75\x00\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x51\x8D\x45\x00\x50\x6A\x00\xFF\x75\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x84\xC0\x75\x00\xE8\x00\x00\x00\x00\x8B\xE5\x5D\xC3\x83\x7D\xFC";
inline const char* cPythonPlayerSingletonPointerFunctionMask = "xx????x????x????xxxxxx?xx?xx?x????xx?xxx?x????xxxxxx?xx????x????x????xxxxxxxxxxxxxxxxx?xx?xx?x????xx?xxx?x????xxxxxxx";
inline const BYTE cPythonPlayerSingletonPointerOffsetToClassPointer = 0x2;

// F3 0F ? ? ? ? ? ? FF B6
// should be unique in the main module
inline const char* editEntityFunctionPattern = "\xF3\x0F\x00\x00\x00\x00\x00\x00\xFF\xB6";
inline const char* editEntityFunctionMask = "xx??????xx";

inline const char* whFirstValueForAdditionPattern = "\x56\x8B\xF1\xE8\x00\x00\x00\x00\x84\xC0\x74\x00\x68";
inline const char* whFirstValueForAdditionMask = "xxxx????xxx?x";
inline const uintptr_t whFirstValueOffsetToValue = 0x1D;

inline const char* whSecondValueForAdditionPattern = "\xC6\x81\x84\x19\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x8B\x45";
inline const char* whSecondValueForAdditionMask = "xxxxxx?xxxxxxxxxxxxxx";
inline const uintptr_t whSecondValueOffsetToValue = 0x2;

inline const char* whBaseAddressPattern = "\x8B\x0D\x00\x00\x00\x00\xE9\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\x8B\x0D\x00\x00\x00\x00\xE9\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\x8B\xD1";
inline const char* whBaseAddressMask = "xx????x????xxxxxxx????x????xxxxxxx";
inline const uintptr_t whBaseAddressOffsetToValue = 0x2;

inline const char* attackEntityPattern = "\x55\x8B\xEC\x8B\x45\x00\x89\x41\x00\x5D\xC2\x00\x00\xCC\xCC\xCC\x55\x8B\xEC\x51\x53\x56\x57\x8B\xF9\x8B\x47\x00\x8D\x4F";
inline const char* attackEntityMask = "xxxxx?xx?xx??xxxxxxxxxxxxxx?xx";