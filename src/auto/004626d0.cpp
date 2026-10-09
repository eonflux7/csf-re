#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(bool, __fastcall, Scene_SetSelectionEnabled, 0x0044F3A0, (void* self, void* edx, char a2));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ObjectProvider_IsActive, 0x00471540, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ObjectProvider_Commit, 0x004F53D0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ObjectProvider_SetPendingRequest, 0x004F5EF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall SceneMenu_ResetSelectionAndRefresh(int /*a1*/, int /*_18*/, int /*a3*/)
{
    int result;

    Scene_SetSelectionEnabled(g_pCurrentScene, nullptr, 0);
    result = static_cast<int>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (result)
    {
        int globalValue = GetGlobalValue_4FBDE0();
        int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
        void* cachedProvider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
        result = ObjectProvider_IsActive(cachedProvider, nullptr);
        if (!result)
        {
            int requestArgs[3];
            requestArgs[1] = -1;
            requestArgs[2] = -1;
            requestArgs[0] = reinterpret_cast<int>(CSF_VTBL(0x0082D448));
            ObjectProvider_SetPendingRequest(cachedProvider, nullptr, reinterpret_cast<int>(requestArgs));
            ObjectProvider_SetUpdateMode(cachedProvider, nullptr, 3);
            return static_cast<int>(ObjectProvider_Commit(cachedProvider, nullptr));
        }
    }
    return result;
}

HOOK(0x004626D0, SceneMenu_ResetSelectionAndRefresh, "auto");
