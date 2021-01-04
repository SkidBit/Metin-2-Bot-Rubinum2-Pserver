#pragma once
#include "stdafx.h"

#define CHECK_BAD_PTR(x) if(IsBadReadPtr(this,sizeof(x))) return nullptr
#define CHECK_BAD(x) if(IsBadReadPtr(this, sizeof(x))) return
#define CHECK_BAD_NUM(x) if(IsBadReadPtr(this, sizeof(x))) return 0
#define CHECK_BAD_VEC3(x) if(IsBadReadPtr(this, sizeof(x))) return Vector3{0,0,0}

struct Vector3 { float x, y, z; };

class Player
{
public:
	char pad_0000[732]; //0x0000
	Vector3 position; //0x02DC
	char pad_02E8[1016]; //0x02E8
	int32_t attackStance; //0x06E0
	char pad_06E4[5496]; //0x06E4
	int8_t skipCollision; //0x1C5C

	void disableSkipCollision() { CHECK_BAD(Player); skipCollision = 0x0; };
	void enableSkipCollision() { CHECK_BAD(Player); skipCollision = 0x1; };
	int32_t getAttackStance() { CHECK_BAD_NUM(Player); return attackStance; };
	Vector3 getPosition() { CHECK_BAD_VEC3(Player); return position; };
	int8_t getSkipCollision() { CHECK_BAD_NUM(Player); return skipCollision; };
}; //Size: 0x2848

class Entity
{
public:
	char pad_0000[4]; //0x0000
	Vector3 position; //0x0004
	int32_t isPlayerCharacter; //0x0010 // if player 3F800000
	char pad_0014[84]; //0x0014
	int32_t isRendered; //0x0068
	char pad_006C[924]; //0x006C
	int32_t attackStance; //0x0408 != 0 if attacking
	char pad_040C[4]; //0x040C
	int32_t mobId; //0x0410
	char pad_0414[68]; //0x0414
	int32_t mobIsDead; //0x0458 1 if died recently, 0 otherwise
	char pad_045C[4]; //0x045C
	int32_t isOwnPlayer; //0x0460 1 if own, 0 else
	char pad_0464[5400]; //0x0464
	int32_t uid; //0x197C
	char pad_1980[4]; //0x1980
	int8_t skipCollision; //0x1984 1 if active, 0 else
	char pad_1985[707]; //0x1985

	int32_t getAttackStance() { CHECK_BAD_NUM(Entity); return attackStance; };
	void disableSkipCollision() { CHECK_BAD(Entity); skipCollision = 0x0; };
	void enableSkipCollision() { CHECK_BAD(Entity); skipCollision = 0x1; };
	int8_t getSkipCollision() { CHECK_BAD_NUM(Entity); return skipCollision; };
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
const int metinIdStart = 8001;
const int metinIdEnd = 8416;
const int oreIdStart = 30301;
const int oreIdEnd = 30306;

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

inline const char* attackEntityPattern = "\x55\x8B\xEC\x8B\x45\x00\x89\x41\x00\x5D\xC2\x00\x00\xCC\xCC\xCC\x55\x8B\xEC\x51\x53\x56\x57\x8B\xF9\x8B\x47\x00\x8D\x4F";
inline const char* attackEntityMask = "xxxxx?xx?xx??xxxxxxxxxxxxxx?xx";

inline const char* chrMgrInstancePointerPattern = "\x55\x8b\xec\xa1\x00\x00\x00\x00\x5d\xc3\xcc\xcc\xcc\xcc\xcc\xcc\x55\x8b\xec\xa1\x00\x00\x00\x00\x5d\xc3\xcc\xcc\xcc\xcc\xcc\xcc\x55\x8b\xec\xa1\x00\x00\x00\x00\x5d\xc3\xcc\xcc\xcc\xcc\xcc\xcc\x55\x8b\xec\xa1\x00\x00\x00\x00\x5d\xc3\xcc\xcc\xcc\xcc\xcc\xcc\x55\x8b\xec\xa1\x00\x00\x00\x00\x5d\xc3\xcc\xcc\xcc\xcc\xcc\xcc\x55\x8b\xec\xa1\x00\x00\x00\x00\x5d\xc3\xcc\xcc\xcc\xcc\xcc\xcc\x55\x8b\xec\x83\xec";
inline const char* chrMgrInstanePointerMask = "xxxx????xxxxxxxxxxxx????xxxxxxxxxxxx????xxxxxxxxxxxx????xxxxxxxxxxxx????xxxxxxxxxxxx????xxxxxxxxxxxxx";

inline const char* getMainPtrFunctionPattern = "\x55\x8b\xec\x51\x89\x4d\x00\x8b\x45\x00\x8b\x40\x00\x8b\xe5\x5d\xc3\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\x55\x8b\xec\x83\xec\x00\x89\x4d\x00\x83\x7d";
inline const char* getMainPtrFunctionMask = "xxxxxx?xx?xx?xxxxxxxxxxxxxxxxxxxxxxxx?xx?xx";

inline const char* getMainInstancePtrClassPointerPattern = "\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x10\x56\x57";
inline const char* getMainInstancePtrClassPointerMask = "xxxxxxxxxxx";
inline const BYTE getMainInstancePtrClassPointerOffset = 0xF;

inline const char* getMainInstancePtrPattern = "\x8B\x41\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x81\xEC";
inline const char* getMainInstancePtrMask = "xx?xxxxxxxxxxxxxxxxxx";