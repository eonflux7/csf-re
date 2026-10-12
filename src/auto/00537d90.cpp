#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, AppendFourByteElement_50CDF0, 0x0050CDF0, (void* self, void* , std::uint32_t* source));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ResetPending, 0x0050E1E0, (void* self, void*));
CSF_FUNC(int, __fastcall, Object_IsFieldF0NonZero, 0x0050E210, (void* self, void*));
CSF_FUNC(int, __fastcall, MenuMPCharSelect_GetCharMpMenuAndReleaseChildren, 0x005427A0, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x00899AF0);
CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);
CSF_GLOBAL(const char*, g_pszMenu_P_MsgBox, 0x00899B28);

static std::uint32_t __fastcall Interface_ActivateGameplayMenus(
    void* self, void* /*edx*/)
{
    MenuMPCharSelect_GetCharMpMenuAndReleaseChildren(
        CSF_AT(void*, self, 0x14C), nullptr);

    void* menu = CSF_AT(void*, self, 0x18);
    std::uint32_t gamePanel =
        reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x5C / 4])(
                menu, nullptr, g_pszMenu_P_Juego);

    menu = CSF_AT(void*, self, 0x18);
    std::uint32_t messagePanel =
        reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x5C / 4])(
                menu, nullptr, g_pszMenu_P_MsgBox);

    menu = CSF_AT(void*, self, 0x18);
    std::uint32_t tacticsPanel =
        reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x5C / 4])(
                menu, nullptr, g_pszMenu_P_Tactic);

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (static_cast<std::uint8_t>(Object_IsFieldF0NonZero(state, nullptr)))
    {
        state = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_ResetPending(state, nullptr);
    }

    alignas(8) std::uint8_t panelListBytes[0x10]{};
    S_0050c9f0& panelList =
        *reinterpret_cast<S_0050c9f0*>(panelListBytes);
    std::memset(&panelList.field_0004, 0, 12);

    AppendFourByteElement_50CDF0(&panelList, nullptr, &gamePanel);
    AppendFourByteElement_50CDF0(&panelList, nullptr, &messagePanel);
    AppendFourByteElement_50CDF0(&panelList, nullptr, &tacticsPanel);

    menu = CSF_AT(void*, self, 0x18);
    reinterpret_cast<void (__fastcall*)(void*, void*, S_0050c9f0*, int)>(
        CSF_AT(void**, menu, 0)[0x64 / 4])(
            menu, nullptr, &panelList, 1);

    const std::uint32_t allocation = panelList.field_0004;
    if (allocation)
        return static_cast<std::uint32_t>(
            Mem_FreeDebug(reinterpret_cast<void*>(allocation)));
    return allocation;
}

HOOK(0x00537D90, Interface_ActivateGameplayMenus, "auto");
