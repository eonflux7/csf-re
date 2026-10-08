#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszMenu_P_MeJuMP, 0x00899AF8);
CSF_GLOBAL(const char*, g_pszMenu_P_LbMePr, 0x00899B40);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFrie, 0x00899B58);

static int __fastcall CMenuMpSessions_NavigateBack(
    CMenuMpSessions* self, void* /*edx*/)
{
    void* menu = reinterpret_cast<void*>(self->field_0044);
    ((void (__fastcall*)(void*, void*, CMenuMpSessions*, int))
        CSF_AT(void**, menu, 0)[0x60 / 4])(menu, nullptr, self, 0);

    const uint32_t origin = self->field_01e0;
    menu = reinterpret_cast<void*>(self->field_0044);
    void** vtable = CSF_AT(void**, menu, 0);
    int result;

    if (origin == 3)
    {
        result = ((int (__fastcall*)(void*, void*, const char*))
            vtable[0x58 / 4])(menu, nullptr, g_pszMenu_P_MeJuMP);
    }
    else if (origin == 4)
    {
        result = ((int (__fastcall*)(void*, void*, const char*))
            vtable[0x58 / 4])(menu, nullptr, g_pszMenu_P_LbFrie);
    }
    else
    {
        void* entry = ((void* (__fastcall*)(void*, void*, const char*))
            vtable[0x5c / 4])(menu, nullptr, g_pszMenu_P_LbMePr);

        void* control = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, entry, 0)[0x20 / 4])(entry, nullptr);

        result = ((int (__fastcall*)(void*, void*, int))
            CSF_AT(void**, control, 0)[0xa8 / 4])(control, nullptr, 0);
    }

    self->field_0020 = 5;
    return result;
}

HOOK(0x00560210, CMenuMpSessions_NavigateBack, "auto");
