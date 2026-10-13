#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, RwRasterShowRaster, 0x006276E0, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GetFpsStatsFromGlobal, 0x006B2B50, (void* self, void* edx));
CSF_FUNC(void, __fastcall, GfxIm3d_UpdateFrameRates, 0x006E2D50, (void* self, void*));

// Shows the render engine's raster (vsync flag taken from field_0028), then
// forwards the FPS stats object from the global to the frame-rate updater.
static int __fastcall CRenderEngine_ShowRasterAndUpdateFrameRates(CRenderEngine* self, void* /*edx*/)
{
    int showFlag = self->field_0028 ? 1 : 0;
    RwRasterShowRaster(static_cast<int>(self->field_0038), 0, showFlag);

    C_00860288* globalInt = reinterpret_cast<C_00860288*>(GetGlobalInt());
    int fpsStats = GetFpsStatsFromGlobal(globalInt, nullptr);
    if (fpsStats != 0) {
        // Original tail-jumps to GfxIm3d_UpdateFrameRates; its result is returned unchanged.
        GfxIm3d_UpdateFrameRates(reinterpret_cast<void*>(static_cast<uintptr_t>(fpsStats)), nullptr);
        return 0;
    }
    return fpsStats;
}

HOOK(0x006DB810, CRenderEngine_ShowRasterAndUpdateFrameRates, "auto");
