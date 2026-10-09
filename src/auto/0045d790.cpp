#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Bindings for the original callees (addresses from the packet).
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (void* self, void* /*edx*/, int value));
// Original pushes three arguments and never cleans them up; the caller's ret 12 consumes them later.
CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, (int a1, int a2, int a3));
// ecx = resolved value; three stack arguments (a4 int, a5 float, a6 int) popped by the callee (ret 12).
CSF_FUNC(int, __fastcall, sub_4F4980, 0x004F4980, (void* self, void* /*edx*/, int a4, float a5, int a6));

static float MissionMenu_BitsToFloat(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

// Resolves a menu entry from the nested argument block and returns the result of the final call.
static int __stdcall MissionMenu_SelectEntryFromArgs(int a1, int a2, uint32_t* a3)
{
    int menuObject = Get_008b0678();
    MenuMissionInfo_SetGlobalIndex(reinterpret_cast<void*>(static_cast<intptr_t>(menuObject)), nullptr, 92);

    // Walk *a3 -> +4 -> object whose field at +0x1C selects the inline or indirect slot.
    uint32_t firstObject = a3[0];
    uint32_t entryHolder = CSF_AT(uint32_t, firstObject, 4);
    uint32_t entryCount = CSF_AT(uint32_t, entryHolder, 0x1C);
    uint32_t entryAddress;
    if (entryCount < 0x10)
        entryAddress = entryHolder + 4 + 4;
    else
        entryAddress = CSF_AT(uint32_t, entryHolder, 4 + 4);

    // a3[1] -> +4 -> +4
    uint32_t secondObject = a3[1];
    uint32_t secondHolder = CSF_AT(uint32_t, secondObject, 4);
    uint32_t secondValue = CSF_AT(uint32_t, secondHolder, 4);

    int resolved = GetGlobal8B0558(static_cast<int>(entryAddress), static_cast<int>(secondValue), -1);
    return sub_4F4980(reinterpret_cast<void*>(static_cast<intptr_t>(resolved)), nullptr,
                      static_cast<int>(entryAddress),
                      MissionMenu_BitsToFloat(secondValue),
                      -1);
}

HOOK(0x0045D790, MissionMenu_SelectEntryFromArgs, "auto");
