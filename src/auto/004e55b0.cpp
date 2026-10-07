#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Decrements the 8-bit counter at offset 0x1C when it is non-zero and returns
// the value held in that byte afterwards (the decremented value, or 0 if it
// was already 0).
static char __fastcall DecrementByteCounter(void* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x1C);
    if (result)
    {
        --result;
        CSF_AT(char, self, 0x1C) = result;
    }
    return result;
}

HOOK(0x4E55B0, DecrementByteCounter, "auto");
