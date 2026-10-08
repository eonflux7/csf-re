#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Pointer-to-string global used as the credits panel's pending text.
CSF_GLOBAL(const char*, g_pszMenu_P_MePrin, 0x00899AE4);

// Initializes the credits text sub-object (slot 24 call) and, when no
// pending text id is stored, immediately pushes the default credits text
// (slot 22 call) into it. Returns the pending id if one was set, otherwise
// the result of the text-set call.
static int __fastcall CMenuCredits_InitTitlePanel(CMenuCredits* self, void* /*edx*/)
{
    void* pPanel = reinterpret_cast<void*>(self->field_0010);
    ((void(__fastcall*)(void*, void*, CMenuCredits*, int))(*reinterpret_cast<void***>(pPanel))[24])(
        pPanel, nullptr, self, 0);

    int nPendingText = static_cast<int>(self->field_0014);
    if (!nPendingText)
    {
        void* pPanel2 = reinterpret_cast<void*>(self->field_0010);
        return ((int(__fastcall*)(void*, void*, const char*))(*reinterpret_cast<void***>(pPanel2))[22])(
            pPanel2, nullptr, g_pszMenu_P_MePrin);
    }
    return nPendingText;
}

HOOK(0x0052F810, CMenuCredits_InitTitlePanel, "auto");
