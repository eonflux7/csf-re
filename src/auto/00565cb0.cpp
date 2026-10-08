#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Global holding the const char* menu-entry key (value is loaded at runtime).
CSF_GLOBAL(const char*, g_pszMenu_P_LbMePr, 0x00899B40);

// Activates the DMatch entry: calls the owning menu's vtable slot 24 (x,1),
// looks up the entry by key via slot 23, then calls slot 8 on the returned
// object and slot 42 with 0 on that result, returning the final value.
static int __fastcall CMenuMpGameMode_SelectDeathMatchMode(CMenuMpGameMode* self, void* /*edx*/)
{
    void* owningMenu = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014));

    // owningMenu->vtbl[0x60](owningMenu, self, 1)
    ((void(__fastcall*)(void*, void*, CMenuMpGameMode*, int))
        CSF_AT(void**, owningMenu, 0)[0x60 / 4])(owningMenu, nullptr, self, 1);

    // v2 = owningMenu->vtbl[0x5C](owningMenu, g_pszMenu_P_LbMePr)
    void* entry = ((void*(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, owningMenu, 0)[0x5C / 4])(owningMenu, nullptr, g_pszMenu_P_LbMePr);

    // v3 = entry->vtbl[0x20](entry)
    void* entryItem = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, entry, 0)[0x20 / 4])(entry, nullptr);

    // return entryItem->vtbl[0xA8](entryItem, 0)
    return ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, entryItem, 0)[0xA8 / 4])(entryItem, nullptr, 0);
}

HOOK(0x00565CB0, CMenuMpGameMode_SelectDeathMatchMode, "auto");
