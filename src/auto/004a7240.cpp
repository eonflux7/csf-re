#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CMenuMpGameMode_GetSceneEntry, 0x0040F790, (CMenuMpGameMode* self, void* , int index));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float scalar));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, C_Unknown_GetField_01c0, 0x00492B30, (void* self, void*));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, C_008342e8_SelectPlacement, 0x0049F220, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovement_4DB140, 0x004DB140, (C_0082f9f8* self, void* , int movement));

static int __fastcall C_0082f9f8_PrepareMountPlacementAndStartMovement(
    C_0082f9f8* self, void* /*edx*/)
{
    alignas(8) uint8_t placementBytes[0xC];
    alignas(8) uint8_t focusBytes[0x14];

    void* resolvedObject = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x100)));

    auto* target = CSF_AT(C_00833908*,
        reinterpret_cast<void*>(self->field_0008), 0x118);

    if (!resolvedObject)
        return C_00833908_GetTargetName(target, nullptr);

    const int placement = C_Unknown_GetField_01c0(target, nullptr);
    CSF_AT(CMenuMpGameMode*, focusBytes, 0x10) =
        CSF_AT(CMenuMpGameMode*, resolvedObject, 0x4C);

    C_008342e8_CopyControlPosition(
        resolvedObject, nullptr, placement,
        reinterpret_cast<uint32_t*>(placementBytes));

    int sceneEntry = CMenuMpGameMode_GetSceneEntry(
        CSF_AT(CMenuMpGameMode*, focusBytes, 0x10), nullptr,
        CSF_AT(int, placementBytes, 0));

    if (CSF_AT(uint32_t, reinterpret_cast<void*>(sceneEntry), 0) != 0)
    {
        return C_0082f9f8_StartMovement_4DB140(
            self, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0xB0));
    }

    void* linkedObject = CSF_AT(void*, self, 0x118);
    void* transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, linkedObject, 0)[0x28 / 4])(linkedObject, nullptr);
    void* positionObject = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, transform, 0)[0x18 / 4])(transform, nullptr);
    auto* position = reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, positionObject, 0)[0x10 / 4])(positionObject, nullptr);

    CSF_AT(uint32_t, focusBytes, 0) = position[0];
    CSF_AT(uint32_t, focusBytes, 4) = position[1];
    CSF_AT(uint32_t, focusBytes, 8) = position[2];

    C_008342e8_SelectPlacement(resolvedObject, nullptr, placement);

    void* placementCollection =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, resolvedObject, 0)[0x28 / 4])(
                resolvedObject, nullptr);

    const int placementIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, placementCollection, 0)[0x98 / 4])(
                placementCollection, nullptr,
                CSF_AT(int, placementBytes, 4));

    if (placementIndex == -1)
    {
        C_00833908_GetTargetName(
            CSF_AT(C_00833908*,
                reinterpret_cast<void*>(self->field_0008), 0x118),
            nullptr);
    }
    else
    {
        placementCollection =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, resolvedObject, 0)[0x28 / 4])(
                    resolvedObject, nullptr);

        void* selectedPlacement =
            reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, placementCollection, 0)[0x8C / 4])(
                    placementCollection, nullptr, placementIndex);

        SceneLoader_ConfigureFocusEntry(
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x118),
            nullptr, reinterpret_cast<int*>(focusBytes));

        void* scalarObject =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, selectedPlacement, 0)[0x18 / 4])(
                    selectedPlacement, nullptr);

        CSF_AT(float, focusBytes, 0xC) =
            static_cast<float>(
                reinterpret_cast<double(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, scalarObject, 0)[0x28 / 4])(
                        scalarObject, nullptr));

        const double resolvedScalar = C_008342e8_ComputeResolvedObjectScalar(
            reinterpret_cast<C_008342e8*>(resolvedObject), nullptr);

        const float scalar =
            static_cast<float>(resolvedScalar + CSF_AT(float, focusBytes, 0xC));

        DispatchSceneObjectWithPayload(
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x118),
            nullptr, scalar);
    }

    CMenuMpGameMode* scene = CSF_AT(CMenuMpGameMode*, focusBytes, 0x10);
    sceneEntry = CMenuMpGameMode_GetSceneEntry(
        scene, nullptr,
        CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x10C));

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(sceneEntry), 0xC) == 0)
    {
        scene->vHandleLinkedItemChange(
            CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x10C));
    }

    return C_0082f9f8_StartMovement_4DB140(
        self, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0xD8));
}

HOOK(0x004A7240, C_0082f9f8_PrepareMountPlacementAndStartMovement, "auto");
