#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GfxParticleEmitters_ClearEmbeddedState, 0x00703710, (int* a1));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, GfxParticleEmitters_ReleaseBaseState, 0x00703300, (void* self, void* edx));

static void __fastcall GfxParticleEmitters_DestroyEmbeddedState(S_00703300* self, void* /*edx*/)
{
    GfxParticleEmitters_ClearEmbeddedState(reinterpret_cast<int*>(self));

    if (self->field_0010)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0010));
        self->field_0010 = 0;
    }

    // Tail jump to the base-state release routine; `this` is forwarded in ecx, no stack args.
    GfxParticleEmitters_ReleaseBaseState(self, nullptr);
}

HOOK(0x007037C0, GfxParticleEmitters_DestroyEmbeddedState, "auto");
