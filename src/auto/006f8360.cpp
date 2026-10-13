#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int(__cdecl*)(int atomic, int data);

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, RpMatFXAtomicEnableEffects, 0x00571270, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RpMatFXMaterialSetUVTransformMatrices, 0x00571920, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RpMatFXMaterialSetEffects, 0x00571A80, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __cdecl, RwFrameTranslate, 0x00623830, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwEngineGetCurrentVideoMode, 0x00626AB0, ());
CSF_FUNC(int, __cdecl, Skin_DispatchType4Handler, 0x00626AC0, (int a1));
CSF_FUNC(int, __cdecl, RwCameraSetViewWindow, 0x00629000, (int a1, uint32_t* a2));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));
CSF_FUNC(int, __fastcall, C_008655e8_EndCameraUpdate, 0x006D2BE0, (C_008655e8* self, void*));
CSF_FUNC(bool, __fastcall, GfxCamera_BeginCameraUpdate, 0x006D3010, (void* self, void* edx, int arg0, float a3, float a2, float a5, int a6));
CSF_FUNC(int, __fastcall, C_00861988_SetSecondaryParameters, 0x006FD040, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, C_00861988_ApplyParameters, 0x006FD0E0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, ScreenEffects_PrepareLayer, 0x00709360, (int a1));

CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);
CSF_GLOBAL(float, g_RasterByteScale, 0x00843C14);
CSF_GLOBAL(float, g_MaterialAnimationPhaseScale, 0x00862964);
CSF_GLOBAL(float, g_MaterialAnimationFrameScale, 0x00862968);
CSF_GLOBAL(float, g_MaterialAnimationAmplitudeScale, 0x0086296C);
CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

