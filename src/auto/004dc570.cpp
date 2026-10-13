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

// Copies a 0x1C-byte block of two 3-dword groups: dwords 0, 2, 3 and 4, 5, 6 of the
// source. Dword 1 (offset 4) is not copied. Returns the destination in eax.
static uint32_t* __fastcall Vec3Pair_CopyFrom(uint32_t* self, void* /*edx*/, const uint32_t* src)
{
    self[0] = src[0];
    self[2] = src[2];
    self[3] = src[3];
    self[4] = src[4];
    self[5] = src[5];
    self[6] = src[6];
    return self;
}

HOOK(0x004DC570, Vec3Pair_CopyFrom, "auto");
