#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00862cd4_ReleaseResources, 0x006FB5A0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_008629a4_DestroyBase, 0x006F8FF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int* __fastcall C_00862cd4_Destroy(C_00862cd4* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x00862CD4);
    CSF_AT(uint32_t, self, 0x04) =
        reinterpret_cast<uint32_t>(CSF_VTBL(0x00862CB8));

    C_00862cd4_ReleaseResources(self, nullptr);
    C_008629a4_DestroyBase(self, nullptr);

    if ((flags & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<int*>(self);
}

HOOK(0x006FBAA0, C_00862cd4_Destroy, "auto");
