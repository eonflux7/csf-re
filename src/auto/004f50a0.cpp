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

// Adds deltaX/deltaY to the two float accumulators at this+0xB8 / this+0xBC
// (pseudocode indices 46 and 47 of the float array).
static void __fastcall CharacterModel_AccumulateOffset(float* self, void* /*edx*/, float deltaX, float deltaY)
{
    CSF_AT(float, self, 0xB8) = deltaX + CSF_AT(float, self, 0xB8);
    CSF_AT(float, self, 0xBC) = deltaY + CSF_AT(float, self, 0xBC);
}

HOOK(0x004F50A0, CharacterModel_AccumulateOffset, "auto");
