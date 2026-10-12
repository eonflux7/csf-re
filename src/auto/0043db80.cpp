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

// Base-section cleanup run on the whole object (same callee as sub_43AD60).
CSF_FUNC(int, __fastcall, C_00830d90_Cleanup, 0x0043AD60, (void* self, void* edx));
// Frees the pointer storage embedded at +0x4C (S_0047ea40).
CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790, (int* self, void* edx));
// Game heap free.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body for C_00830e5c. Takes one stack argument (flags byte; bit 0 = free storage).
static int* __fastcall C_00830e5c_Destroy(C_00830e5c* self, void* /*edx*/, char flags)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00830E5C);

    DestroyPointerStorage(reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x4C), nullptr);
    C_00830d90_Cleanup(self, nullptr);

    if ((flags & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<int*>(self);
}

HOOK(0x0043DB80, C_00830e5c_Destroy, "auto");
