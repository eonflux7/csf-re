#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Adds a float delta to the float stored at +0x08 of the controller object (class not yet identified).
static void __fastcall AuxiliaryController_AccumulateValue(float* self, void* /*edx*/, float delta)
{
    CSF_AT(float, self, 0x8) = delta + CSF_AT(float, self, 0x8);
}

HOOK(0x443760, AuxiliaryController_AccumulateValue, "auto");
