#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Adds (flag clear) or subtracts (flag set) the float at offset +4 of the
// argument object from the float at offset +0x2C of self. The flag is the
// byte at +0x51 of the object returned by GetGlobalValue_8BC1E0.
static int __fastcall Cheats_AdjustOffsetByFlag(float* self, void* /*edx*/, int a2) {
    int result = GetGlobalValue_8BC1E0();
    void* globalObj = reinterpret_cast<void*>(static_cast<uintptr_t>(result));

    float newOffset;
    if (CSF_AT(uint8_t, globalObj, 0x51) != 0) {
        result = a2;
        newOffset = CSF_AT(float, self, 0x2C) - CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), 4);
    } else {
        newOffset = CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), 4) + CSF_AT(float, self, 0x2C);
    }
    CSF_AT(float, self, 0x2C) = newOffset;
    return result;
}

HOOK(0x0042B110, Cheats_AdjustOffsetByFlag, "auto");
