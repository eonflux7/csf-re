#include <cstdint>
#include "hook.h"

// Returns the 32-bit value stored at offset +0x20 of the object/struct
// passed in. Plain field getter (no side effects, no dependencies).
static int32_t __cdecl GetFieldAtOffset32(int32_t a1)
{
    return *reinterpret_cast<int32_t*>(a1 + 0x20);
}

HOOK(0x0069FFD0, GetFieldAtOffset32, "auto");
