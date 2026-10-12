#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszMenu_P_Help, 0x00899AA4);
CSF_GLOBAL(const char*, g_pszMenu_P_MeJueg, 0x00899AF4);
CSF_GLOBAL(const char*, g_pszMenu_P_MeJuMP, 0x00899AF8);
CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);

static std::uint8_t __fastcall Interface_ActivatePendingMenu(
    void* self, void* /*edx*/)
{
    std::uint8_t result = CSF_AT(std::uint8_t, self, 0x183);
    if (!result)
        return result;

    void* menu = CSF_AT(void*, self, 0x18);
    void** vtable = CSF_AT(void**, menu, 0);
    void* item =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*, int)>(
            vtable[0x5C / 4])(menu, nullptr, g_pszMenu_P_MeJueg, 1);

    result =
        reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*, void*)>(
            vtable[0x74 / 4])(
                CSF_AT(void*, self, 0x18), nullptr, item);

    if (!result)
    {
        menu = CSF_AT(void*, self, 0x18);
        vtable = CSF_AT(void**, menu, 0);
        item =
            reinterpret_cast<void* (__fastcall*)(void*, void*, const char*, int)>(
                vtable[0x5C / 4])(menu, nullptr, g_pszMenu_P_Help, 1);

        result =
            reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*, void*)>(
                vtable[0x74 / 4])(
                    CSF_AT(void*, self, 0x18), nullptr, item);

        if (!result)
        {
            menu = CSF_AT(void*, self, 0x18);
            vtable = CSF_AT(void**, menu, 0);
            item =
                reinterpret_cast<void* (__fastcall*)(void*, void*, const char*, int)>(
                    vtable[0x5C / 4])(menu, nullptr, g_pszMenu_P_MeJuMP, 1);

            result =
                reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*, void*)>(
                    vtable[0x74 / 4])(
                        CSF_AT(void*, self, 0x18), nullptr, item);

            if (!result)
            {
                menu = CSF_AT(void*, self, 0x18);
                vtable = CSF_AT(void**, menu, 0);
                item =
                    reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
                        vtable[0x5C / 4])(menu, nullptr, g_pszMenu_P_Tactic);

                result = static_cast<std::uint8_t>(
                    reinterpret_cast<std::uintptr_t>(item));

                if (item)
                {
                    CSF_AT(std::uint32_t, self, 0x154) = 0;
                    CSF_AT(std::uint8_t, self, 0x158) = 1;
                    CSF_AT(void*, item, 0x14) =
                        static_cast<std::uint8_t*>(self) + 0x154;

                    menu = CSF_AT(void*, self, 0x18);
                    vtable = CSF_AT(void**, menu, 0);
                    result =
                        reinterpret_cast<std::uint8_t (__fastcall*)(
                            void*, void*, const char*)>(
                                vtable[0x58 / 4])(
                                    menu, nullptr, g_pszMenu_P_Tactic);
                }
            }
        }
    }

    CSF_AT(std::uint8_t, self, 0x183) = 0;
    return result;
}

HOOK(0x00535F60, Interface_ActivatePendingMenu, "auto");
