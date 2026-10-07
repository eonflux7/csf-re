#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns true when the pointer stored at +0x8C is null.
static int __fastcall C_0041c550_IsField8CNull(void* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x8c) == 0;
}

HOOK(0x0041C550, C_0041c550_IsField8CNull, "auto");
