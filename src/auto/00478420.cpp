#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008317c8_ActivateLinkedEntry(
    C_008317c8* self, void* /*edx*/)
{
    if (!self->field_0070)
        return 0;

    void* entry = reinterpret_cast<void*>(self->vResolveEntryLookupValue());
    void* linked = reinterpret_cast<void*>(self->field_0070);
    void** linkedVtable = CSF_AT(void**, linked, 0);
    void** entryVtable = CSF_AT(void**, entry, 0);

    int value = reinterpret_cast<int(__fastcall*)(void*, void*, int, int)>(
        linkedVtable[0xA8 / 4])(linked, nullptr, 100, 0);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        entryVtable[0xA0 / 4])(entry, nullptr, value);

    linked = reinterpret_cast<void*>(self->field_0070);
    void* resolved = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, linked, 0)[0xA8 / 4])(linked, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, resolved, 0)[0x34 / 4])(resolved, nullptr, 1);

    linked = reinterpret_cast<void*>(self->field_0070);
    resolved = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, linked, 0)[0xA8 / 4])(linked, nullptr);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, resolved, 0)[0x3C / 4])(resolved, nullptr, 1);
}

HOOK(0x00478420, C_008317c8_ActivateLinkedEntry, "auto");
