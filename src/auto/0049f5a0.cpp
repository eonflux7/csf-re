#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: sub_49F330 (__thiscall, one stack arg; ret 4). Grows the entry array at this+0x198.
CSF_FUNC(void*, __fastcall, sub_49F330, 0x0049F330, (void* self, void* edx, int a2));

// Appends a 28-byte entry to the array at this+0x198 and returns a pointer to it.
// Array header: +0x198 data pointer, +0x19C capacity, +0x1A0 count, +0x1A4 growth.
// Entry layout: +0x0 unused, +0x4 = -1, +0x8 = -1, +0xC..+0x18 = the four arguments.
static uint32_t* __fastcall C_00833908_ConfigureMissionInterface(C_00833908* self, void* /*edx*/,
                                                                 int a2, int a3, int a4, int a5) {
    void* arrayHeader = reinterpret_cast<uint8_t*>(self) + 0x198;

    if (CSF_AT(uint32_t, self, 0x1A0) >= CSF_AT(uint32_t, self, 0x19C)) {
        sub_49F330(arrayHeader, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x1A4)));
    }

    uint32_t entryIndex = CSF_AT(uint32_t, self, 0x1A0);
    uint8_t* entryBase = CSF_AT(uint8_t*, self, 0x198) + static_cast<size_t>(entryIndex) * 0x1C;
    CSF_AT(uint32_t, self, 0x1A0) = entryIndex + 1;

    uint32_t* result = reinterpret_cast<uint32_t*>(entryBase);
    result[1] = 0xFFFFFFFFu;
    result[2] = 0xFFFFFFFFu;
    result[3] = static_cast<uint32_t>(a2);
    result[4] = static_cast<uint32_t>(a3);
    result[5] = static_cast<uint32_t>(a4);
    result[6] = static_cast<uint32_t>(a5);
    return result;
}

HOOK(0x0049F5A0, C_00833908_ConfigureMissionInterface, "auto");
