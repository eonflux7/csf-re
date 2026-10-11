#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, WeaponFirearms_ReleaseResources, 0x00476260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, WeaponFirearms_DestroyBase, 0x004764C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int* __fastcall C_008319c0_Destroy(C_008319c0* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x008319C0);

    void* entry = reinterpret_cast<void*>(
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_VTBL(0x00831A68)[0])(self, nullptr));

    if (entry &&
        reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[0xA8 / 4])(entry, nullptr))
    {
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[0xA4 / 4])(entry, nullptr);
    }

    WeaponFirearms_ReleaseResources(self, nullptr);
    WeaponFirearms_DestroyBase(self, nullptr);
    if (flags & 1)
        Mem_Free(self);

    return reinterpret_cast<int*>(self);
}

HOOK(0x0047AD30, C_008319c0_Destroy, "auto");
