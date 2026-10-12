#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies a 0x1C-byte icon-status record from source into target, stamps the
// status-icon vtable at +0x1C, then copies the two trailing words at +0x20/+0x24.
// Returns target (NULL-safe: a null target is returned untouched).
static uint32_t* __stdcall HudStatusIcon_CopyFrom(uint32_t* target, uint32_t* source)
{
    uint32_t* result = target;
    if (target)
    {
        std::memcpy(target, source, 0x1C);
        CSF_AT(void*, target, 0x1C) = CSF_VTBL(0x0082D448);
        CSF_AT(uint32_t, target, 0x20) = CSF_AT(uint32_t, source, 0x20);
        CSF_AT(uint32_t, target, 0x24) = CSF_AT(uint32_t, source, 0x24);
    }
    return result;
}

HOOK(0x00458760, HudStatusIcon_CopyFrom, "auto");
