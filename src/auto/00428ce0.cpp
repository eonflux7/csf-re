#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(uint32_t*, __fastcall, sub_428680, 0x00428680, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void, __fastcall, sub_428B90, 0x00428B90, (void* self, void* edx, float* a2));

// Resets the 93 cheat slots (20-byte records starting at +0x10: a dword at +0x0C = 0,
// a float at +0x10 = 1.0f), then reads the 16-byte flag table returned by the
// global state's sub-object and applies each set flag.
static void __fastcall C_00830030_ResetCheatSlots(C_00830030* self, void* /*edx*/)
{
    for (int slot = 0; slot < 93; ++slot)
    {
        std::size_t recordOffset = 0x10 + static_cast<std::size_t>(slot) * 0x14;
        CSF_AT(uint32_t, self, recordOffset - 4) = 0;
        CSF_AT(float, self, recordOffset) = 1.0f;
    }

    uint8_t* globalState = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
    void* stateChild = CSF_AT(void*, globalState, 0x10);
    char* flagTable = reinterpret_cast<char*>(
        ((char* (__fastcall*)(void*, void*))CSF_AT(void**, stateChild, 0)[0x38 / 4])(stateChild, nullptr));

    if (flagTable)
    {
        for (uint32_t i = 0; i < 0x10; ++i)
        {
            if (flagTable[i] != 0)
                sub_428680(self, nullptr, 3, static_cast<int>(i));
        }
        sub_428B90(self, nullptr, reinterpret_cast<float*>(flagTable));
    }
}

HOOK(0x428CE0, C_00830030_ResetCheatSlots, "auto");
