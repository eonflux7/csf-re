#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_Value_8BB894, 0x008BB894);
CSF_FUNC(int, __stdcall, GfxParticleRender_GetRasterStorageSize, 0x00705A70, (uint32_t* entry));

static int __cdecl GfxIm3d_GetObjectValue(int entryHandle)
{
    // The original loads g_8BB894 into ecx before the stdcall call; the callee ignores ecx.
    int unusedContextValue = g_Value_8BB894;
    (void)unusedContextValue;
    return GfxParticleRender_GetRasterStorageSize(reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(entryHandle)));
}

HOOK(0x006E2E90, GfxIm3d_GetObjectValue, "auto");
