#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Stores the given 32-bit value into this+0x18 and returns it unchanged.
static int __fastcall SetValue18(void* self, void* /*edx*/, int value)
{
    CSF_AT(uint32_t, self, 0x18) = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x0050AAB0, SetValue18, "auto");
