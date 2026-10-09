#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, SceneLoader_CleanupCurrentScene, 0x0044FB50, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetMissionState, 0x004E92B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetFirstManager, 0x0051F7B0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetThirdManager, 0x00523640, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetSecondManager, 0x005258F0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CMenuMissionInfo_ReleaseStoredObject, 0x0053D4F0, (void* self, void*));
CSF_FUNC(int, __cdecl, RpWorldRemoveCamera, 0x0058DDC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwCameraGetWorld, 0x0058DDF0, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetShutdownInterface, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CSceneLoader_ClearScene(
    C_0082d410* self, void* /*edx*/, char detachCameraOnly)
{
    AssignStringRange_429B60(
        reinterpret_cast<uint8_t*>(self) + 0x60,
        nullptr,
        reinterpret_cast<char*>(&g_byte_82E65C),
        0);

    if (detachCameraOnly)
    {
        void* manager = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        void* firstElement = reinterpret_cast<void*>(GetFirstElement(manager, nullptr));
        void* provider = GetCachedObjectProvider(firstElement, nullptr);
        void* referencedObject = GetReferencedObject(provider, nullptr);
        void* camera = GetCachedObjectProvider(referencedObject, nullptr);
        int cameraValue = reinterpret_cast<int>(camera);
        int world = RwCameraGetWorld(cameraValue);
        if (world)
            return RpWorldRemoveCamera(world, cameraValue);
        return world;
    }

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* shutdownInterface = reinterpret_cast<void*>(
        SceneLoader_GetShutdownInterface(globalObject, nullptr));
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, shutdownInterface, 0)[0x28 / 4])(
            shutdownInterface, nullptr);

    SceneLoader_ResetFirstManager(
        reinterpret_cast<void*>(GetGlobalValue_51F740()), nullptr);
    SceneLoader_ResetSecondManager(
        reinterpret_cast<void*>(Get_008b0678()), nullptr);
    SceneLoader_ResetThirdManager(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    GetValue8B0554();
    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* menuManager = CSF_AT(void*, globalState, 0x14);
    int missionInfo = reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, menuManager, 0)[0x5C / 4])(
            menuManager, nullptr, g_pszMenu_P_Tactic);
    if (missionInfo)
        CMenuMissionInfo_ReleaseStoredObject(
            reinterpret_cast<void*>(missionInfo), nullptr);

    SceneLoader_CleanupCurrentScene(g_pCurrentScene, nullptr);
    return SceneLoader_ResetMissionState(
        reinterpret_cast<void*>(GetValue8B0554()), nullptr, 0);
}

HOOK(0x00501AB0, CSceneLoader_ClearScene, "auto");
