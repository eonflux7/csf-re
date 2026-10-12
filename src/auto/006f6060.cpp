#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, S_006d2390_ReleaseBuffers, 0x006D2440, (void* self, void* edx));

// Releases the two S_006d2390 buffer members and the auxiliary block at +0x08.
// Original is __thiscall with ret 0, so no stack bytes are popped.
static int __fastcall GfxScene_ReleaseAuxiliarySceneData(C_00862874* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00862874);

    S_006d2390_ReleaseBuffers(reinterpret_cast<void*>(&self->field_0058), nullptr);
    S_006d2390_ReleaseBuffers(reinterpret_cast<void*>(&self->field_0018), nullptr);

    int result = static_cast<int>(self->field_0008);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0008));
        self->field_0008 = 0;
    }
    return result;
}

HOOK(0x006F6060, GfxScene_ReleaseAuxiliarySceneData, "auto");
