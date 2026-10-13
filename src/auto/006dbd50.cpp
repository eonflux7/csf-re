#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwEngineStop, 0x00626CE0, ());
CSF_FUNC(int, __cdecl, RwEngineClose, 0x00626E30, ());
CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Releases the render rasters and RenderWare engine subsystems owned by the
// render engine, then re-initializes the object as its base class C_00860170.
static void __fastcall CRenderEngine_ShutdownAndRebase(CRenderEngine* self, void* /*edx*/)
{
    uint32_t screenshotRaster = self->field_003c;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861328);

    if (screenshotRaster)
    {
        RwRasterDestroy(screenshotRaster);
        self->field_003c = 0;
    }
    if (self->field_0038)
    {
        RwRasterDestroy(self->field_0038);
        self->field_0038 = 0;
    }
    if (self->field_0058)
        RwEngineStop();
    if (self->field_0034)
        RwEngineClose();

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861260);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006DBD50, CRenderEngine_ShutdownAndRebase, "auto");
