#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneObject, 0x004448B0, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneResources, 0x00469840, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetServiceInterface, 0x00726C40, (void* self, void* edx));

static int __fastcall CSceneLoaderBase_PrepareSceneTransition(
    void* self, void* /*edx*/)
{
    SceneLoader_PrepareSceneResources(
        CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr);
    SceneLoader_PrepareSceneObject(
        CSF_AT(void*, g_pCurrentScene, 0xC0), nullptr);

    if (CSF_AT(uint8_t, self, 0xB6))
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        if (CSF_AT(void*, state, 0x1C))
        {
            state = reinterpret_cast<void*>(GetGlobalState_50D190());
            void* controller = CSF_AT(void*, state, 0x1C);
            ((void(__fastcall*)(void*, void*, float))
                CSF_AT(void**, controller, 0)[0x44 / 4])(
                    controller, nullptr, 10.0f);
        }
    }

    if (!CompareStringSubstring(
            reinterpret_cast<uint8_t*>(self) + 0x98, nullptr,
            0, CSF_AT(unsigned int, self, 0xAC),
            reinterpret_cast<const char*>(&g_byte_82E65C), 0))
    {
        void* collection = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        void* firstElement =
            reinterpret_cast<void*>(GetFirstElement(collection, nullptr));
        void* provider = GetCachedObjectProvider(firstElement, nullptr);
        uint32_t menuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
        ObjectProvider_SetUpdateMode(provider, nullptr, menuContext ? 0 : 4);
    }

    void* transitionState = reinterpret_cast<void*>(GetValue8B0554());
    void* globalService = reinterpret_cast<void*>(GetGlobalInt());
    void* service = reinterpret_cast<void*>(
        SceneLoader_GetServiceInterface(globalService, nullptr));
    void* receiver = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, service, 0)[0x48 / 4])(service, nullptr));

    return ((int(__fastcall*)(void*, void*, uint32_t))
        CSF_AT(void**, receiver, 0)[0x1C / 4])(
            receiver, nullptr, CSF_AT(uint32_t, transitionState, 0x95C));
}

HOOK(0x00500800, CSceneLoaderBase_PrepareSceneTransition, "auto");
