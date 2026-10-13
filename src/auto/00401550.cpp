#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_ProcessDisplayFlags_453660, 0x00453660, (CGuiControl* self, void* , S_0041e5f0* a2, float* a3, int* a4, float a5));
CSF_FUNC(int, __fastcall, C_008342e8_GetDrawState, 0x0049CF10, (C_008342e8* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GameConfig_IsSceneObjectRemovable, 0x007558C0, (int objectType));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall C_0082d46c_ApplyScaledEffectToNearbyObjects(void* request)
{
    const int status = CSF_AT(int, request, 0x44);
    if (status)
        return status;

    const float radius = CSF_AT(float, request, 0x54);
    void* source = CSF_AT(void*, request, 0x40);
    int strength;
    if (source && static_cast<uint8_t>(
            GameConfig_IsSceneObjectRemovable(CSF_AT(int, source, 0x50))))
    {
        const int drawState = C_008342e8_GetDrawState(
            CSF_AT(C_008342e8*, request, 0x40), nullptr);
        strength = CSF_AT(int, reinterpret_cast<void*>(drawState), 0x118);
    }
    else
    {
        strength = CSF_AT(int, request, 0x58);
    }

    void* position = static_cast<uint8_t*>(request) + 0x34;

    alignas(8) uint8_t filterBytes[0x34];
    int* filter = reinterpret_cast<int*>(filterBytes);
    filter[0] = 2353;
    filter[1] = -1;
    filter[2] = -1;
    filter[3] = -1;
    filter[4] = 0;
    filter[5] = 0;
    filter[6] = 0;
    filter[7] = 10;
    filter[8] = 0;
    filter[9] = 0;
    filter[10] = 0;
    filter[11] = -1;
    filter[12] = 0;

    alignas(8) uint8_t sphereBytes[0x10];
    float* sphere = reinterpret_cast<float*>(sphereBytes);
    sphere[0] = CSF_AT(float, request, 0x34);
    sphere[1] = CSF_AT(float, request, 0x38);
    sphere[2] = CSF_AT(float, request, 0x3C);
    sphere[3] = radius;

    alignas(8) uint8_t resultsBytes[0x1C];
    CSF_AT(void*, resultsBytes, 0x00) = nullptr;
    CSF_AT(uint32_t, resultsBytes, 0x04) = 0;
    CSF_AT(uint32_t, resultsBytes, 0x08) = 0;
    CSF_AT(int, resultsBytes, 0x0C) = 10;

    CGuiControl_ProcessDisplayFlags_453660(
        static_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        reinterpret_cast<S_0041e5f0*>(resultsBytes), sphere, filter, 0.0f);

    const float scale = CSF_AT(
        float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x144);
    const uint32_t count = CSF_AT(uint32_t, resultsBytes, 0x08);

    for (uint32_t index = 0; index < count; ++index)
    {
        void* entry = CSF_AT(void**, resultsBytes, 0x00)[index];
        void* object = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[0x28 / 4])(entry, nullptr);
        void* effect = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x78 / 4])(object, nullptr);

        if (effect && reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                CSF_AT(void**, effect, 0)[0x28 / 4])(effect, nullptr))
        {
            const float amount = static_cast<float>(strength) * scale;
            reinterpret_cast<void (__fastcall*)(void*, void*, float, void*)>(
                CSF_AT(void**, effect, 0)[0x48 / 4])(
                    effect, nullptr, amount, position);
        }
    }

    if (CSF_AT(void*, filterBytes, 0x10))
    {
        CoreMemory_Free(CSF_AT(void*, filterBytes, 0x10));
        CSF_AT(void*, filterBytes, 0x10) = nullptr;
    }

    void* entries = CSF_AT(void*, resultsBytes, 0x00);
    if (entries)
        return CoreMemory_Free(entries);
    return 0;
}

HOOK(0x00401550, C_0082d46c_ApplyScaledEffectToNearbyObjects, "auto");
