#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Adds a time delta to the float stored at +0xD4 of the GfxScene object (class not yet identified).
static void __fastcall GfxScene_AdvanceElapsedTime(float* self, void* /*edx*/, float elapsedDelta)
{
    CSF_AT(float, self, 0xD4) = elapsedDelta + CSF_AT(float, self, 0xD4);
}

HOOK(0x6D9D00, GfxScene_AdvanceElapsedTime, "auto");
