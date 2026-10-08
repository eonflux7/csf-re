#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Pointer to the current profile name string used as the button caption.
CSF_GLOBAL(const char*, g_pszMenu_P_MePrin, 0x00899AE4);

// 0x0053B940
// Clears the "Profiles" listbox, configures the profile button (owner = this, index 0)
// and sets the button caption to the current profile-name string.
static int __fastcall CMenuProfiles_ResetProfileControls(CMenuProfiles* self, void* /*edx*/)
{
    void* pProfilesListbox = CSF_AT(void*, self, 0x28);
    // listbox->vtbl[0x40/4]() : reset/clear contents (no stack args)
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, pProfilesListbox, 0)[0x40 / 4])(pProfilesListbox, nullptr);

    void* pProfileButton = CSF_AT(void*, self, 0x2c);
    // button->vtbl[0x60/4](this, 0) : attach to this menu / select index 0
    ((void(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, pProfileButton, 0)[0x60 / 4])(pProfileButton, nullptr, self, 0);

    // Reload the button pointer: the previous call may have modified the object.
    pProfileButton = CSF_AT(void*, self, 0x2c);
    // button->vtbl[0x58/4](text) : set caption
    return ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, pProfileButton, 0)[0x58 / 4])(pProfileButton, nullptr, g_pszMenu_P_MePrin);
}

HOOK(0x0053B940, CMenuProfiles_ResetProfileControls, "auto");
