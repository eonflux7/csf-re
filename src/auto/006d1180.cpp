#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Inserts a light into the world (RpWorldAddLight-style). Callee pops one stack argument.
CSF_FUNC(uint32_t*, __fastcall, Lights_WorldInsert, 0x006DA140, (void* self, void* edx, void* a2));

// Returns the result of inserting this light's world entry. When the owner flag at +0x3C is
// set, the second argument is the first dword of the object pointed to by +0x34; otherwise it
// is null.
static uint32_t* __fastcall Lights_AttachLightToWorld(void* self, void* /*edx*/)
{
    uint32_t worldHandle = CSF_AT(uint32_t, self, 0x08);

    if (CSF_AT(uint32_t, self, 0x3C) != 0)
    {
        uint32_t lightDataPtr = CSF_AT(uint32_t, self, 0x34);
        uint32_t lightData = *reinterpret_cast<uint32_t*>(lightDataPtr);
        return Lights_WorldInsert(reinterpret_cast<void*>(worldHandle), nullptr,
                                  reinterpret_cast<void*>(lightData));
    }
    return Lights_WorldInsert(reinterpret_cast<void*>(worldHandle), nullptr, nullptr);
}

HOOK(0x006D1180, Lights_AttachLightToWorld, "auto");
