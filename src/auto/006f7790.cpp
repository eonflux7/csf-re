#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __cdecl, GfxFrames_ApplyToClumpAtomics, 0x006C01C0, (int clump, int data));

static void __fastcall C_008628b8_ReleaseResources(C_008628b8* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00862900);

    uint32_t renderObjectRaw = self->field_000c;
    if (renderObjectRaw) {
        void* renderObject = reinterpret_cast<void*>(renderObjectRaw);
        auto releaseSlot = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, renderObject, 0)[1]);
        releaseSlot(renderObject, nullptr, 1);
        self->field_000c = 0;
    }

    GfxFrames_ApplyToClumpAtomics((int)self->field_0004,
                                  (int)(uintptr_t)CSF_ADDR_FN(0x006F7720));

    uint32_t ownedBlock = CSF_AT(uint32_t, self, 0x14);
    if (ownedBlock) {
        CoreMemory_Free(reinterpret_cast<void*>(ownedBlock));
        CSF_AT(uint32_t, self, 0x14) = 0;
    }

    *(void***)self = CSF_VTBL(0x008628B8);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006F7790, C_008628b8_ReleaseResources, "auto");
