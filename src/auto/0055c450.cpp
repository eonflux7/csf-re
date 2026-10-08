#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original global holding a menu-id string pointer. Only its value is read.
CSF_GLOBAL(const char*, g_pszMenu_P_LbSlCu, 0x00899B30);

// Dword global set to 3 by this routine (edit-screen state).
CSF_GLOBAL(uint32_t, g_dwAccountEditScreenState, 0x0089A5EC);

// Original callees, bound by address.
CSF_FUNC(void, __cdecl, MenuAccounts_ResetPendingEdit, 0x006A6D40, (void));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, (void));
CSF_FUNC(int, __fastcall, MenuCharSelect_FinalizePreviewSlot, 0x0073E920, (int* self, void* edx));

static int __fastcall CMenuAccountEdit_ApplyChangesAndReload(CMenuAccountEdit* self, void* /*edx*/)
{
    // Slot 22 (+0x58) on the child object at +0x14, called with the menu-id string and edx unused.
    void* firstChild = reinterpret_cast<void*>(self->field_0014);
    using SetStringSlotFn = void(__fastcall*)(void*, void*, const char*);
    reinterpret_cast<SetStringSlotFn>(CSF_AT(void**, firstChild, 0)[22])(
        firstChild, nullptr, g_pszMenu_P_LbSlCu);

    g_dwAccountEditScreenState = 3u;

    MenuAccounts_ResetPendingEdit();

    int selection = GetGlobalDword_73C020();
    MenuCharSelect_FinalizePreviewSlot(
        reinterpret_cast<int*>(static_cast<std::intptr_t>(selection)), nullptr);

    // Re-read self->field_0014 after the calls above, as the original does.
    void* secondChild = reinterpret_cast<void*>(self->field_0014);
    using RefreshSlotFn = int(__fastcall*)(void*, void*, void*, int);
    return reinterpret_cast<RefreshSlotFn>(CSF_AT(void**, secondChild, 0)[24])(
        secondChild, nullptr, static_cast<void*>(self), 0);
}

HOOK(0x0055C450, CMenuAccountEdit_ApplyChangesAndReload, "auto");
