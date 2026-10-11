#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, WeaponFirearms_DestroyMembers, 0x00478C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, WeaponFirearms_DestroyBase, 0x004764C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int* __fastcall C_008317c8_Destroy(C_008317c8* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x008317C8);
    WeaponFirearms_DestroyMembers(self, nullptr);
    CSF_AT(void*, self, 0x64) = CSF_VTBL(0x0082D410);
    WeaponFirearms_DestroyBase(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x00479480, C_008317c8_Destroy, "auto");
