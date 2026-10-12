#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static bool __fastcall NetSpawnMessages_ContainsValue(void* self, void* /*edx*/, int value)
{
    const auto* entries = static_cast<const uint8_t*>(self) + 0x28;
    for (int index = 0; index < 32; ++index)
    {
        if (CSF_AT(int32_t, entries, index * 4) == value)
            return true;
    }
    return false;
}

HOOK(0x00751EC0, NetSpawnMessages_ContainsValue, "auto");
