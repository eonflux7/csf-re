#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, FxParticleManager_GetParticleController, 0x006CA810, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

CSF_GLOBAL(std::uint32_t, g_IconResources, 0x008B043C);

static int __stdcall HudAlertIcons_SetIcon(
    int iconStateAddress, int iconIndex,
    int controllerSetting1, int controllerSetting2, int controllerSetting3)
{
    void* state = reinterpret_cast<void*>(iconStateAddress);

    if (iconIndex == -1)
    {
        CSF_AT(int, state, 0x10) = -1;
        return iconStateAddress;
    }

    void* effect = CSF_AT(void*, state, 0x14);
    if (effect)
    {
        if (iconIndex != CSF_AT(int, state, 0x10))
        {
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, effect, 0)[0x24 / 4])(effect, nullptr);

            const std::uint32_t resource = (&g_IconResources)[iconIndex];
            effect = CSF_AT(void*, state, 0x14);
            ((void(__fastcall*)(void*, void*, std::uint32_t))
                CSF_AT(void**, effect, 0)[0x18 / 4])(effect, nullptr, resource + 8);

            effect = CSF_AT(void*, state, 0x14);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, effect, 0)[0x20 / 4])(effect, nullptr);

            effect = CSF_AT(void*, state, 0x14);
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, effect, 0)[0x28 / 4])(effect, nullptr, 0);
        }
    }
    else
    {
        const std::uint32_t resource = (&g_IconResources)[iconIndex];
        void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
        void* registry = reinterpret_cast<void*>(
            SceneLoader_GetObjectRegistry(sceneLoader, nullptr));

        effect = ((void*(__fastcall*)(void*, void*, std::uint32_t))
            CSF_AT(void**, registry, 0)[0x14 / 4])(registry, nullptr, resource + 8);

        CSF_AT(void*, state, 0x14) = effect;
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, effect, 0)[0x28 / 4])(effect, nullptr, 0);
    }

    const int effectAddress = CSF_AT(int, state, 0x14);
    CSF_AT(int, state, 0x10) = iconIndex;
    void* controller = reinterpret_cast<void*>(
        FxParticleManager_GetParticleController(effectAddress));

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, controller, 0)[0x1C / 4])(
            controller, nullptr, controllerSetting1);

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, controller, 0)[0x08 / 4])(
            controller, nullptr, controllerSetting2);

    return ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, controller, 0)[0x0C / 4])(
            controller, nullptr, controllerSetting3);
}

HOOK(0x0049FE70, HudAlertIcons_SetIcon, "auto");