static int __fastcall C_008629f8_RenderAnimatedMaterialLayer(
    C_008629f8* self, void* /*edx*/)
{
    if (!self->field_0068)
        return 0;

    // Preserve the contiguous scratch area: the temporary vec4 overlaps
    // the translation vector subsequently stored in the same area.
    alignas(8) uint8_t scratch[0x50];
    auto* savedParameters = reinterpret_cast<uint32_t*>(scratch);
    auto* whiteParameters = reinterpret_cast<uint32_t*>(scratch + 0x10);
    auto* translation = reinterpret_cast<uint32_t*>(scratch + 0x14);
    auto* frameTranslation = reinterpret_cast<uint32_t*>(scratch + 0x20);
    auto* viewWindow = reinterpret_cast<uint32_t*>(scratch + 0x2C);

    GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    SceneLoader_NoOp();
    const int previousVideoMode = RwEngineGetCurrentVideoMode();
    Skin_DispatchType4Handler(0);
    ScreenEffects_PrepareLayer(static_cast<int>(self->field_0060));

    void* manager = reinterpret_cast<void*>(
        ResolveClumpManager(GetGlobalInt(), nullptr));
    void* parameters =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, manager, 0)[0x48 / 4])(manager, nullptr);

    // The generated C_00861988 wrappers do not match these runtime slots.
    const uint32_t* currentParameters =
        reinterpret_cast<const uint32_t*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, parameters, 0)[0x38 / 4])(parameters, nullptr);
    savedParameters[0] = currentParameters[0];
    savedParameters[1] = currentParameters[1];
    savedParameters[2] = currentParameters[2];
    savedParameters[3] = currentParameters[3];

    CSF_AT(float, whiteParameters, 0) = 1.0f;
    CSF_AT(float, whiteParameters, 4) = 1.0f;
    CSF_AT(float, whiteParameters, 8) = 1.0f;
    CSF_AT(float, whiteParameters, 12) = 1.0f;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, parameters, 0)[0x34 / 4])(
            parameters, nullptr, whiteParameters);

    CSF_AT(float, whiteParameters, 0) = 1.0f;
    CSF_AT(float, whiteParameters, 4) = 1.0f;
    CSF_AT(float, whiteParameters, 8) = 1.0f;
    CSF_AT(float, whiteParameters, 12) = 1.0f;
    C_00861988_SetSecondaryParameters(parameters, nullptr, whiteParameters);
    C_00861988_ApplyParameters(parameters, nullptr);

    const int atomic = GfxFrames_GetProviderFrameObject(
        static_cast<int>(self->field_0068));
    const float phase = self->field_0070 * g_MaterialAnimationPhaseScale;
    void* geometry = CSF_AT(void*, reinterpret_cast<void*>(atomic), 0x18);
    const int materialCount = CSF_AT(int, geometry, 0x24);
    const float amplitude =
        static_cast<float>(CSF_AT(int,
            reinterpret_cast<void*>(self->field_0054), 0x80)) *
        g_MaterialAnimationAmplitudeScale;
    const float slowPhase = phase * g_ValueScale;
    const float slowerPhase = phase * g_RasterByteScale;
    const float offsetX =
        (std::sin(slowPhase) + std::sin(phase) - std::cos(slowerPhase)) *
        amplitude;
    const float offsetY =
        -((std::sin(slowerPhase) + std::cos(slowPhase) + std::cos(phase)) *
          amplitude);

    CSF_AT(float, translation, 0) = offsetX;
    CSF_AT(float, translation, 4) = offsetY;
    CSF_AT(float, translation, 8) = 0.0f;
    GfxMatrix_CopyTranslationAndUpdate(
        reinterpret_cast<uint8_t*>(self) + 0x78, nullptr, translation);

    for (int i = 0; i < materialCount; ++i)
    {
        int* materials = CSF_AT(int*, geometry, 0x20);
        const uint8_t effectsEnabled = self->field_0074;
        const int material = materials[i];
        if (!effectsEnabled)
        {
            RpMatFXMaterialSetEffects(material, 5);
            // The preceding cdecl arguments remain on the original stack.
            RpMatFXAtomicEnableEffects(atomic, material, 5);
            self->field_0074 = 1;
        }
        RpMatFXMaterialSetUVTransformMatrices(
            material, static_cast<int>(self->field_0078), 0);
    }

    SceneLoader_NoOp();
    const int frame = CSF_AT(int,
        reinterpret_cast<void*>(self->field_0068), 4);
    void* layerSettings = reinterpret_cast<void*>(self->field_0054);

    CSF_AT(float, frameTranslation, 0) = 0.0f;
    CSF_AT(float, frameTranslation, 4) = 0.0f;
    CSF_AT(float, frameTranslation, 8) = 0.0f;
    CSF_AT(float, viewWindow, 0) =
        static_cast<float>(CSF_AT(int, layerSettings, 0x28));
    CSF_AT(float, viewWindow, 4) =
        static_cast<float>(CSF_AT(int, layerSettings, 0x54));
    CSF_AT(float, frameTranslation, 0) =
        offsetX * g_MaterialAnimationFrameScale;
    CSF_AT(float, frameTranslation, 4) =
        offsetY * g_MaterialAnimationFrameScale;
    RwFrameTranslate(frame, reinterpret_cast<int>(frameTranslation), 0);

    void* camera = reinterpret_cast<void*>(self->field_006c);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, camera, 0)[0x60 / 4])(camera, nullptr, 1);

    camera = reinterpret_cast<void*>(self->field_006c);
    void* cameraTransform =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, camera, 0)[0x10 / 4])(camera, nullptr);
    CSF_AT(float, translation, 0) = 0.0f;
    CSF_AT(float, translation, 4) = 0.0f;
    CSF_AT(float, translation, 8) = -100.0f;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, cameraTransform, 0)[0x14 / 4])(
            cameraTransform, nullptr, translation);

    camera = reinterpret_cast<void*>(self->field_006c);
    cameraTransform =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, camera, 0)[0x10 / 4])(camera, nullptr);
    CSF_AT(float, translation, 0) = 0.0f;
    CSF_AT(float, translation, 4) = 0.0f;
    CSF_AT(float, translation, 8) = 1.0f;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, cameraTransform, 0)[0x58 / 4])(
            cameraTransform, nullptr, translation);

    void* rwCamera = GetCachedObjectProvider(
        reinterpret_cast<void*>(self->field_006c), nullptr);
    RwCameraSetViewWindow(reinterpret_cast<int>(rwCamera), viewWindow);

    int alphaBits;
    std::memcpy(&alphaBits, &g_ColorBlack_A, sizeof(alphaBits));
    GfxCamera_BeginCameraUpdate(
        reinterpret_cast<void*>(self->field_006c), nullptr,
        0, g_ColorBlack, g_ColorBlack_G, g_ColorBlack_B, alphaBits);

    using SetRenderStateFn = void(__cdecl*)(int, int);
    CSF_AT(SetRenderStateFn, g_pRwEngineInstance, 0x20)(10, 2);
    CSF_AT(SetRenderStateFn, g_pRwEngineInstance, 0x20)(11, 1);
    RpClumpForAllAtomics(
        static_cast<int>(self->field_0068),
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006F8090)),
        static_cast<int>(self->field_0064));

    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, parameters, 0)[0x34 / 4])(
            parameters, nullptr, savedParameters);
    C_00861988_ApplyParameters(parameters, nullptr);
    Skin_DispatchType4Handler(previousVideoMode);
    return C_008655e8_EndCameraUpdate(
        reinterpret_cast<C_008655e8*>(self->field_006c), nullptr);
}

HOOK(0x006F8360, C_008629f8_RenderAnimatedMaterialLayer, "auto");
