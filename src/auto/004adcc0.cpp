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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(const char*, __cdecl, GetFutureErrorWhatString, 0x004E5750, (int index));
CSF_FUNC(char*, __cdecl, SceneLightsObjects_GetDistraccionString, 0x004F0DF0, (int index));

// Returns the address of the handle slot to use: slot at +0x628 unless
// the flag at +0x630 is negative, in which case the slot at +0x610 is used.
static uint8_t* SelectHandleSlotAddress(int* self)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    if (CSF_AT(int, self, 0x630) < 0)
        return base + 0x610;
    return base + 0x628;
}

static const char* __fastcall GetObjectDescriptionString(int* self, void* /*edx*/)
{
    // If the selected handle slot is valid, resolve it and read its target name.
    uint8_t* slotAddress = SelectHandleSlotAddress(self);
    if (*reinterpret_cast<int*>(slotAddress + 8) >= 0)
    {
        int handleAddress = static_cast<int>(reinterpret_cast<uintptr_t>(SelectHandleSlotAddress(self)));
        C_00833908* resolved = reinterpret_cast<C_00833908*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress));
        if (resolved)
            C_00833908_GetTargetName(resolved, nullptr);
    }

    int errorIndex = CSF_AT(int, self, 0x608);
    if (errorIndex != -1)
        return GetFutureErrorWhatString(errorIndex);

    int lightsIndex = CSF_AT(int, self, 0x60C);
    if (lightsIndex != -1)
        return SceneLightsObjects_GetDistraccionString(lightsIndex);

    return reinterpret_cast<const char*>(static_cast<uintptr_t>(static_cast<uint32_t>(errorIndex)));
}

HOOK(0x004ADCC0, GetObjectDescriptionString, "auto");
