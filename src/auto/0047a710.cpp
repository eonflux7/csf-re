#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, WeaponFirearms_UpdateBase, 0x00476260, (void* self, void* edx));

static int __fastcall C_008319c0_UpdateResolvedEntry(C_008319c0* self, void* /*edx*/)
{
    void* entry = reinterpret_cast<void*>(self->vResolveEntryLookupValue());
    if (entry &&
        reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[0xA8 / 4])(entry, nullptr))
    {
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[0xA4 / 4])(entry, nullptr);
    }

    return WeaponFirearms_UpdateBase(self, nullptr);
}

HOOK(0x0047A710, C_008319c0_UpdateResolvedEntry, "auto");
