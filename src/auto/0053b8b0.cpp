#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(const char*, g_pszMenu_P_MOGIng, 0x00899AB8);

// The child control at self+0x14 gets its context/owner set (virtual slot 0x60,
// called with (this, 0)) and then its caption set to g_pszMenu_P_MOGIng
// (virtual slot 0x58). Returns the caption call's result.
static int __fastcall CMenuOptionsPC_ApplyChildContextAndLabel(void* self, void* /*edx*/)
{
    void* child = CSF_AT(void*, self, 0x14);
    ((void (__fastcall*)(void*, void*, void*, int))CSF_AT(void**, child, 0)[0x60 / 4])(child, nullptr, self, 0);

    // Re-read the child pointer: the previous virtual call may replace it.
    child = CSF_AT(void*, self, 0x14);
    const char* label = g_pszMenu_P_MOGIng;
    return ((int (__fastcall*)(void*, void*, const char*))CSF_AT(void**, child, 0)[0x58 / 4])(child, nullptr, label);
}

HOOK(0x0053B8B0, CMenuOptionsPC_ApplyChildContextAndLabel, "auto");
