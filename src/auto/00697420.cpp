#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

static int __cdecl GfxScene_IsObjectProcessingEnabled()
{
    const uint32_t begin = CSF_AT(uint32_t, g_pOptions, 0x54);
    if (begin == 0)
        return 0;

    const uint32_t difference = CSF_AT(uint32_t, g_pOptions, 0x58) - begin;
    return (static_cast<int32_t>(difference) >> 5) != 0;
}

HOOK(0x00697420, GfxScene_IsObjectProcessingEnabled, "auto");
