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

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AppMain_GetDisplayRaster, 0x00534FA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwRasterRenderScaled, 0x00627470, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwRasterPushContext, 0x006275B0, (int raster));
CSF_FUNC(int, __cdecl, RwRasterPopContext, 0x00627650, ());

// Prepares the screen-effect layer: pushes the raster context, renders the
// display raster scaled into the rectangle taken from the parameter block,
// then pops the context.
static int __stdcall ScreenEffects_PrepareLayer(int a1)
{
    auto* globalObj = reinterpret_cast<C_00860130*>(GetGlobalInt());
    auto* menuObj = reinterpret_cast<CMenuMpNetworkType*>(LookupObjectById(globalObj, nullptr));

    int rasterRect[4];
    rasterRect[0] = 0;
    rasterRect[1] = 0;

    uint8_t* params = reinterpret_cast<uint8_t*>(a1);
    rasterRect[2] = CSF_AT(int, params, 0x0C);
    rasterRect[3] = CSF_AT(int, params, 0x10);

    RwRasterPushContext(a1);
    int displayRaster = AppMain_GetDisplayRaster(menuObj, nullptr);
    RwRasterRenderScaled(displayRaster,
                         static_cast<int>(reinterpret_cast<intptr_t>(rasterRect)));
    return RwRasterPopContext();
}

HOOK(0x709360, ScreenEffects_PrepareLayer, "auto");
