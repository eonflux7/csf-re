#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CMenuMpGameMode_GetSceneEntry, 0x0040F790, (CMenuMpGameMode* self, void* , int index));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(void, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float payload));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, C_Unknown_GetField_01c0, 0x00492B30, (void* self, void*));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, C_008342e8_SelectPlacement, 0x0049F220, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovement_4DB140, 0x004DB140, (C_0082f9f8* self, void* , int movement));

static int __fastcall C_0082f9f8_StartMovementTowardMountPlacement(
    C_0082f9f8* self, void* /*edx*/)
{
    alignas(8) uint8_t positionBytes[0x10];

    auto* mount = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x100)));

    bool placementFound = false;
    if (mount)
    {
        const int placement = C_Unknown_GetField_01c0(
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x118),
            nullptr);
        auto* sceneMode = reinterpret_cast<CMenuMpGameMode*>(mount->field_004c);

        CSF_AT(int, positionBytes, 0x0C) = placement;
        C_008342e8_CopyControlPosition(
            mount, nullptr, placement, reinterpret_cast<uint32_t*>(positionBytes));

        auto* sceneEntry = reinterpret_cast<uint32_t*>(
            CMenuMpGameMode_GetSceneEntry(
                sceneMode, nullptr, CSF_AT(int, positionBytes, 0x00)));

        C_008342e8_SelectPlacement(
            mount, nullptr, CSF_AT(int, positionBytes, 0x0C));

        // Both scene-entry variants perform the same lookup and setup.
        void* placements = reinterpret_cast<void*>(mount->vGetField016c());
        const int placementIndex =
            ((int(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, placements, 0)[0x98 / 4])(
                    placements, nullptr, sceneEntry[1]);

        if (placementIndex != -1)
        {
            placements = reinterpret_cast<void*>(mount->vGetField016c());
            void* selectedPlacement =
                ((void*(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, placements, 0)[0x8C / 4])(
                        placements, nullptr, placementIndex);

            void* placementData =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, selectedPlacement, 0)[0x18 / 4])(
                        selectedPlacement, nullptr);
            int* focusEntry =
                ((int*(__fastcall*)(void*, void*))
                    CSF_AT(void**, placementData, 0)[0x10 / 4])(
                        placementData, nullptr);

            SceneLoader_ConfigureFocusEntry(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x118),
                nullptr, focusEntry);

            placementData =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, selectedPlacement, 0)[0x18 / 4])(
                        selectedPlacement, nullptr);
            CSF_AT(float, positionBytes, 0x0C) = static_cast<float>(
                ((double(__fastcall*)(void*, void*))
                    CSF_AT(void**, placementData, 0)[0x28 / 4])(
                        placementData, nullptr));

            const double mountScalar =
                C_008342e8_ComputeResolvedObjectScalar(mount, nullptr);
            const float payload = static_cast<float>(
                mountScalar + CSF_AT(float, positionBytes, 0x0C));

            DispatchSceneObjectWithPayload(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_0008), 0x118),
                nullptr, payload);
            placementFound = true;
        }
    }

    if (!placementFound)
    {
        C_00833908_GetTargetName(
            CSF_AT(C_00833908*, reinterpret_cast<void*>(self->field_0008), 0x118),
            nullptr);
    }

    return C_0082f9f8_StartMovement_4DB140(
        self, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0xEC));
}

HOOK(0x004A70C0, C_0082f9f8_StartMovementTowardMountPlacement, "auto");
