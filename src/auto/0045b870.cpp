#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(bool, __fastcall, SetSceneInteractionEnabled, 0x0044F3A0, (void* self, void* edx, char a2));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_457A80, 0x00457A80, ());
CSF_FUNC(char, __fastcall, ResetGlobalValueObject, 0x00457AF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SetMenuContextState, 0x0049A0E0, (void* self, void* edx, int a4, char a5));
CSF_FUNC(int, __fastcall, RefreshMenuContext, 0x0049A7D0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ApplyCachedProviderState, 0x004F53D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SetCachedProviderMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// GetGlobalValue_4FBDE0 -> GetFirstElement -> GetCachedObjectProvider, chained as the original does.
static void* GetGlobalFirstElementProvider()
{
    const int globalValueHandle = GetGlobalValue_4FBDE0();
    const int firstElementHandle = GetFirstElement(reinterpret_cast<void*>(globalValueHandle), nullptr);
    return GetCachedObjectProvider(reinterpret_cast<void*>(firstElementHandle), nullptr);
}

// Applies or clears the scene menu mode depending on a flag byte reached through the request object.
static char __stdcall ApplySceneMenuModeFromRequest(int /*a1*/, int /*a2*/, uint32_t requestHandle)
{
    // requestHandle -> [0] -> [+4] -> [+1]: the byte that selects the mode.
    const uint32_t requestInner = CSF_AT(uint32_t, reinterpret_cast<void*>(requestHandle), 0x0);
    const uint32_t requestBody = CSF_AT(uint32_t, reinterpret_cast<void*>(requestInner), 0x4);
    const uint8_t modeFlag = CSF_AT(uint8_t, reinterpret_cast<void*>(requestBody), 0x1);

    if (modeFlag != 0)
    {
        SetSceneInteractionEnabled(g_pCurrentScene, nullptr, 0);

        const uint32_t sceneMenuHandle = GetSceneMenuContext(g_pCurrentScene, nullptr);
        void* sceneMenuContext = reinterpret_cast<void*>(sceneMenuHandle);
        char result = static_cast<char>(sceneMenuHandle);

        if (sceneMenuContext != nullptr)
        {
            const int globalValue457A80 = GetGlobalValue_457A80();
            // Code-address annotation: function value, not a vtable.
            [[maybe_unused]] const auto resetObjectVtbl = reinterpret_cast<void**>(CSF_ADDR_FN(0x00457AF0));
            ResetGlobalValueObject(reinterpret_cast<void*>(globalValue457A80), nullptr);

            RefreshMenuContext(sceneMenuContext, nullptr);
            SetMenuContextState(sceneMenuContext, nullptr, 1, 1);

            void* firstProvider = GetGlobalFirstElementProvider();
            SetCachedProviderMode(firstProvider, nullptr, 3);

            void* provider = GetGlobalFirstElementProvider();
            result = ApplyCachedProviderState(provider, nullptr);
        }
        return result;
    }

    void* sceneMenuContext = reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (sceneMenuContext != nullptr)
    {
        SetMenuContextState(sceneMenuContext, nullptr, 0, 1);

        void* provider = GetGlobalFirstElementProvider();
        SetCachedProviderMode(provider, nullptr, 0);
    }
    return static_cast<char>(SetSceneInteractionEnabled(g_pCurrentScene, nullptr, 1));
}

HOOK(0x0045B870, ApplySceneMenuModeFromRequest, "auto");
