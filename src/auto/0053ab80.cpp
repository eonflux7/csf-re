#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Pointer-to-string global used as the default title text of the options panel.
CSF_GLOBAL(const char*, g_pszMenu_P_MePrin, 0x00899AE4);

// Initializes the title text sub-object at +0x14 (slot 24 call) and, when no
// pending text id is stored at +0x18, immediately pushes the default title
// text (slot 22 call) into it. Returns the pending id if one was set,
// otherwise the result of the text-set call.
static int __fastcall CMenuOptionsPC_InitTitlePanel(CMenuOptionsPC* self, void* /*edx*/)
{
    void* pPanel = reinterpret_cast<void*>(self->field_0014);
    ((void(__fastcall*)(void*, void*, CMenuOptionsPC*, int))(*reinterpret_cast<void***>(pPanel))[24])(
        pPanel, nullptr, self, 0);

    int nPendingText = static_cast<int>(self->field_0018);
    if (!nPendingText)
    {
        void* pPanel2 = reinterpret_cast<void*>(self->field_0014);
        return ((int(__fastcall*)(void*, void*, const char*))(*reinterpret_cast<void***>(pPanel2))[22])(
            pPanel2, nullptr, g_pszMenu_P_MePrin);
    }
    return nPendingText;
}

HOOK(0x0053AB80, CMenuOptionsPC_InitTitlePanel, "auto");
