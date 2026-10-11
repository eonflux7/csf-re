#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008317c8_ResetLinkedWeapon(C_008317c8* self, void* /*edx*/)
{
    void* linked = reinterpret_cast<void*>(self->field_0070);
    if (!linked)
        return 0;

    void* entry = reinterpret_cast<void*>(
        ((int (__fastcall*)(void*, void*))CSF_AT(void**, linked, 0)[0xA8 / 4])
        (linked, nullptr));
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, entry, 0)[0xA4 / 4])
        (entry, nullptr);

    linked = reinterpret_cast<void*>(self->field_0070);
    entry = reinterpret_cast<void*>(
        ((int (__fastcall*)(void*, void*))CSF_AT(void**, linked, 0)[0xA8 / 4])
        (linked, nullptr));
    ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, entry, 0)[0x34 / 4])
        (entry, nullptr, 0);

    linked = reinterpret_cast<void*>(self->field_0070);
    entry = reinterpret_cast<void*>(
        ((int (__fastcall*)(void*, void*))CSF_AT(void**, linked, 0)[0xA8 / 4])
        (linked, nullptr));
    return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, entry, 0)[0x3C / 4])
        (entry, nullptr, 0);
}

HOOK(0x00478480, C_008317c8_ResetLinkedWeapon, "auto");
