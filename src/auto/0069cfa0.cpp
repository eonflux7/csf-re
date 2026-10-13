#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static uint32_t __fastcall ErrorLogRegistry_ReleaseAndReset(S_0069d0f0* self, void* /*edx*/)
{
    uint32_t result = CSF_AT(uint32_t, self, 0x3C);

    if (result >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x28);
        if (result)
            result = static_cast<uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result))));
    }

    CSF_AT(uint32_t, self, 0x3C) = 15;
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint8_t, self, 0x28) = 0;

    if (CSF_AT(uint32_t, self, 0x20) >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x0C);
        if (result)
            result = static_cast<uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result))));
    }

    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 15;
    CSF_AT(uint8_t, self, 0x0C) = 0;

    return result;
}

HOOK(0x69CFA0, ErrorLogRegistry_ReleaseAndReset, "auto");
