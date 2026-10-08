#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, MenuAccounts_TestMenuEvent, 0x007A8ED0,
         (int event));

CSF_GLOBAL(const char*, g_pszMenu_P_LbMePr, 0x00899B40);

static int __fastcall MenuAccounts_HandleEventAndResetControl(
    void* self, void* /*edx*/, int event, int /*unused*/)
{
    int result = MenuAccounts_TestMenuEvent(event);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    void* menu = CSF_AT(void*, self, 0x10);
    void** menuVtable = CSF_AT(void**, menu, 0x00);
    void* owner = static_cast<std::uint8_t*>(self) - 4;

    reinterpret_cast<void (__fastcall*)(void*, void*, void*, int)>(
        menuVtable[0x60 / 4])(menu, nullptr, owner, 1);

    menu = CSF_AT(void*, owner, 0x14);
    const char* controlName = g_pszMenu_P_LbMePr;
    menuVtable = CSF_AT(void**, menu, 0x00);

    void* control =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            menuVtable[0x5C / 4])(menu, nullptr, controlName);

    void* controlObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0x00)[0x20 / 4])(
                control, nullptr);

    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, controlObject, 0x00)[0xA8 / 4])(
            controlObject, nullptr, 0);
}

HOOK(0x00565E90, MenuAccounts_HandleEventAndResetControl, "auto");
