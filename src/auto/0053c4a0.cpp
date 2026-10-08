#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Global holding the menu name pointer for the campaign ('P_Campan..') screen.
CSF_GLOBAL(const char*, g_pszMenu_P_Campan, 0x00899AD8);

// CMenuDemoExit::SwitchToCampaign  (vtable slot target at 0x0053C4A0)
// Clears the pending-menu field, tells the manager to dismiss this demo screen
// (slot 0x60) and then asks the manager to show the campaign menu (slot 0x58).
static int __fastcall CMenuDemoExit_SwitchToCampaign(CMenuDemoExit* self, void* /*edx*/)
{
    void* pManager = reinterpret_cast<void*>(self->m_pManager);
    self->m_pPendingMenu = 0;

    // manager->vtbl[0x60 / 4](self, 0)  -- __thiscall, callee pops 8 bytes
    ((void (__fastcall*)(void*, void*, CMenuDemoExit*, int))
        CSF_AT(void**, pManager, 0)[0x60 / 4])(pManager, nullptr, self, 0);

    void* pManagerReloaded = reinterpret_cast<void*>(self->m_pManager);

    // return manager->vtbl[0x58 / 4](g_pszMenu_P_Campan)  -- __thiscall, callee pops 4 bytes
    return ((int (__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, pManagerReloaded, 0)[0x58 / 4])(pManagerReloaded, nullptr, g_pszMenu_P_Campan);
}

HOOK(0x0053C4A0, CMenuDemoExit_SwitchToCampaign, "auto");
