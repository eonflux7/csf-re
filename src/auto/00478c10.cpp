#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, WeaponFirearms_FinalizeReset, 0x00476260,
               (void* self, void* edx));

static int __fastcall C_008317c8_ResetLinkedWeaponState(
    C_008317c8* self, void* /*edx*/)
{
    self->vSlotE0();

    void* linked = reinterpret_cast<void*>(self->field_0070);
    if (linked)
    {
        void* entry =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, linked, 0)[0xA8 / 4])(linked, nullptr);
        ((void (__fastcall*)(void*, void*))
            CSF_AT(void**, entry, 0)[0xA4 / 4])(entry, nullptr);

        linked = reinterpret_cast<void*>(self->field_0070);
        entry =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, linked, 0)[0xA8 / 4])(linked, nullptr);
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, entry, 0)[0x34 / 4])(entry, nullptr, 0);

        linked = reinterpret_cast<void*>(self->field_0070);
        entry =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, linked, 0)[0xA8 / 4])(linked, nullptr);
        ((void (__fastcall*)(void*, void*, int))
            CSF_AT(void**, entry, 0)[0x3C / 4])(entry, nullptr, 0);
    }

    void* entry = reinterpret_cast<void*>(self->vResolveEntryLookupValue());
    if (entry &&
        ((bool (__fastcall*)(void*, void*))
            CSF_AT(void**, entry, 0)[0xA8 / 4])(entry, nullptr))
    {
        ((void (__fastcall*)(void*, void*))
            CSF_AT(void**, entry, 0)[0xA4 / 4])(entry, nullptr);
    }

    return WeaponFirearms_FinalizeReset(self, nullptr);
}

HOOK(0x00478C10, C_008317c8_ResetLinkedWeaponState, "auto");
