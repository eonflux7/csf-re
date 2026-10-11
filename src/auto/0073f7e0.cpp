#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SpawnPoint_LaunchSpawnRequest, 0x00747830, (void* self, void* edx, char a2, char a3, char a4, char a5, char a6, char a7));

static char __fastcall CMenuMpStats_LatchStatsSnapshot(CMenuMpStats* self, void* /*edx*/, int a2, int a3, int a4, int a5, int a6, int a7)
{
    uint8_t latchedFlag = CSF_AT(uint8_t, self, 0x99);
    if (latchedFlag != 0)
    {
        return static_cast<char>(latchedFlag);
    }

    if (Sub_64B250(self, nullptr) != 0)
    {
        SpawnPoint_LaunchSpawnRequest(self, nullptr,
            static_cast<char>(a2), static_cast<char>(a3), static_cast<char>(a4),
            static_cast<char>(a5), static_cast<char>(a6), static_cast<char>(a7));
    }

    CSF_AT(uint8_t, self, 0x98) = 1;
    CSF_AT(uint8_t, self, 0x99) = 1;
    CSF_AT(int, self, 0xA0) = a2;
    CSF_AT(int, self, 0xA4) = a3;
    CSF_AT(int, self, 0xA8) = a4;
    CSF_AT(int, self, 0xAC) = a5;
    CSF_AT(int, self, 0xB0) = a6;
    CSF_AT(int, self, 0xB4) = a7;

    // Original returns the low byte of a5 (last value loaded into eax/al).
    return static_cast<char>(a5);
}

HOOK(0x0073F7E0, CMenuMpStats_LatchStatsSnapshot, "auto");
