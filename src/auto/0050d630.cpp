#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(char, __fastcall, MenuSystem_PrepareLanguageSelection, 0x00534210,
         (void* self, void* edx));

CSF_GLOBAL(const char*, g_MenuGameplayName, 0x00899AF0);
CSF_GLOBAL(const char*, g_MenuScenesName, 0x00899AE8);
CSF_GLOBAL(const char*, g_MenuLanguageName, 0x00899AC0);
CSF_GLOBAL(const char*, g_MenuIntroName, 0x00899B2C);
CSF_GLOBAL(const char*, g_MenuStartName, 0x00899B8C);

static int __fastcall CMenuManager_SelectInitialScreen(
    CMenuManager* self, void* /*edx*/, char skipIntro)
{
    using PanelMethod = int (__fastcall*)(void*, void*, const char*);
    using CloseMethod = void (__fastcall*)(void*, void*);

    int result = GetGlobalValue_8B0568();
    if (CSF_AT(std::uint8_t,
               reinterpret_cast<void*>(static_cast<std::uintptr_t>(result)),
               0xB4) != 0)
    {
        void* panels = reinterpret_cast<void*>(self->field_0014);
        const char* name = g_MenuGameplayName;
        void** vtable = CSF_AT(void**, panels, 0);
        return reinterpret_cast<PanelMethod>(vtable[0x58 / 4])(
            panels, nullptr, name);
    }

    result = GetGlobalValue_8BC1E0();
    if (CSF_AT(std::uint8_t,
               reinterpret_cast<void*>(static_cast<std::uintptr_t>(result)),
               0xA6C) != 0)
    {
        void* panels = reinterpret_cast<void*>(self->field_0014);
        const char* name = g_MenuScenesName;
        void** vtable = CSF_AT(void**, panels, 0);
        return reinterpret_cast<PanelMethod>(vtable[0x58 / 4])(
            panels, nullptr, name);
    }

    result = GetGlobalValue_8B0568();
    if (CSF_AT(std::uint8_t,
               reinterpret_cast<void*>(static_cast<std::uintptr_t>(result)),
               0xB4) != 0)
        return result;

    result = GetGlobalValue_8BC1E0();
    if (CSF_AT(std::uint8_t,
               reinterpret_cast<void*>(static_cast<std::uintptr_t>(result)),
               0xA6C) != 0)
        return result;

    void* panels = reinterpret_cast<void*>(self->field_0014);
    const char* name = g_MenuLanguageName;
    void** vtable = CSF_AT(void**, panels, 0);
    result = reinterpret_cast<PanelMethod>(vtable[0x5C / 4])(
        panels, nullptr, name);
    if (result == 0)
        return result;

    const char needsSelection = MenuSystem_PrepareLanguageSelection(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(result)), nullptr);

    name = g_MenuLanguageName;
    panels = reinterpret_cast<void*>(self->field_0014);
    vtable = CSF_AT(void**, panels, 0);
    if (needsSelection != 0)
        return reinterpret_cast<PanelMethod>(vtable[0x58 / 4])(
            panels, nullptr, name);

    result = reinterpret_cast<PanelMethod>(vtable[0x5C / 4])(
        panels, nullptr, name);
    if (result != 0)
    {
        void* languagePanel =
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(result));
        void** languageVtable = CSF_AT(void**, languagePanel, 0);
        reinterpret_cast<CloseMethod>(languageVtable[0x3C / 4])(
            languagePanel, nullptr);
    }

    name = g_MenuStartName;
    if (skipIntro == 0)
        name = g_MenuIntroName;

    panels = reinterpret_cast<void*>(self->field_0014);
    vtable = CSF_AT(void**, panels, 0);
    return reinterpret_cast<PanelMethod>(vtable[0x58 / 4])(
        panels, nullptr, name);
}

HOOK(0x0050D630, CMenuManager_SelectInitialScreen, "auto");
