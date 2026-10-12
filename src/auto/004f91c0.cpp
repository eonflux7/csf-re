#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall DevConsole_ResetState(void* self, void* /*edx*/, char preserveSecondaryState)
{
    CSF_AT(std::uint32_t, self, 0x08) = 0;
    if (!preserveSecondaryState)
    {
        CSF_AT(std::uint32_t, self, 0x0C) = 0;
        void* linkedObject = CSF_AT(void*, self, 0x04);
        if (linkedObject)
            CSF_AT(std::uint32_t, linkedObject, 0x24) = 0;
    }
    return 0;
}

HOOK(0x004F91C0, DevConsole_ResetState, "auto");
