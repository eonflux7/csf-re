#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Adds the float pointed to by a2 into the float field at +0x28 of self,
// then returns a2 unchanged.
static float* __fastcall Cheats_AddArgToFieldAt28(void* self, void* /*edx*/, float* a2) {
    CSF_AT(float, self, 0x28) = *a2 + CSF_AT(float, self, 0x28);
    return a2;
}

HOOK(0x42B0F0, Cheats_AddArgToFieldAt28, "auto");
