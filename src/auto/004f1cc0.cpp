#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall HudStringShow_AddFlags(void* self, void* /*edx*/, int flags)
{
    CSF_AT(uint32_t, self, 0x20) |= static_cast<uint32_t>(flags);
    return flags;
}

HOOK(0x004F1CC0, HudStringShow_AddFlags, "auto");
