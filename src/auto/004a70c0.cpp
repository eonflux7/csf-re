#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CMenuMpGameMode_GetSceneEntry, 0x0040F790, (CMenuMpGameMode* self, void* , int index));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float payload));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, C_Unknown_GetField_01c0, 0x00492B30, (void* self, void*));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (C_008342e8* self, void* , int index, uint32_t* out));
CSF_FUNC(int, __fastcall, C_008342e8_SelectPlacement, 0x0049F220, (C_008342e8* self, void* , int a2));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovement_4DB140, 0x004DB140, (C_0082f9f8* self, void* , int movement));

static int __fastcall C_0082f9f8_StartMovementTowardMountPlacement(
    C_0082f9f8* self, void* /*edx*/)
{
    auto* mount = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x100)));

    bool focusConfigured = false;
    if (mount)
    {
        alignas(8) uint8_t positionBytes[0x10];

        const int placementIndex = C_Unknown_GetField_01c0(
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x118),
            nullptr);
        auto* mode = reinterpret_cast<CMenuMpGameMode*>(mount->field_004c);
        CSF_AT(int, positionBytes, 0xC) = placementIndex;

        C_008342e8_CopyControlPosition(
            mount, nullptr, placementIndex,
            reinterpret_cast<uint32_t*>(positionBytes));

        auto* sceneEntry = reinterpret_cast<uint32_t*>(
            CMenuMpGameMode_GetSceneEntry(
                mode, nullptr, CSF_AT(int, positionBytes, 0)));

        C_008342e8_SelectPlacement(
            mount, nullptr, CSF_AT(int, positionBytes, 0xC));

        // Both scene-entry type branches perform identical operations.
        void* placementCollection =
            reinterpret_cast<void*>(mount->vGetField016c());
        const int objectIndex =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, placementCollection, 0)[0x98 / 4])(
                    placementCollection, nullptr, sceneEntry[1]);

        if (objectIndex != -1)
        {
            placementCollection =
                reinterpret_cast<void*>(mount->vGetField016c());
            void* placementObject =
                reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, placementCollection, 0)[0x8C / 4])(
                        placementCollection, nullptr, objectIndex);

            void* placementData =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, placementObject, 0)[0x18 / 4])(
                        placementObject, nullptr);
            int* focusEntry =
                reinterpret_cast<int*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, placementData, 0)[0x10 / 4])(
                        placementData, nullptr);

            SceneLoader_ConfigureFocusEntry(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x118),
                nullptr, focusEntry);

            placementData =
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, placementObject, 0)[0x18 / 4])(
                        placementObject, nullptr);
            const float placementScalar = static_cast<float>(
                reinterpret_cast<double(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, placementData, 0)[0x28 / 4])(
                        placementData, nullptr));
            const double resolvedScalar =
                C_008342e8_ComputeResolvedObjectScalar(mount, nullptr);

            DispatchSceneObjectWithPayload(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x118),
                nullptr, static_cast<float>(resolvedScalar + placementScalar));
            focusConfigured = true;
        }
    }

    if (!focusConfigured)
    {
        C_00833908_GetTargetName(
            CSF_AT(C_00833908*,
                   reinterpret_cast<void*>(self->field_0008), 0x118),
            nullptr);
    }

    return C_0082f9f8_StartMovement_4DB140(
        self, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0xEC));
}

HOOK(0x004A70C0, C_0082f9f8_StartMovementTowardMountPlacement, "auto");
