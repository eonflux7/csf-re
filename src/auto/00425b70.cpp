#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall: ecx = this, edx unused. Original `ret 0`, so no stack bytes are popped.
static int __fastcall IsZeroFlagAndCounterAtFinalStep(void* self, void* /*edx*/)
{
    // Original: if (this[3] != 0) return 0;
    if (CSF_AT(int, self, 0x0C) != 0)
        return 0;

    // Original: return this[9] == this[7] - 2;
    const uint32_t counterAtStep = static_cast<uint32_t>(CSF_AT(int, self, 0x24));
    const uint32_t limitMinusTwo = static_cast<uint32_t>(CSF_AT(int, self, 0x1C)) - 2u;
    return counterAtStep == limitMinusTwo ? 1 : 0;
}

HOOK(0x425B70, IsZeroFlagAndCounterAtFinalStep, "auto");
