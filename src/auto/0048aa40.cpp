#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static void __fastcall UpdateTimeController(float* self, void* /*edx*/, float deltaTime)
{
    CSF_AT(uint8_t, self, 0x4) = 0;
    CSF_AT(float, self, 0x0) = deltaTime + CSF_AT(float, self, 0x0);
}

HOOK_KEEP_ECX(0x0048AA40, UpdateTimeController, 1, "auto");
