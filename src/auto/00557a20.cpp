#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, AppendFourByteElement_50CDF0, 0x0050CDF0, (void* self, void* , std::uint32_t* source));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalState_ResetPending, 0x0050E1E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Object_IsFieldF0NonZero, 0x0050E210, (void* self, void*));
CSF_FUNC(int, __fastcall, CMenuSoundOptions_ReleaseSoundAndChain, 0x0053CC70, (int* self, void*));
CSF_FUNC(int, __fastcall, CMenuVideoOptions_ValidateAndApplyMode, 0x00549CF0, (CMenuVideoOptions* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(const char*, g_pszMenu_P_VideoI, 0x00899AB4);
CSF_GLOBAL(const char*, g_pszMenu_P_MOGIng, 0x00899AB8);
CSF_GLOBAL(const char*, g_pszMenu_P_SoundI, 0x00899AC8);
CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x00899AF0);

static std::uint32_t __fastcall MpSpawnPoints_ReturnToGameplay(
    void* self, void* /*edx*/)
{
    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* stateMenu = CSF_AT(void*, state, 0x18);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, stateMenu, 0)[0x58 / 4])(stateMenu, nullptr, 0);

    void* menuManager = CSF_AT(void*, self, 0x13BC);
    std::uint32_t gameplayMenu =
        reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menuManager, 0)[0x5C / 4])(
                menuManager, nullptr, g_pszMenu_P_Juego);

    menuManager = CSF_AT(void*, self, 0x13BC);
    int* soundMenu =
        reinterpret_cast<int*(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menuManager, 0)[0x5C / 4])(
                menuManager, nullptr, g_pszMenu_P_SoundI);

    menuManager = CSF_AT(void*, self, 0x13BC);
    if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*, int)>(
            CSF_AT(void**, menuManager, 0)[0x74 / 4])(
                menuManager, nullptr, soundMenu, 0))
    {
        CMenuSoundOptions_ReleaseSoundAndChain(soundMenu, nullptr);
    }

    menuManager = CSF_AT(void*, self, 0x13BC);
    void* inGameOptions =
        reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menuManager, 0)[0x5C / 4])(
                menuManager, nullptr, g_pszMenu_P_MOGIng);

    menuManager = CSF_AT(void*, self, 0x13BC);
    if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*, int)>(
            CSF_AT(void**, menuManager, 0)[0x74 / 4])(
                menuManager, nullptr, inGameOptions, 0))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, inGameOptions, 0)[0x54 / 4])(
                inGameOptions, nullptr);
    }

    menuManager = CSF_AT(void*, self, 0x13BC);
    CMenuVideoOptions* videoMenu =
        reinterpret_cast<CMenuVideoOptions*(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menuManager, 0)[0x5C / 4])(
                menuManager, nullptr, g_pszMenu_P_VideoI);

    menuManager = CSF_AT(void*, self, 0x13BC);
    if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*, int)>(
            CSF_AT(void**, menuManager, 0)[0x74 / 4])(
                menuManager, nullptr, videoMenu, 0))
    {
        CMenuVideoOptions_ValidateAndApplyMode(videoMenu, nullptr);
    }

    S_0050c9f0 menus{};
    std::memset(&menus.field_0004, 0, 12);
    AppendFourByteElement_50CDF0(&menus, nullptr, &gameplayMenu);

    menuManager = CSF_AT(void*, self, 0x13BC);
    reinterpret_cast<void(__fastcall*)(void*, void*, S_0050c9f0*, int)>(
        CSF_AT(void**, menuManager, 0)[0x64 / 4])(
            menuManager, nullptr, &menus, 1);

    state = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (static_cast<std::uint8_t>(Object_IsFieldF0NonZero(state, nullptr)))
    {
        state = reinterpret_cast<void*>(GetGlobalState_50D190());
        GlobalState_ResetPending(state, nullptr);
    }

    if (menus.field_0004)
        return Mem_FreeDebug(reinterpret_cast<void*>(menus.field_0004));
    return menus.field_0004;
}

HOOK(0x00557A20, MpSpawnPoints_ReturnToGameplay, "auto");
