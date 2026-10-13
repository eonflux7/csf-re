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

// Initializes a 0x16-byte world record: two dwords from the arguments, a pair of
// dwords copied from the object a5 points to, then a flag byte and a cleared byte.
static void* __fastcall GfxWorld_InitRecord(void* self, void* /*edx*/,
                                           uint32_t firstValue, uint32_t secondValue,
                                           uint32_t thirdValue, uint32_t* sourcePair,
                                           char flagValue)
{
    CSF_AT(uint32_t, self, 0x00) = firstValue;
    CSF_AT(uint32_t, self, 0x08) = thirdValue;
    CSF_AT(uint32_t, self, 0x04) = secondValue;
    CSF_AT(uint32_t, self, 0x0C) = sourcePair[0];
    CSF_AT(uint32_t, self, 0x10) = sourcePair[1];
    CSF_AT(uint8_t, self, 0x14) = static_cast<uint8_t>(flagValue);
    CSF_AT(uint8_t, self, 0x15) = 0;
    return self;
}

HOOK(0x6B6CB0, GfxWorld_InitRecord, "auto");
