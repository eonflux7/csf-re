#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpMatFXMaterialSetUVTransformMatrices, 0x00571920, (int a1, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GetBit2OfField4, 0x006B8170, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(char, __cdecl, GfxEnvironment_SetSectorObjectFlag4, 0x006C0160, (int a1, char a2));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));
CSF_FUNC(int*, __fastcall, C_0085e7b8_ResolveChainedValue, 0x006C48C0, (C_0085e7b8* self, void* , int* a2, int a3));
CSF_FUNC(int, __cdecl, SkinGeometry_SetMaterialAlpha, 0x006C4E60, (int atomicPtr, int alpha));

CSF_FUNC(int, __fastcall, GfxObjectInstances_ShouldSkipRendering, 0x006D5F60, (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, S_006e6100_GetRenderOpacity, 0x006E5C70, (int a1));
CSF_FUNC(void, __fastcall, S_006e6100_PrepareRendering, 0x006E6100, (void* self, void* edx));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

static void __fastcall S_006e6100_Render(S_006e6100* self, void* /*edx*/)
{
    if (!self->field_0019)
        return;

    void* visibilityObject = reinterpret_cast<void*>(self->field_0010);
    if (!((bool (__fastcall*)(void*, void*))
          CSF_AT(void**, visibilityObject, 0)[0x40 / 4])(
              visibilityObject, nullptr))
        return;

    const uint8_t variant = self->field_0018;
    if (variant && !CSF_AT(uint32_t, self, 0x1C + 4 * variant))
        return;

    // The packet labels this function address as a vtable reference.
    (void)CSF_VTBL(0x006E6100);
    S_006e6100_PrepareRendering(self, nullptr);

    const int globalObject = GetGlobalInt();
    const int sceneController = SceneLoader_GetSceneController(
        reinterpret_cast<void*>(globalObject), nullptr);

    const int objectType = ((int (__fastcall*)(S_006e6100*, void*))
        reinterpret_cast<void**>(self->field_0000)[0x10 / 4])(
            self, nullptr);

    if (objectType == 250 &&
        static_cast<uint8_t>(GfxObjectInstances_ShouldSkipRendering(
            CSF_AT(void*, reinterpret_cast<void*>(sceneController), 0xBC),
            nullptr, reinterpret_cast<int>(self))))
        return;

    void* propertiesObject = reinterpret_cast<void*>(self->field_0040);
    C_0085e7b8* properties =
        ((C_0085e7b8* (__fastcall*)(void*, void*))
         CSF_AT(void**, propertiesObject, 0)[0x1C / 4])(
             propertiesObject, nullptr);

    int* entry = reinterpret_cast<int*>(
        C_0085e7b8_FindEntryPayloadByKey(properties, nullptr, 7));
    int* uvTransform = entry ? reinterpret_cast<int*>(*entry) : nullptr;

    if (uvTransform)
        RpMatFXMaterialSetUVTransformMatrices(
            uvTransform[0], uvTransform[1], 0);

    alignas(8) uint8_t resolvedValueBytes[0x8];
    float& opacity = CSF_AT(float, resolvedValueBytes, 0x4);
    opacity = 1.0f;

    const int attachment = GfxObjectProps_GetObjectAttachment(
        static_cast<int>(self->field_000c));
    if (static_cast<uint8_t>(
            GetBit2OfField4(reinterpret_cast<void*>(attachment), nullptr)))
        opacity = static_cast<float>(
            S_006e6100_GetRenderOpacity(reinterpret_cast<int>(self)));

    propertiesObject = reinterpret_cast<void*>(self->field_0040);
    properties = ((C_0085e7b8* (__fastcall*)(void*, void*))
        CSF_AT(void**, propertiesObject, 0)[0x1C / 4])(
            propertiesObject, nullptr);

    int* resolvedValue = C_0085e7b8_ResolveChainedValue(
        properties, nullptr, reinterpret_cast<int*>(resolvedValueBytes), 3);
    opacity *= *reinterpret_cast<float*>(resolvedValue);

    if (opacity < g_TransitionOne)
        SkinGeometry_SetMaterialAlpha(
            static_cast<int>(self->field_000c),
            static_cast<int>(
                static_cast<int64_t>(opacity * g_ColorChannelScale)));

    GfxEnvironment_SetSectorObjectFlag4(
        static_cast<int>(self->field_000c), 1);

    using AtomicRenderFunction = void (__cdecl*)(int);
    const uint32_t atomic = self->field_000c;
    CSF_AT(AtomicRenderFunction, reinterpret_cast<void*>(atomic), 0x48)(
        static_cast<int>(atomic));

    GfxEnvironment_SetSectorObjectFlag4(
        static_cast<int>(self->field_000c), 0);

    if (uvTransform)
        RpMatFXMaterialSetUVTransformMatrices(uvTransform[0], 0, 0);

    if (opacity < g_TransitionOne)
        SkinGeometry_SetMaterialAlpha(
            static_cast<int>(self->field_000c), 255);
}

HOOK(0x006E61B0, S_006e6100_Render, "auto");
