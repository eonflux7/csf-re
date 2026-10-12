#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: RwRasterRenderScaled @ 0x00627470, __cdecl, pops 8.
CSF_FUNC(int, __cdecl, RwRasterRenderScaled, 0x00627470, (int a1, int a2));

// Original: 0x737110, __thiscall, ret 0.
// If the raster at +0x1C is set, pack four dwords (+0x24, +0x28, +0x3C, +0x40)
// into a local array and render the raster with that array; otherwise return 0.
static int __fastcall C_00867260_RenderRasterScaled(C_00867260* self, void* /*edx*/) {
    int raster = CSF_AT(int, self, 0x1c);
    if (raster == 0) {
        return raster;
    }

    uint32_t renderParams[4];
    renderParams[0] = CSF_AT(uint32_t, self, 0x24);
    renderParams[1] = CSF_AT(uint32_t, self, 0x28);
    renderParams[2] = CSF_AT(uint32_t, self, 0x3c);
    renderParams[3] = CSF_AT(uint32_t, self, 0x40);

    return RwRasterRenderScaled(raster, static_cast<int>(reinterpret_cast<uintptr_t>(renderParams)));
}

HOOK(0x737110, C_00867260_RenderRasterScaled, "auto");
