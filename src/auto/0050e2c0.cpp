#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees and data, bound by original address.
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_GLOBAL(const char*, g_pszMenu_P_Profil, 0x00899AE0);
CSF_GLOBAL(const char*, g_pszMenu_P_Campan, 0x00899AD8);

// Shows the profile panel when the global flag at +0xCC is set; otherwise runs the
// slot-0x70 hook on this manager and then shows the campaign panel.
static int __fastcall CMenuManager_ShowProfileOrCampaignPanel(CMenuManager* self, void* /*edx*/) {
    void* globalObject = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_75FDF0()));
    if (CSF_AT(uint8_t, globalObject, 0xCC)) {
        void* panelOwner = reinterpret_cast<void*>(self->field_0014);
        return ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, panelOwner, 0)[0x58 / 4])(
            panelOwner, nullptr, g_pszMenu_P_Profil);
    }

    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x70 / 4])(self, nullptr);

    void* panelOwner = reinterpret_cast<void*>(self->field_0014);
    return ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, panelOwner, 0)[0x58 / 4])(
        panelOwner, nullptr, g_pszMenu_P_Campan);
}

HOOK(0x0050E2C0, CMenuManager_ShowProfileOrCampaignPanel, "auto");
