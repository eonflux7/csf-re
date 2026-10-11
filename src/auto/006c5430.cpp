#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetInnerByteFlag, 0x006B81E0, (void* self, void*));
CSF_FUNC(double, __fastcall, GfxSkin_GetMaximumDistance, 0x006B8290, (void* self, void* edx, char a2));
CSF_FUNC(double, __fastcall, GfxSkin_GetMinimumDistance, 0x006B82C0, (void* self, void* edx, char a2));
CSF_FUNC(double, __fastcall, GfxSkin_GetFadeWidth, 0x006B82F0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(int, __cdecl, GfxSkin_GetDistanceOverrideOwner, 0x006B8840, (int a1));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* , int value));
CSF_FUNC(void, __fastcall, GfxSkin_ClearSlotValue, 0x006C4630, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, C_0085e7b8_ResolveChainedValue, 0x006C48C0, (void* self, void* edx, int* a2, int a3));
CSF_FUNC(int, __fastcall, GfxSkin_SetSlotValue, 0x006C4B00, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __cdecl, GfxSkin_ComputeDistanceFade, 0x006C5310, (float* a1, float* a2, float* a3, float a4, float a5, float a6, float a7));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(double, __fastcall, GfxSkin_GetDistanceScale, 0x006D2A80, (void* self, void* edx));

static char __cdecl GfxSkin_ApplyDistanceFade(void* sceneObject, void* viewObject)
{
    auto* attachment = reinterpret_cast<C_0085ec18*>(
        SceneLoader_GetObjectAttachment(reinterpret_cast<int>(sceneObject)));

    float fade = 1.0f;
    std::uint32_t* transform =
        RwFrameGetLTM(CSF_AT(std::uint32_t, sceneObject, 0x4));

    // Reserve the original frame's room for the position passed by address.
    alignas(8) std::uint8_t positionBytes[0x1C];
    float* position = reinterpret_cast<float*>(positionBytes);
    position[0] = CSF_AT(float, transform, 0x30);
    position[1] = CSF_AT(float, transform, 0x34);
    position[2] = CSF_AT(float, transform, 0x38);

    void* referenceObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, viewObject, 0)[0x10 / 4])(
                viewObject, nullptr);
    float* referencePosition =
        reinterpret_cast<float* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, referenceObject, 0)[0x10 / 4])(
                referenceObject, nullptr);

    float maximumDistance =
        static_cast<float>(GfxSkin_GetMaximumDistance(attachment, nullptr, 1));
    float minimumDistance =
        static_cast<float>(GfxSkin_GetMinimumDistance(attachment, nullptr, 1));
    float fadeWidth =
        static_cast<float>(GfxSkin_GetFadeWidth(attachment, nullptr, 1));

    void* overrideOwner = reinterpret_cast<void*>(
        GfxSkin_GetDistanceOverrideOwner(reinterpret_cast<int>(sceneObject)));
    void* distanceOverride =
        overrideOwner ? CSF_AT(void*, overrideOwner, 0x10) : nullptr;

    if (distanceOverride && CSF_AT(std::uint8_t, distanceOverride, 0xBC))
    {
        maximumDistance = CSF_AT(float, distanceOverride, 0xC0);
        minimumDistance = CSF_AT(float, distanceOverride, 0xC4);
        fadeWidth = CSF_AT(float, distanceOverride, 0xC8);
    }

    float distanceScale =
        static_cast<float>(GfxSkin_GetDistanceScale(viewObject, nullptr));
    char withinRange = GfxSkin_ComputeDistanceFade(
        &fade, position, referencePosition, distanceScale,
        maximumDistance, minimumDistance, fadeWidth);

    if (withinRange)
    {
        void* attachedObject = CSF_AT(void*, attachment, 0x8);
        auto* skinState =
            reinterpret_cast<C_008670f0* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, attachedObject, 0)[0x1C / 4])(
                    attachedObject, nullptr);

        if (GetInnerByteFlag(attachment, nullptr) == 1)
        {
            void* globalState =
                reinterpret_cast<void*>(GetGlobalValue_6C5760());
            void* regionProvider = CSF_AT(void*, globalState, 0x4);
            int region = GetSceneRegionAt(
                regionProvider, nullptr, fade < g_TransitionOne ? 7 : 6);
            C_008670f0_SetField0C(skinState, nullptr, region);
        }

        if (fade < g_TransitionOne)
        {
            void* provider = GetCachedObjectProvider(skinState, nullptr);
            int valueBits;
            C_0085e7b8_ResolveChainedValue(provider, nullptr, &valueBits, 3);

            float baseValue;
            std::memcpy(&baseValue, &valueBits, sizeof(baseValue));
            fade = baseValue * fade;
            std::memcpy(&valueBits, &fade, sizeof(valueBits));
            GfxSkin_SetSlotValue(skinState, nullptr, 3, valueBits);
            return withinRange;
        }

        GfxSkin_ClearSlotValue(skinState, nullptr, 3);
    }

    return withinRange;
}

HOOK(0x006C5430, GfxSkin_ApplyDistanceFade, "auto");
