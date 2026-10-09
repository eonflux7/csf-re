#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(uint32_t, __fastcall, Hud_UpdateResourceWidget, 0x004344A0, (void* self, void*));
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(char, __fastcall, ApplySceneResourceName, 0x004DED90, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, CSceneLoader_RefreshSceneResources, 0x00501F90,
         (CSceneLoaderBase* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CSceneLoader_UpdateSceneResources(
    CSceneLoaderBase* self, void* /*edx*/, int /*unused*/)
{
    GetValue8B0554();
    auto* settings = reinterpret_cast<uint8_t*>(GetValue8B0554());
    auto* sceneSettings = settings + 0x54;

    if (!CSF_AT(uint8_t, settings, 0x1F2C) &&
        !CSF_AT(uint8_t, sceneSettings, 0x1ED9) &&
        !CSF_AT(uint8_t, sceneSettings, 0x1EDA) &&
        !CSF_AT(uint8_t, sceneSettings, 0x1EE0))
    {
        const char* resourceName =
            CSF_AT(uint32_t, sceneSettings, 0x1A40) < 0x10u
                ? reinterpret_cast<const char*>(sceneSettings + 0x1A2C)
                : CSF_AT(const char*, sceneSettings, 0x1A2C);

        if (*resourceName &&
            CGuiControl_GetField012C(
                reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr))
        {
            resourceName =
                CSF_AT(uint32_t, sceneSettings, 0x1A40) < 0x10u
                    ? reinterpret_cast<const char*>(sceneSettings + 0x1A2C)
                    : CSF_AT(const char*, sceneSettings, 0x1A2C);

            auto* resource =
                reinterpret_cast<void*>(CGuiControl_GetField012C(
                    reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr));
            ApplySceneResourceName(
                resource, nullptr, reinterpret_cast<int>(resourceName));
        }
    }

    CSceneLoader_RefreshSceneResources(self, nullptr);
    auto* hud = reinterpret_cast<void*>(GetGlobal8afc78());
    return static_cast<int>(Hud_UpdateResourceWidget(hud, nullptr));
}

HOOK(0x005021A0, CSceneLoader_UpdateSceneResources, "auto");
