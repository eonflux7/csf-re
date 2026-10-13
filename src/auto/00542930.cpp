#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ResetPending, 0x0050E1E0, (void* self, void*));
CSF_FUNC(int, __fastcall, Object_IsFieldF0NonZero, 0x0050E210, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(void, __fastcall, MpSpawnPoints_Update, 0x00557DD0, (void* self, void* , float deltaTime));
CSF_FUNC(int, __cdecl, GameBddLoader_ShouldPreloadResources, 0x0069D5C0, (int preloadMode));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(void, __fastcall, NetSession_AdvanceCharacterPreviewTimer, 0x0073EAA0, (void* self, void* , float elapsed));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x00899AF0);
CSF_GLOBAL(int, g_MpSessionState, 0x008B0690);

static hook::Tail __fastcall MenuMpCharacterSelect_UpdateMenuPreviewTime(
    void* self, const uint32_t* args)
{
    float deltaTime;
    std::memcpy(&deltaTime, args, sizeof(deltaTime));

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (!CSF_AT(uint8_t, globalState, 0xD0))
    {
        if (GetGlobalState_6A6D70())
        {
            void* preview = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            return hook::tail_jump(
                preview, (*reinterpret_cast<void***>(preview))[0x18 / 4]);
        }
        return hook::tail_ret(0, 4);
    }

    globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (CSF_AT(uint8_t, globalState, 0xD0))
    {
        void* session = reinterpret_cast<void*>(GetGlobalDword_73C020());
        NetSession_AdvanceCharacterPreviewTimer(session, nullptr, deltaTime);
    }

    void* spawnPoints = reinterpret_cast<void*>(GetGlobal8B0688());
    MpSpawnPoints_Update(spawnPoints, nullptr, deltaTime);

    if (GetGlobalState_6A6D70())
    {
        void* preview = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, preview, 0)[0x18 / 4])(
                preview, nullptr, deltaTime);
    }

    if (CSF_AT(float, self, 0x0C) < g_fZero)
    {
        GetGlobal8B0688();
        if (!g_MpSessionState)
            return hook::tail_ret(0, 4);

        if (static_cast<uint8_t>(GameBddLoader_ShouldPreloadResources(0)))
        {
            CSF_AT(float, self, 0x0C) = 0.1f;
            return hook::tail_ret(0, 4);
        }

        globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        if (static_cast<uint8_t>(Object_IsFieldF0NonZero(globalState, nullptr)))
        {
            globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
            GameBddLoader_ResetPending(globalState, nullptr);
        }
    }
    else
    {
        globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        void* menu = CSF_AT(void*, globalState, 0x14);
        void* gamePanel =
            reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, menu, 0)[0x5C / 4])(
                    menu, nullptr, g_pszMenu_P_Juego);

        float elapsed = deltaTime + CSF_AT(float, self, 0x0C);
        CSF_AT(float, self, 0x0C) = elapsed;
        if (!(elapsed >= g_MessageBoxAnimationScale) || !gamePanel)
            return hook::tail_ret(0, 4);

        if (reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                CSF_AT(void**, gamePanel, 0)[0xBC / 4])(
                    gamePanel, nullptr))
            return hook::tail_ret(0, 4);
    }

    // The original obtains this menu object from the table at self's first word.
    void* menu = CSF_AT(void*, CSF_AT(void*, self, 0), 0x18);
    void* gamePanel =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x5C / 4])(
                menu, nullptr, g_pszMenu_P_Juego);
    if (gamePanel)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, gamePanel, 0)[0x54 / 4])(
                gamePanel, nullptr, 1);
    }

    return hook::tail_ret(0, 4);
}

HOOK_TAIL(0x00542930, MenuMpCharacterSelect_UpdateMenuPreviewTime, "menu_mp_character_select");
