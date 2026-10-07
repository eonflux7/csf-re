#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the 32-bit value stored at offset 0x168 of the object
// (inherited portion of the class, not part of C_00865328's own declared fields).
static uint32_t __fastcall C_00865328_GetField0168(C_00865328* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x168);
}

HOOK(0x00724EF0, C_00865328_GetField0168, "auto");
