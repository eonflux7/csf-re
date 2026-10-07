#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Increments a small counter stored at +0x7C (unsigned compare < 0xF).
// Returns the incremented value; if the counter already reached 0xF it is
// left untouched and its current value is returned.
static uint32_t __fastcall IncrementSaturatingCounter(void* self, void* /*edx*/)
{
    uint32_t counter = CSF_AT(uint32_t, self, 0x7C);
    if (counter < 0xF)
    {
        ++counter;
        CSF_AT(uint32_t, self, 0x7C) = counter;
    }
    return counter;
}

HOOK(0x69ADB0, IncrementSaturatingCounter, "auto");
