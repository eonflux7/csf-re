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

CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));
CSF_FUNC(int, __cdecl, RwTextureCreate, 0x00631780, (int a1));
CSF_FUNC(int, __cdecl, RxPipelineSetMaxNodes, 0x006337F0, (int a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, Water_GetRasterSize, 0x006D59D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Water_ReleaseTargets, 0x00709A80, (int a1, int a2, int a3, int a4));
CSF_FUNC(char, __fastcall, Water_PreCreate, 0x0070D3F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Water_InitTargets, 0x0070D4E0, (void* self, void* edx, unsigned int a2, int a3, int a4));

static char __fastcall Water_CreateRenderTargets(S_006fdbb0* self, void* /*edx*/)
{
    char ok = 1;

    Water_PreCreate(self, nullptr);
    RxPipelineSetMaxNodes(0);

    int height = Water_GetRasterSize(reinterpret_cast<void*>(self->field_00f8), nullptr);
    int width = Water_GetRasterSize(reinterpret_cast<void*>(self->field_00f8), nullptr);
    uint32_t raster = (uint32_t)RwRasterCreate(width, height, 0, 1);
    self->field_00f4 = raster;
    if (!raster) {
        ok = 0;
    }
    RxPipelineSetMaxNodes(1);

    uint8_t* sub = reinterpret_cast<uint8_t*>(self) + 0xDC;
    if (self->field_00e0) {
        Water_ReleaseTargets((int)self->field_00e0, (int)CSF_AT(uint32_t, self, 0xE4), (int)sub, 0);
        if (self->field_00e0) {
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_00e0));
        }
    }
    self->field_00e0 = 0;
    CSF_AT(uint32_t, self, 0xE4) = 0;
    CSF_AT(uint32_t, self, 0xE8) = 0;

    if (!ok) {
        return 0;
    }

    Water_InitTargets(sub, nullptr, 2, 0, 0);

    char result = ok;
    for (int i = 0; i < 16; i += 8) {
        result = ok;
        if (!ok) {
            break;
        }
        int size = Water_GetRasterSize(reinterpret_cast<void*>(self->field_00f8), nullptr);
        uint32_t* slot = reinterpret_cast<uint32_t*>(self->field_00e0 + i);
        int r = RwRasterCreate(size, size, 32, 1285);
        slot[0] = (uint32_t)r;
        if (r) {
            slot[1] = (uint32_t)RwTextureCreate(r);
            result = 1;
        } else {
            result = 0;
        }
        ok = result;
        uint32_t tex = CSF_AT(uint32_t, self->field_00e0 + i, 4);
        if (tex) {
            CSF_AT(uint32_t, tex, 0x50) = (CSF_AT(uint32_t, tex, 0x50) & 0xFFFFFF00u) | 2u;
        }
    }
    return result;
}

HOOK(0x0070D5C0, Water_CreateRenderTargets, "auto");
