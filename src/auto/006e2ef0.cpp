#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_Value_8BB894, 0x008BB894);

CSF_FUNC(int, __stdcall, GfxParticleRender_LoadRasterFromImage, 0x00705D10, (char* filename));

static int __cdecl GfxIm3d_LoadNamedRaster(int a1)
{
    // The original loads g_Value_8BB894 into ecx but never uses it, so no read is emitted here.
    (void)g_Value_8BB894;
    return GfxParticleRender_LoadRasterFromImage(reinterpret_cast<char*>(static_cast<uintptr_t>(a1)));
}

HOOK(0x006E2EF0, GfxIm3d_LoadNamedRaster, "auto");
