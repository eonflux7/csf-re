#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static unsigned int __fastcall SceneObjectOptions_ReleaseAndResetStorage(void* self, void* /*edx*/)
{
    uint32_t result = CSF_AT(uint32_t, self, 0x38);

    if (result >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x24);
        if (result)
            result = static_cast<uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result))));
    }

    CSF_AT(uint32_t, self, 0x38) = 15;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint8_t, self, 0x24) = 0;

    if (CSF_AT(uint32_t, self, 0x1C) >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x08);
        if (result)
            result = static_cast<uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result))));
    }

    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 15;
    CSF_AT(uint8_t, self, 0x08) = 0;

    return result;
}

HOOK(0x4018E0, SceneObjectOptions_ReleaseAndResetStorage, "auto");
