#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callees, bound by address.
CSF_FUNC(int, __fastcall, DevConsole_ReleaseResource, 0x004FB970, (void* self, void*));
CSF_FUNC(char, __fastcall, DevConsole_AcquireAuxiliaryResource, 0x004FBA30, (void* self, void* edx));

// Flag byte at +0 tracks whether the resource is held; dwords at +4 and +8 are reset
// before the acquire call. The acquire result is returned, and on failure the
// resource is released and the flag cleared.
static char __fastcall GameBddLoader_InitializeMenuAuxiliaryState(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0))
    {
        DevConsole_ReleaseResource(self, nullptr);
        CSF_AT(uint8_t, self, 0) = 0;
    }
    CSF_AT(uint32_t, self, 4) = 0;
    CSF_AT(uint32_t, self, 8) = 0;

    char acquired = DevConsole_AcquireAuxiliaryResource(self, nullptr);
    CSF_AT(uint8_t, self, 0) = 1;

    if (!acquired)
    {
        DevConsole_ReleaseResource(self, nullptr);
        CSF_AT(uint8_t, self, 0) = 0;
    }
    return acquired;
}

HOOK(0x4FBBA0, GameBddLoader_InitializeMenuAuxiliaryState, "auto");
