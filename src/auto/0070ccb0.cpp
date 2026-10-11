#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Adds a delta to the float stored at +0xEC of the Water object (class not yet identified).
static void __fastcall WaterParams_AddToFloatField0EC(float* self, void* /*edx*/, float delta)
{
    CSF_AT(float, self, 0xEC) = delta + CSF_AT(float, self, 0xEC);
}

HOOK(0x70CCB0, WaterParams_AddToFloatField0EC, "auto");
