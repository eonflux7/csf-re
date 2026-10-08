// menu_mp_character_select
#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Menu name string used to locate the multiplayer character-select menu.
CSF_GLOBAL(const char*, g_pszMenu_P_CharMP, 0x899AFC);

// Original helper called on the object stored at menu+0x58 (__thiscall, ret 0).
CSF_FUNC(int, __fastcall, Sub_541ED0, 0x541ED0, (int* self, void* edx));

// Looks up the P_CharMP menu through the app's menu manager (app->field_0x18) and
// then releases the two child objects referenced by the returned menu (menu+0x58
// and menu+0x54), clearing both fields. Tail-jump target 0x52F320 is inlined.
static int __fastcall MenuMPCharSelect_GetCharMpMenuAndReleaseChildren(void* self, void* /*edx*/)
{
    // eax = [this] ; ecx = [eax + 0x18]
    uint32_t app = CSF_AT(uint32_t, self, 0x00);
    uint32_t menuManager = CSF_AT(uint32_t, reinterpret_cast<void*>(app), 0x18);

    // mov edx, [ecx] ; push name ; call [edx + 0x5c]  (slot 0x5c / 4 = 23)
    void** managerVtbl = CSF_AT(void**, reinterpret_cast<void*>(menuManager), 0x00);
    int menu = ((int(__fastcall*)(void*, void*, const char*))managerVtbl[23])(
        reinterpret_cast<void*>(menuManager), nullptr, g_pszMenu_P_CharMP);

    int result = menu;

    // mov ecx, [esi + 0x58] ; if (ecx) call 0x541ED0
    int* childA = CSF_AT(int*, reinterpret_cast<void*>(menu), 0x58);
    if (childA)
        result = Sub_541ED0(childA, nullptr);

    // reload [esi + 0x58] ; if non-null: virtual[0](childA, 1) then store 0
    childA = CSF_AT(int*, reinterpret_cast<void*>(menu), 0x58);
    if (childA)
    {
        void** childVtbl = CSF_AT(void**, childA, 0x00);
        result = ((int(__fastcall*)(void*, void*, int))childVtbl[0])(childA, nullptr, 1);
        CSF_AT(uint32_t, reinterpret_cast<void*>(menu), 0x58) = 0;
    }

    // mov ecx, [esi + 0x54] ; if non-null: virtual[0](childB, 1) then store 0
    int* childB = CSF_AT(int*, reinterpret_cast<void*>(menu), 0x54);
    if (childB)
    {
        void** childVtbl = CSF_AT(void**, childB, 0x00);
        result = ((int(__fastcall*)(void*, void*, int))childVtbl[0])(childB, nullptr, 1);
        CSF_AT(uint32_t, reinterpret_cast<void*>(menu), 0x54) = 0;
    }

    return result;
}

HOOK(0x5427A0, MenuMPCharSelect_GetCharMpMenuAndReleaseChildren, "auto");
