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
	char pad_0000[16]; //0x0000
	char name[24]; //0x0010
	char pad_0028[692]; //0x0028
	Vector3 position; //0x02DC
	int32_t isPlayerCharacter; //0x02E8 // if player 3F800000
	char pad_02EC[84]; //0x02EC
	int32_t isRendered; //0x0340
	char pad_0344[924]; //0x0344
	int32_t attackStance; //0x06E0
	char pad_06E4[4]; //0x06E4
	int32_t mobID; //0x06E8
	char pad_06EC[68]; //0x06EC
	int32_t mobDiedRecently; //0x0730 1 if died recently, 0 otherwise
	char pad_0734[5408]; //0x0734
	int32_t uID; //0x1C54
	char pad_1C58[4]; //0x1C58
	int8_t skipCollision; //0x1C5C
	char pad_1C5D[1019]; //0x1C5D

	void disableSkipCollision() { CHECK_BAD(Entity); skipCollision = 0x0; };
	void enableSkipCollision() { CHECK_BAD(Entity); skipCollision = 0x1; };
	int32_t getAttackStance() { CHECK_BAD_NUM(Entity); return attackStance; };
	Vector3 getPosition() { CHECK_BAD_VEC3(Entity); return position; };
	int8_t getSkipCollision() { CHECK_BAD_NUM(Entity); return skipCollision; };
	int32_t getIsPlayerCharacter() { CHECK_BAD_NUM(Entity); return isPlayerCharacter; };
	int32_t getIsRendered() { CHECK_BAD_NUM(Entity); return isRendered; };
	void setIsRendered(int32_t value) { CHECK_BAD(Entity); isRendered = value; };
	int32_t getMobDiedRecently() { CHECK_BAD_NUM(Entity); return mobDiedRecently; };
	int32_t getUid() { CHECK_BAD_NUM(Entity); return uID; };
	int32_t getMobId() { CHECK_BAD_NUM(Entity); return mobID; };

}; //Size: 0x2058

using TCharacterInstanceMap = std::map<DWORD, Entity*>;

//ids
const int metinIdStart = 8001;
const int metinIdEnd = 8416;
const int oreIdStart = 30301;
const int oreIdEnd = 30306;

const int playerIdentifier = 0x3F800000;

const int distanceToPreferClosestStone = 2000;

//globals
extern uintptr_t baseAdressMainMod;
extern bool shutdown;
extern bool botRunning;
extern bool freezeWhenPlayersPresent;
extern bool firstLoop;

extern bool gotWhispered;

extern uintptr_t originalStartRecvWhisperPacketFunction;
extern uintptr_t recvWhisperPacketFunctionAddress;

extern std::vector<int> blacklistedUids;

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

inline const char* getMainPtrFunctionPattern = "\x55\x8b\xec\x51\x89\x4d\x00\x8b\x45\x00\x8b\x40\x00\x8b\xe5\x5d\xc3\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\x55\x8b\xec\x83\xec\x00\x89\x4d\x00\x83\x7d";
inline const char* getMainPtrFunctionMask = "xxxxxx?xx?xx?xxxxxxxxxxxxxxxxxxxxxxxx?xx?xx";

inline const char* getMainInstancePtrClassPointerPattern = "\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x10\x56\x57";
inline const char* getMainInstancePtrClassPointerMask = "xxxxxxxxxxx";
inline const BYTE getMainInstancePtrClassPointerOffset = 0xF;

inline const char* getMainInstancePtrPattern = "\x8B\x41\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x81\xEC";
inline const char* getMainInstancePtrMask = "xx?xxxxxxxxxxxxxxxxxx";

inline const char* recvWhisperPacketFunctionPattern = "\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xF0\x53\x56\x57\x50\x8D\x45\xF4\x64\xA3\x00\x00\x00\x00\x8B\xF9\x8D\x85\x00\x00\x00\x00";
inline const char* recvWhisperPacketFunctionMask = "xxxxxx????xx????xxx????x????xxxxxxxxxxxxxx????xxxx????";

// to get CPythonCharacterManager pointer
inline const char* recvDeadPacketFunctionPattern = "\x55\x8B\xEC\x83\xEC\x10\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xFC\x53\x8D\x45\xF0";
inline const char* recvDeadPacketFunctionMask = "xxxxxxx????xxxxxxxxx";
inline const BYTE offsetTocPythonCharacterManagerPointer = 0x44;
inline const BYTE offsetToTCharacterInstanceMap = 0x20;