#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterClear, 0x006274B0, (int a1));
CSF_FUNC(int, __cdecl, RwRasterRenderFast, 0x00627510, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwRasterPushContext, 0x006275B0, (int raster));
CSF_FUNC(int, __cdecl, RwRasterPopContext, 0x00627650, ());
CSF_FUNC(int, __fastcall, CRenderEngine_RenderRasterTransition, 0x006DBB60, (void* self, void* edx, int a2, int a3, char a4, int a5, int a6));

static int __fastcall CRenderEngine_RenderProgressFrame(
    CRenderEngine* self, void* /*edx*/, int progress)
{
    const int scaledProgress =
        static_cast<int32_t>(static_cast<uint32_t>(progress) * 4);
    const int frameIndex = scaledProgress / 100 % 4;
    const int offsetX = frameIndex <= 0 ? 0 : 0x44;
    const int offsetY = frameIndex <= 0 ? 0 : 0x46;

    const int raster = static_cast<int>(
        CSF_AT(uint32_t, &self->field_0040, frameIndex * 4));

    if (frameIndex > 0)
    {
        return CRenderEngine_RenderRasterTransition(
            self, nullptr, static_cast<int>(self->field_0040),
            raster, 1, offsetX, offsetY);
    }

    if (raster)
    {
        RwRasterPushContext(static_cast<int>(self->field_0038));
        RwRasterClear(0);
        const int y = static_cast<int>(self->field_0054 + offsetY);
        const int x = static_cast<int>(self->field_0050 + offsetX);
        RwRasterRenderFast(raster, x, y);
        return RwRasterPopContext();
    }

    return raster;
}

HOOK(0x006DBBF0, CRenderEngine_RenderProgressFrame, "auto");
