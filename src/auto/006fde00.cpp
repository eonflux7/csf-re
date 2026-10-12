#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, RpMatFXAtomicEnableEffects, 0x00571270, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RpMatFXMaterialSetUVTransformMatrices, 0x00571920, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RpMatFXMaterialSetEffects, 0x00571A80, (int a1, int a2));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));

static void __fastcall S_006fde00_UpdateMaterialUVTransforms(
    S_006fde00* self, void* /*edx*/)
{
    const int atomic = GfxFrames_GetProviderFrameObject(
        static_cast<int>(self->field_0008));
    void* const geometry = CSF_AT(void*, reinterpret_cast<void*>(atomic), 0x18);
    const int materialCount = CSF_AT(int, geometry, 0x24);

    alignas(8) uint8_t translationBytes[0x18];
    auto* translation = reinterpret_cast<uint32_t*>(translationBytes);
    translation[0] = 0;
    translation[1] = self->field_00c4;
    translation[2] = 0;

    GfxMatrix_CopyTranslationAndUpdate(
        reinterpret_cast<uint8_t*>(self) + 0xB8, nullptr, translation);

    for (int index = 0; index < materialCount; ++index)
    {
        const bool effectsEnabled = self->field_00c0 != 0;
        const int material = CSF_AT(int*, geometry, 0x20)[index];

        if (!effectsEnabled)
        {
            RpMatFXMaterialSetEffects(material, 5);
            // The previous cdecl call's arguments remain below the atomic.
            RpMatFXAtomicEnableEffects(atomic, material, 5);
            self->field_00c0 = 1;
        }

        RpMatFXMaterialSetUVTransformMatrices(
            material, static_cast<int>(self->field_00b8), 0);
    }

    SceneLoader_NoOp();
}

HOOK(0x006FDE00, S_006fde00_UpdateMaterialUVTransforms, "auto");
