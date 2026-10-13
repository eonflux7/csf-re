#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpWorldForAllWorldSectors, 0x0058CFF0, (int a1, int (__cdecl* a2)(int*, int), int a3));
CSF_FUNC(int*, __cdecl, RpWorldStreamRead, 0x0059FEA0, (int a1));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwStreamFindChunk, 0x00629AE0, (uint32_t* a1, int a2, uint32_t* a3, unsigned int* a4));
CSF_FUNC(void*, __fastcall, SceneLoader_ResetRenderWorld, 0x006B70D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxObjectProps_ForEachWorldMaterial, 0x006BBEF0, (int world));

static char __fastcall C_00860388_LoadVisibilityResource(
    C_00860388* self, void* /*edx*/, int resource)
{
    SceneLoader_ResetRenderWorld(self, nullptr);

    uint32_t* stream = RwStreamOpen(2, 1, resource);
    if (!stream)
        return 0;

    if (RwStreamFindChunk(stream, 11, nullptr, nullptr))
        self->field_0010 = reinterpret_cast<uint32_t>(
            RpWorldStreamRead(reinterpret_cast<int>(stream)));

    RwStreamClose(stream, nullptr);

    uint32_t world = self->field_0010;
    if (world)
        GfxObjectProps_ForEachWorldMaterial(static_cast<int>(world));

    world = self->field_0010;
    self->field_0027 = 0;
    if (world)
    {
        RpWorldForAllWorldSectors(
            static_cast<int>(world),
            reinterpret_cast<int (__cdecl*)(int*, int)>(CSF_ADDR_FN(0x006B6B50)),
            reinterpret_cast<int>(self));
    }

    world = self->field_0010;
    if (world && self->field_0027)
    {
        RpWorldForAllWorldSectors(
            static_cast<int>(world),
            reinterpret_cast<int (__cdecl*)(int*, int)>(CSF_ADDR_FN(0x006B6B70)),
            reinterpret_cast<int>(self));
    }

    return 1;
}

HOOK(0x006B7190, C_00860388_LoadVisibilityResource, "auto");
