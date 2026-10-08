#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Pointer to the campaign profile name string used as the button caption.
CSF_GLOBAL(const char*, g_pszMenu_P_Campan, 0x00899AD8);

// 0x0053B970
// Clears the campaign listbox (at +0x28), configures the campaign button (at +0x2C,
// owner = this, index 0) and finally sets the button caption to the current
// campaign profile-name string.
static int __fastcall CMenuProfiles_ResetCampaignControls(CMenuProfiles* self, void* /*edx*/)
{
    void* pCampaignListbox = CSF_AT(void*, self, 0x28);
    // listbox->vtbl[0x40/4]() : reset/clear contents (no stack args)
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, pCampaignListbox, 0)[0x40 / 4])(pCampaignListbox, nullptr);

    void* pCampaignButton = CSF_AT(void*, self, 0x2c);
    // button->vtbl[0x60/4](this, 0) : attach to this menu / select index 0
    ((void(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, pCampaignButton, 0)[0x60 / 4])(pCampaignButton, nullptr, self, 0);

    // Reload the button pointer: the previous call may have modified the object.
    pCampaignButton = CSF_AT(void*, self, 0x2c);
    // button->vtbl[0x58/4](text) : set caption
    return ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, pCampaignButton, 0)[0x58 / 4])(pCampaignButton, nullptr, g_pszMenu_P_Campan);
}

HOOK(0x0053B970, CMenuProfiles_ResetCampaignControls, "auto");
