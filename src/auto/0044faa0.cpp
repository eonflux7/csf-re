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

CSF_GLOBAL(uint32_t, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(uint32_t, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(uint32_t, g_ZeroVectorZ, 0x008BB428);

// Resets a nearby-candidate result record: zeroes the vectors and flags,
// sets the two float slots to a large sentinel (0x7F7FFFFF = FLT_MAX),
// then copies the zero vector into the two vector slots.
static uint32_t* __fastcall InitializeNearbyCandidateResult(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;

    CSF_AT(uint32_t, self, 0x08) = 2139095039u;
    CSF_AT(uint32_t, self, 0x0C) = 2139095039u;

    CSF_AT(uint32_t, self, 0x20) = g_ZeroVectorX;
    CSF_AT(uint32_t, self, 0x24) = g_ZeroVectorY;
    CSF_AT(uint32_t, self, 0x28) = g_ZeroVectorZ;

    CSF_AT(uint32_t, self, 0x14) = g_ZeroVectorX;
    CSF_AT(uint32_t, self, 0x18) = g_ZeroVectorY;
    CSF_AT(uint32_t, self, 0x1C) = g_ZeroVectorZ;

    return static_cast<uint32_t*>(self);
}

HOOK(0x0044FAA0, InitializeNearbyCandidateResult, "auto");
