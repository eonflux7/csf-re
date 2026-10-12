#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall AiDistracted_CheckReactionDelay(float* self, void* /*edx*/, float delay)
{
    return CSF_AT(float, self, 0x58) >= delay;
}

HOOK(0x00448170, AiDistracted_CheckReactionDelay, "auto");
