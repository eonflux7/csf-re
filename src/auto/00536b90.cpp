#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_401090, 0x00401090, ());
CSF_FUNC(int, __fastcall, Interface_UpdateGlobalDisplayTime, 0x00401CD0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_00830578_Update, 0x0042CE40, (C_00830578* self, void* , int updateArgument));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(void, __fastcall, CHud_Update_433650, 0x00433650, (CHud* self, void* , float elapsed));
CSF_FUNC(int, __fastcall, Interface_UpdateSceneDisplayTime, 0x0044F390, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(void, __fastcall, S_00452280_Update, 0x00454070, (S_00452280* self, void* , float deltaTime));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(char, __fastcall, C_0083f448_UpdateAnimations, 0x004FCEB0, (C_0083f448* self, void* , int timeBits));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void, __fastcall, S_004fe580_UpdateSceneLoading, 0x005037E0, (S_004fe580* self, void* , float* position, float elapsedTime));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, Get_Global_8B0578, 0x005189D0, ());
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(void, __fastcall, S_0051f950_UpdateEffects, 0x00521A10, (S_0051f950* self, void* , int elapsedBits));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, S_00522010_AccumulateAndPropagateTime, 0x005231D0, (S_00522010* self, void* , int deltaBits));
CSF_FUNC(char, __cdecl, MenuMpCharacterSelect_TickCameraPreview, 0x005425D0, ());
CSF_FUNC(void, __fastcall, Interface_UpdateMenuPreviewTime, 0x00542930, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, Interface_UpdateGlobalFrameTime, 0x006B13C0, (void* self, void* edx, void* a1));

CSF_GLOBAL(float, g_SceneUpdateThreshold, 0x00841F20);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_00830578_UpdateDisplayState(
    C_00830578* self, void* /*edx*/, float elapsedTime)
{
    int elapsedBits;
    std::memcpy(&elapsedBits, &elapsedTime, sizeof(elapsedBits));

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (CSF_AT(uint8_t, state, 0x89) != 0)
    {
        state = reinterpret_cast<void*>(GetGlobalState_50D190());
        void* activeObject = CSF_AT(void*, state, 0x74);
        if (activeObject != nullptr)
        {
            if (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, activeObject, 0)[0x10 / 4])(
                    activeObject, nullptr) != -1 &&
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, activeObject, 0)[0x10 / 4])(
                    activeObject, nullptr) != -2)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, activeObject, 0)[0x8 / 4])(
                    activeObject, nullptr);
            }
        }
    }

    void* menuContext = reinterpret_cast<void*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (menuContext != nullptr)
    {
        alignas(8) uint8_t positionBytes[0xC];
        float* position =
            reinterpret_cast<float*(__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, menuContext, 0)[0xA4 / 4])(
                menuContext, nullptr, reinterpret_cast<float*>(positionBytes));

        auto* loader =
            reinterpret_cast<S_004fe580*>(GetGlobalValue_8B0568());
        S_004fe580_UpdateSceneLoading(loader, nullptr, position, elapsedTime);
    }

    state = reinterpret_cast<void*>(GetGlobalState_50D190());
    float previewTime = CSF_AT(float, state, 0xA0);
    Interface_UpdateMenuPreviewTime(
        CSF_AT(void*, self, 0x14C), nullptr, previewTime);

    void* frameTimeManager = reinterpret_cast<void*>(GetGlobalInt());
    Interface_UpdateGlobalFrameTime(
        frameTimeManager, nullptr,
        reinterpret_cast<void*>(static_cast<uint32_t>(elapsedBits)));

    state = reinterpret_cast<void*>(GetGlobalState_50D190());
    C_00830578_Update(self, nullptr, CSF_AT(int, state, 0xA0));

    if (elapsedTime > g_SceneUpdateThreshold)
    {
        S_00452280_Update(
            reinterpret_cast<S_00452280*>(g_pCurrentScene),
            nullptr, elapsedTime);
    }

    auto* timeManager =
        reinterpret_cast<S_00522010*>(GetGlobalValue_521BB0());
    S_00522010_AccumulateAndPropagateTime(
        timeManager, nullptr, elapsedBits);

    auto* animationManager =
        reinterpret_cast<C_0083f448*>(GetGlobalValue_4FBDE0());
    C_0083f448_UpdateAnimations(animationManager, nullptr, elapsedBits);

    state = reinterpret_cast<void*>(GetGlobalState_50D190());
    float hudElapsed = CSF_AT(float, state, 0x9C);
    auto* hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
    CHud_Update_433650(hud, nullptr, hudElapsed);

    auto* effects = reinterpret_cast<S_0051f950*>(GetGlobalValue_51F740());
    S_0051f950_UpdateEffects(effects, nullptr, elapsedBits);

    void* displayManager = reinterpret_cast<void*>(GetGlobalValue_401090());
    Interface_UpdateGlobalDisplayTime(displayManager, nullptr, elapsedBits);

    Interface_UpdateSceneDisplayTime(g_pCurrentScene, nullptr, elapsedBits);
    Get_Global_8B0578();
    return MenuMpCharacterSelect_TickCameraPreview();
}

HOOK(0x00536B90, C_00830578_UpdateDisplayState, "auto");
