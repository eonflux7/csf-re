#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AudioSoundManager_InitializeRequest, 0x0040FE30,
         (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, ApplyGlobalValueRequest, 0x00523940,
         (void* self, void* edx, void* a2, float a3));
CSF_FUNC(int, __fastcall, AudioSoundManager_ResetSelection, 0x00524FB0,
         (void* self, void* edx));
CSF_FUNC(char*, __fastcall, AudioSoundManager_GetSoundSlot, 0x00525270,
         (void* self, void* edx, int a2));
CSF_FUNC(char, __stdcall, AudioSoundManager_IsPersistentSound, 0x00525370,
         (int a1));
CSF_FUNC(char, __stdcall, AudioSoundManager_GetSoundRequestFlag, 0x005253F0,
         (int a1));
CSF_FUNC(int, __stdcall, AudioSoundManager_GetSoundRequestMode, 0x00525470,
         (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall AudioSoundManager_UpdateSceneSound(void* self, void* /*edx*/)
{
    bool inactive = CSF_AT(uint8_t,
                          reinterpret_cast<void*>(GetGlobalState_50D190()),
                          0x89) != 0;
    if (!inactive)
    {
        void* sceneState = CSF_AT(void*, g_pCurrentScene, 0xC0);
        inactive = CSF_AT(uint8_t, sceneState, 0x9) != 0;
        if (!inactive)
        {
            void* context = reinterpret_cast<void*>(
                GetSceneMenuContext(g_pCurrentScene, nullptr));
            inactive = context == nullptr;
            if (!inactive)
                inactive = static_cast<uint8_t>(
                    SceneObj_IsInactive(context, nullptr)) != 0;
        }
    }

    if (inactive)
    {
        char suspended = CSF_AT(char, self, 0x4);
        if (suspended)
            return suspended;

        CSF_AT(char, self, 0x4) = 1;
        return static_cast<char>(AudioSoundManager_ResetSelection(
            static_cast<uint8_t*>(self) + 0x28, nullptr));
    }

    if (CSF_AT(char, self, 0x4))
    {
        CSF_AT(char, self, 0x4) = 0;
        AudioSoundManager_ResetSelection(
            static_cast<uint8_t*>(self) + 0x28, nullptr);
    }

    int currentId = CSF_AT(int, self, 0x28);
    char* currentSlot =
        AudioSoundManager_GetSoundSlot(self, nullptr, currentId);
    int nextId = CSF_AT(int, self, 0x2C);
    char* nextSlot =
        AudioSoundManager_GetSoundSlot(self, nullptr, nextId);
    char currentPersistent = AudioSoundManager_IsPersistentSound(currentId);
    char nextPersistent = AudioSoundManager_IsPersistentSound(nextId);

    if (currentSlot && CSF_AT(void*, currentSlot, 0))
    {
        if ((!nextSlot || !CSF_AT(void*, nextSlot, 0) ||
             currentSlot != nextSlot || !nextPersistent) &&
            (currentPersistent || !nextPersistent))
        {
            void* sound = CSF_AT(void*, currentSlot, 0);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, sound, 0)[0x2C / 4])(sound, nullptr);
        }

        void* sound = CSF_AT(void*, currentSlot, 0);
        bool playing = ((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, sound, 0)[0x3C / 4])(sound, nullptr);
        if (!playing)
            CSF_AT(int, self, 0x28) = -1;
    }

    if (CSF_AT(int, self, 0x28) != -1 || !nextSlot)
        return static_cast<char>(-1);

    CSF_AT(int, self, 0x28) = CSF_AT(int, self, 0x2C);
    if (!nextPersistent)
        CSF_AT(int, self, 0x2C) = -1;

    void* sound = CSF_AT(void*, nextSlot, 0);
    int selectedId = CSF_AT(int, self, 0x28);
    if (sound)
    {
        return static_cast<char>(((int(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, sound, 0)[0x28 / 4])(sound, nullptr, 0, 0));
    }

    void* loaderOwner = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    int resource = SceneLoader_GetResourceById(
        static_cast<uint8_t*>(loaderOwner) + 0x22A4, nullptr, selectedId);
    int mode = static_cast<uint8_t>(
        AudioSoundManager_GetSoundRequestMode(selectedId)) != 0;

    alignas(uint32_t) uint8_t request[0x44]{};
    AudioSoundManager_InitializeRequest(
        request, nullptr, mode + 4, resource, 4, 0, -1);
    CSF_AT(uint8_t, request, 0x36) = 0;
    CSF_AT(char, request, 0x35) =
        AudioSoundManager_GetSoundRequestFlag(selectedId);

    void* manager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    int result = ApplyGlobalValueRequest(manager, nullptr, request, -1.0f);
    CSF_AT(int, nextSlot, 0) = result;
    if (!result)
        CSF_AT(int, self, 0x28) = -1;
    return static_cast<char>(result);
}

HOOK(0x00525C30, AudioSoundManager_UpdateSceneSound, "auto");
