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

CSF_FUNC(int, __cdecl, RwRasterClear, 0x006274B0, (int a1));
CSF_FUNC(int, __cdecl, RwRasterRenderFast, 0x00627510, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwRasterPushContext, 0x006275B0, (int raster));
CSF_FUNC(int, __cdecl, RwRasterPopContext, 0x00627650, ());

// Draws raster a2 (and optionally overlay a3) at the engine's stored offset, or at
// offset (a5, a6) for the overlay. Brackets the drawing with push/pop context.
static int __fastcall CRenderEngine_DrawRasterPair(CRenderEngine* self, void* /*edx*/,
                                                   int a2, int a3, char a4,
                                                   int a5, int a6)
{
    if (a2 == 0) {
        return 0;
    }

    RwRasterPushContext(self->field_0038);
    if (a4) {
        RwRasterClear(0);
    }

    if (a3 != 0) {
        RwRasterRenderFast(a2, self->field_0050, self->field_0054);
        RwRasterRenderFast(a3, a5 + self->field_0050, a6 + self->field_0054);
    } else {
        RwRasterRenderFast(a2, a5 + self->field_0050, a6 + self->field_0054);
    }
    return RwRasterPopContext();
}

HOOK(0x6DBB60, CRenderEngine_DrawRasterPair, "auto");
