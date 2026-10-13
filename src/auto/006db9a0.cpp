#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwEngineStart, 0x00626D60, ());
CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));
CSF_FUNC(int, __cdecl, AppMain_ReportInitializationError, 0x0069CDF0, (const char* caption, char* format, ...));
CSF_FUNC(int, __cdecl, ParticleSystemDefs_ConvertAndShowMessage, 0x00701EC0, (int a1, int a2));

// Creates the color raster (flags 2, depth from +0x14) and the depth raster (flags 1, depth 0)
// for a render target described by the object's width (+4), height (+8) and depth (+0x14).
static char __fastcall RenderEngine_CreateFramebufferRasters(void* self, void* /*edx*/)
{
    if (!RwEngineStart())
    {
        AppMain_ReportInitializationError(CSF_STR(0x0083F688, "ERROR"),
                                          const_cast<char*>(CSF_STR(0x00861308, "RwEngineStart() ha fallado")));
        return 0;
    }

    CSF_AT(uint8_t, self, 0x58) = 1;

    int colorRaster = RwRasterCreate(CSF_AT(int, self, 0x04),
                                     CSF_AT(int, self, 0x08),
                                     CSF_AT(int, self, 0x14),
                                     2);
    CSF_AT(int, self, 0x38) = colorRaster;
    if (!colorRaster)
    {
        ParticleSystemDefs_ConvertAndShowMessage((int)CSF_ADDR_DATA(0x008612F8),
                                                 (int)CSF_ADDR_DATA(0x00861300));
        return 0;
    }

    int depthRaster = RwRasterCreate(CSF_AT(int, self, 0x04),
                                     CSF_AT(int, self, 0x08),
                                     0,
                                     1);
    CSF_AT(int, self, 0x3C) = depthRaster;
    if (!depthRaster)
    {
        ParticleSystemDefs_ConvertAndShowMessage((int)CSF_ADDR_DATA(0x008612F8),
                                                 (int)CSF_ADDR_DATA(0x008612F0));
        return 0;
    }

    return 1;
}

HOOK(0x006DB9A0, RenderEngine_CreateFramebufferRasters, "auto");
