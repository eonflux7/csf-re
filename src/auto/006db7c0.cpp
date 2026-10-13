#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, RwEngineStop, 0x00626CE0, ());
CSF_FUNC(int, __cdecl, RwEngineClose, 0x00626E30, ());

// Frees the screen/back rasters and stops/closes the RenderWare engine as needed.
static int __fastcall CRenderEngine_ReleaseRastersAndEngine(CRenderEngine* self, void* /*edx*/)
{
    if (self->field_003c)
    {
        RwRasterDestroy(self->field_003c);
        self->field_003c = 0;
    }
    if (self->field_0038)
    {
        RwRasterDestroy(self->field_0038);
        self->field_0038 = 0;
    }
    if (self->field_0058)
        RwEngineStop();

    uint8_t engineStarted = self->field_0034;
    if (engineStarted)
        return RwEngineClose();   // tail jump: forwards eax from RwEngineClose
    return engineStarted;
}

HOOK(0x006DB7C0, CRenderEngine_ReleaseRastersAndEngine, "auto");
