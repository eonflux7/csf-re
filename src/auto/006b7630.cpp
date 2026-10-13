#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// RpWorldForAllWorldSectors(RpWorld*, callback, userData), cdecl, pops nothing.
CSF_FUNC(int, __cdecl, RpWorldForAllWorldSectors, 0x0058CFF0,
         (int a1, int (__cdecl* a2)(int*, int), int a3));

// Per-sector callback at 0x006B7240 (passed as a value, not called here).
// Its body is not part of this file.

// Iterates every sector of the world stored at this->field_0010 with the
// sector callback, passing a two-dword context {this, 0}. Afterwards marks
// the object's flag at +0x24 as set and returns the iteration result.
static int __fastcall GfxWorld_IterateSectorsAndMarkFlag(S_006b6cf0* self, void* /*edx*/)
{
    // Context block lives in the frame: [ebp-8] = self, [ebp-4] = 0.
    uint32_t sectorContext[2];
    sectorContext[0] = reinterpret_cast<uint32_t>(self);
    sectorContext[1] = 0;

    auto sectorCallback = reinterpret_cast<int (__cdecl*)(int*, int)>(CSF_ADDR_FN(0x006B7240));

    int result = RpWorldForAllWorldSectors(static_cast<int>(self->field_0010),
                                           sectorCallback,
                                           reinterpret_cast<int>(sectorContext));
    self->field_0024 = 1;
    return result;
}

HOOK(0x6B7630, GfxWorld_IterateSectorsAndMarkFlag, "auto");
