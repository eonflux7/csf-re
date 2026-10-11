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

// Enumeration helper: runs the DirectSound enum dispatcher with a callback.
// Original 0x7DE550 is __cdecl and its return value is used by the caller.
CSF_FUNC(int, __cdecl, DirectSoundEnum_RunWithCallback, 0x7DE550,
         (int (__cdecl* callback)(int*, int), int userData));

// Callback address (SOUNDMAX DirectSound enumeration callback).
static int (__cdecl* GetEnumCallback())(int*, int) {
    return reinterpret_cast<int (__cdecl*)(int*, int)>(CSF_ADDR_FN(0x7109B0));
}

// Returns true if the object's "is set" virtual query (vtbl slot 0x58 / 4 = 22) reports set.
// If set, runs the DirectSound enum dispatcher with the callback and returns its result;
// otherwise returns 0.
static int __fastcall C_00863ba8_RunCallbackIfFlagSet(C_00863ba8* self, void* /*edx*/, int a2)
{
    using IsSetFn = bool(__fastcall*)(void* self, void* edx);
    IsSetFn isSet = reinterpret_cast<IsSetFn>(self->vtbl[0x58 / 4]);
    bool flagSet = isSet(self, nullptr);

    if (flagSet) {
        return DirectSoundEnum_RunWithCallback(GetEnumCallback(), a2);
    }
    return 0;
}

HOOK(0x710F90, C_00863ba8_RunCallbackIfFlagSet, "auto");
