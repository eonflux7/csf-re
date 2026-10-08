#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_MenuMainPage, 0x00899AE4);
CSF_GLOBAL(const char*, g_MenuGamePage, 0x00899AF0);
CSF_GLOBAL(const char*, g_MenuTacticalPage, 0x00899B14);
CSF_GLOBAL(const char*, g_MenuMessageBoxPage, 0x00899B28);
CSF_GLOBAL(const char*, g_MenuIntroPage, 0x00899B2C);
CSF_GLOBAL(const char*, g_MenuStartPage, 0x00899B8C);

CSF_FUNC(int, __fastcall, MenuManager_IsReadyAndAboveThreshold,
         0x0050C880, (float* self, void* edx));
CSF_FUNC(void*, __cdecl, MenuManager_GetState, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, MenuManager_GetGameSettings, 0x0075FDF0, ());
CSF_FUNC(void*, __cdecl, MenuManager_GetIntroSettings, 0x00756A10, ());
CSF_FUNC(void*, __cdecl, MenuManager_GetMenuContext, 0x00506BD0, ());
CSF_FUNC(void*, __cdecl, MenuManager_GetGameplayController, 0x0042D6D0, ());
CSF_FUNC(int, __cdecl, MenuManager_FreeBufferAndClear,
         0x0050C9C0, (void* buffer));
CSF_FUNC(unsigned int, __fastcall, MenuManager_AppendPage,
         0x0050CDF0, (void* buffer, void* edx, void** page));
CSF_FUNC(int, __fastcall, MenuManager_IsStateFieldF0NonZero,
         0x0050E210, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuManager_GetInnerByteAt50,
         0x0050E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuManager_ProcessActiveState,
         0x0050E1E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuManager_SetInnerState,
         0x0050E930, (void* self, void* edx, char enabled));
CSF_FUNC(bool, __fastcall, MenuManager_SetMenuState,
         0x0050DC80, (void* self, void* edx, char enabled));
CSF_FUNC(int, __fastcall, MenuManager_SetGameplayControllerState,
         0x0042DF80, (void* self, void* edx, int state));
CSF_FUNC(std::uintptr_t, __fastcall, MenuManager_ProcessMenuContext,
         0x0050BB70, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuManager_SetIntroPageData,
         0x006F2850, (void* self, void* edx, void* data));

