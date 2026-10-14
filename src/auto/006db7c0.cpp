#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, RwEngineStop, 0x00626CE0, ());
CSF_FUNC(int, __cdecl, RwEngineClose, 0x00626E30, ());

// Frees the screen/back rasters and stops/closes the RenderWare engine as needed.
// Returns the whole eax of the original: the last 32-bit value written to it (the field_0038 load, or the
// return of the RwRasterDestroy / RwEngineStop call after it), with only the low byte replaced by field_0034.
static int __fastcall CRenderEngine_ReleaseRastersAndEngine(CRenderEngine* self, void* /*edx*/)
{
    if (self->field_003c)
    {
        RwRasterDestroy(self->field_003c);
        self->field_003c = 0;
    }
    uint32_t eax = self->field_0038;   // mov eax, [esi+0x38]
    if (eax)
    {
        eax = RwRasterDestroy(eax);
        self->field_0038 = 0;
    }
    if (self->field_0058)
        eax = RwEngineStop();

    uint8_t engineStarted = self->field_0034;   // mov al, [esi+0x34]: low byte only
    if (engineStarted)
        return RwEngineClose();   // tail jump: forwards eax from RwEngineClose
    return static_cast<int>((eax & 0xFFFFFF00u) | engineStarted);
}

HOOK(0x006DB7C0, CRenderEngine_ReleaseRastersAndEngine, "auto");
