#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(void*, __fastcall, SceneLoader_ResetRenderWorld, 0x006B70D0, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, ReleaseWorldSectorList, 0x006B7A00, (uint32_t* a1, int a2, int* a3, int a4, int a5));

// Tears down the render-world wrapper object: releases the world, its sector list
// and the block at +0x08, then re-points the vtable to the base class and runs the base destructor.
static void __fastcall C_00860388_DestroyBase(C_00860388* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00860388);

    SceneLoader_ResetRenderWorld(self, nullptr);

    if (self->field_0014) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }

    // Original pushes: &local (a3), *(block+008) as a4, the block pointer itself as a5.
    int* sectorHead = reinterpret_cast<int*>(self->field_0008);
    int firstSectorWord = *sectorHead;
    int releasedSlot = 0;
    uint32_t* sectorListOwner = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 4);
    ReleaseWorldSectorList(sectorListOwner, 0, &releasedSlot, firstSectorWord, static_cast<int>(self->field_0008));

    if (self->field_0008) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));
    }
    self->field_0008 = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;

    *(void***)self = CSF_VTBL(0x00860378);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006B7F00, C_00860388_DestroyBase, "auto");
