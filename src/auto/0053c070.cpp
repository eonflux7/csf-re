#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(const char*, __cdecl, GetTrucoString, 0x428B10, ());
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x695590, (const char* src, uint16_t* dst, int maxCount));
CSF_FUNC(int, __cdecl, CompareWideStrings, 0x6957B0, (uint16_t* a, uint16_t* b));
CSF_FUNC(int, __cdecl, GetOwnerGlobal, 0x506BD0, ());
CSF_FUNC(int, __cdecl, GetPlayerStateGlobal, 0x756A10, ());
CSF_FUNC(int, __cdecl, GetActiveSessionGlobal, 0x75FDF0, ());

// __thiscall method: self = ecx, edx unused. Returns eax as the original does:
// full eax from GetActiveSessionGlobal on the store path, or the compare result's
// upper bytes merged with the flag byte on the early-exit path.
static int __fastcall MenuProfiles_UpdateTrucoFlag(uint8_t* self, void* /*edx*/)
{
    void* globalObj = reinterpret_cast<void*>(static_cast<uintptr_t>(GetOwnerGlobal()));
    void* ownerObj = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, globalObj, 8)));

    uint16_t trucoWide[128];
    const char* trucoText = GetTrucoString();
    CopyTrucoStringToWide(trucoText, trucoWide, 128);

    void** ownerVtbl = CSF_AT(void**, ownerObj, 0);
    int profileEntryValue = reinterpret_cast<int (__fastcall*)(void*, void*)>(ownerVtbl[9])(ownerObj, nullptr);

    void* profileEntry = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(profileEntryValue)));
    uint32_t entryTextAddr;
    if (CSF_AT(uint32_t, profileEntry, 0x18) < 8u)
        entryTextAddr = static_cast<uint32_t>(profileEntryValue) + 4u;
    else
        entryTextAddr = CSF_AT(uint32_t, profileEntry, 4);

    int compareResult = CompareWideStrings(
        reinterpret_cast<uint16_t*>(static_cast<uintptr_t>(entryTextAddr)), trucoWide);

    uint8_t flag;
    if (compareResult == 0) {
        flag = 1;
    } else {
        uint8_t currentFlag = CSF_AT(uint8_t, self, 0x40);
        flag = 0;
        if (currentFlag == 0) {
            // eax keeps the compare result's upper bytes; only al is replaced.
            return static_cast<int>((static_cast<uint32_t>(compareResult) & 0xFFFFFF00u) | currentFlag);
        }
    }

    CSF_AT(uint8_t, self, 0x40) = flag;

    void* playerState = reinterpret_cast<void*>(static_cast<uintptr_t>(GetPlayerStateGlobal()));
    CSF_AT(uint8_t, playerState, 0x50) = flag;

    void* activeSession = reinterpret_cast<void*>(static_cast<uintptr_t>(GetActiveSessionGlobal()));
    CSF_AT(uint8_t, activeSession, 0xD2) = flag;

    return static_cast<int>(reinterpret_cast<uintptr_t>(activeSession));
}

HOOK(0x0053C070, MenuProfiles_UpdateTrucoFlag, "auto");