static std::uintptr_t __fastcall MenuManager_TransitionToGameOrIntro(
    float* self, void* /*edx*/, int /*unused*/)
{
    std::uintptr_t result = static_cast<std::uint32_t>(
        MenuManager_IsReadyAndAboveThreshold(self, nullptr));
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    void* state = MenuManager_GetState();
    result = reinterpret_cast<std::uintptr_t>(state);
    if (CSF_AT(std::uint8_t, state, 0xD0) != 0)
        return result;

    void* manager = CSF_AT(void*, MenuManager_GetState(), 0x14);
    void* settings = MenuManager_GetGameSettings();
    result = reinterpret_cast<std::uintptr_t>(settings);

    if (CSF_AT(std::uint8_t, settings, 0xCD) == 0)
    {
        ((std::uintptr_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, manager, 0)[0x5C / 4])(
                manager, nullptr, g_MenuStartPage);

        result = ((std::uintptr_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, manager, 0)[0x5C / 4])(
                manager, nullptr, g_MenuMainPage);
        if (result == 0)
            return result;

        result = ((std::uintptr_t(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, manager, 0)[0x74 / 4])(
                manager, nullptr, reinterpret_cast<void*>(result), 0);
        if (static_cast<std::uint8_t>(result) == 0)
            return result;
    }

    void* gamePage = reinterpret_cast<void*>(
        ((std::uintptr_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, manager, 0)[0x5C / 4])(
                manager, nullptr, g_MenuGamePage));
    void* gamePageEntry = gamePage;

    void* messagePageEntry = reinterpret_cast<void*>(
        ((std::uintptr_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, manager, 0)[0x5C / 4])(
                manager, nullptr, g_MenuMessageBoxPage));

    ((std::uintptr_t(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, manager, 0)[0x5C / 4])(
            manager, nullptr, g_MenuTacticalPage);

    const std::uintptr_t gamePageStatus =
        ((std::uintptr_t(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, manager, 0)[0x74 / 4])(
                manager, nullptr, gamePage, 0);

    if (static_cast<std::uint8_t>(gamePageStatus) != 0)
    {
        settings = MenuManager_GetGameSettings();
        result = reinterpret_cast<std::uintptr_t>(settings);
        if (CSF_AT(std::uint8_t, settings, 0xCD) == 0)
            return result;

        if (static_cast<std::uint8_t>(
                MenuManager_IsStateFieldF0NonZero(
                    MenuManager_GetState(), nullptr)) != 0)
        {
            MenuManager_ProcessActiveState(MenuManager_GetState(), nullptr);
        }

        if (MenuManager_GetInnerByteAt50(MenuManager_GetState(), nullptr) != 0)
            MenuManager_SetInnerState(MenuManager_GetState(), nullptr, 0);

        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, gamePage, 0)[0x54 / 4])(
                gamePage, nullptr, 1);

        if (CSF_AT(std::uint8_t, MenuManager_GetState(), 0x89) != 0)
        {
            void* pageBuffer = reinterpret_cast<std::uint8_t*>(self) + 8;
            MenuManager_FreeBufferAndClear(pageBuffer);
            MenuManager_AppendPage(pageBuffer, nullptr, &gamePageEntry);
            ((void(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, manager, 0)[0x64 / 4])(
                    manager, nullptr, pageBuffer, 0);
            MenuManager_SetMenuState(MenuManager_GetState(), nullptr, 1);
        }

        void* controller = CSF_AT(void*, MenuManager_GetState(), 0x10);
        if (controller != nullptr)
        {
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, controller, 0)[0x24 / 4])(
                    controller, nullptr);
        }

        MenuManager_SetGameplayControllerState(
            MenuManager_GetGameplayController(), nullptr, 0);
    }
    else
    {
        if (static_cast<std::uint8_t>(
                MenuManager_IsStateFieldF0NonZero(
                    MenuManager_GetState(), nullptr)) != 0)
        {
            MenuManager_ProcessActiveState(MenuManager_GetState(), nullptr);
        }

        void* pageBuffer = reinterpret_cast<std::uint8_t*>(self) + 8;
        MenuManager_FreeBufferAndClear(pageBuffer);
        MenuManager_AppendPage(pageBuffer, nullptr, &messagePageEntry);
        ((void(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, manager, 0)[0x64 / 4])(
                manager, nullptr, pageBuffer, 0);

        void* introData = reinterpret_cast<std::uint8_t*>(self) + 0x18;
        CSF_AT(std::uint32_t, introData, 0x00) = 0;
        CSF_AT(std::uint32_t, introData, 0x04) = 0;
        CSF_AT(std::uint8_t, introData, 0x08) = 0;
        CSF_AT(std::uint8_t, introData, 0x09) = 0;
        CSF_AT(std::uint32_t, introData, 0x0C) = 0;
        CSF_AT(std::uint8_t, introData, 0x10) = 1;
        CSF_AT(std::uint8_t, introData, 0x11) = 0;
        CSF_AT(std::uint8_t, introData, 0x12) = 0;

        void* introSettings = MenuManager_GetIntroSettings();
        const std::uint32_t capacity =
            CSF_AT(std::uint32_t, introSettings, 0x21D0);
        void* textStorage =
            reinterpret_cast<std::uint8_t*>(introSettings) + 0x21BC;
        void* text = capacity < 0x10
            ? textStorage
            : CSF_AT(void*, textStorage, 0);
        CSF_AT(void*, introData, 0) = text;
        CSF_AT(std::uint32_t, self, 0x1C) = 1;

        void* introPage = reinterpret_cast<void*>(
            ((std::uintptr_t(__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, manager, 0)[0x5C / 4])(
                    manager, nullptr, g_MenuIntroPage));
        MenuManager_SetIntroPageData(introPage, nullptr, introData);

        ((void(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, manager, 0)[0x58 / 4])(
                manager, nullptr, g_MenuIntroPage);
    }

    settings = MenuManager_GetGameSettings();
    result = reinterpret_cast<std::uintptr_t>(settings);
    if (CSF_AT(std::uint8_t, settings, 0xCD) != 0)
    {
        void* context = MenuManager_GetMenuContext();
        result = reinterpret_cast<std::uintptr_t>(context);
        void* innerContext = CSF_AT(void*, context, 0x18);
        if (innerContext != nullptr)
            return MenuManager_ProcessMenuContext(innerContext, nullptr);
    }
    return result;
}

HOOK(0x0050CE60, MenuManager_TransitionToGameOrIntro, "auto");
