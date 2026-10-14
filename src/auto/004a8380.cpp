#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuMpGameMode_GetSceneEntry, 0x0040F790, (CMenuMpGameMode* self, void* , int index));
CSF_FUNC(char, __fastcall, C_0082f9f8_BeginTaskAtTarget, 0x00420A70, (C_0082f9f8* self, void* , uint32_t* target, int param));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, C_00834050_SetActionMode, 0x00492760, (void* self, void* , int newActionMode));

CSF_GLOBAL(float, g_MountDegreesToRadians, 0x00834818);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall AiMountTasks_BeginTaskAtMountTarget(void* self, void* /*edx*/)
{
    auto* mount = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(
                static_cast<uint8_t*>(CSF_AT(void*, self, 0x0c)) + 256))));
    if (!mount)
        return 0;

    void* character = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118);
    auto* task = CSF_AT(C_0082f9f8*, character, 0x30);
    void* mountData = CSF_AT(void*, self, 0x0c);
    auto* sceneEntries = reinterpret_cast<CMenuMpGameMode*>(mount->field_004c);
    int entryAddress = CMenuMpGameMode_GetSceneEntry(
        sceneEntries, nullptr, CSF_AT(int, mountData, 0x10c));

    // Select the base wrapper explicitly: this slot takes no stack arguments
    // and returns the full pointer-sized value in EAX.
    void* mountObjects = reinterpret_cast<void*>(
        mount->C_00833908::vGetField016c());
    int targetIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, mountObjects, 0)[0x98 / 4])(
                mountObjects, nullptr,
                CSF_AT(int, reinterpret_cast<void*>(entryAddress), 0x08));

    alignas(8) uint8_t targetBytes[0x18];
    auto* target = reinterpret_cast<uint32_t*>(targetBytes);
    target[0] = 0;
    target[1] = 0;
    target[2] = 0;
    float targetOrientation = 0.0f;

    if (targetIndex == -1)
    {
        C_00833908_GetTargetName(
            CSF_AT(C_00833908*, CSF_AT(void*, self, 0x08), 0x118),
            nullptr);
    }
    else
    {
        mountObjects = reinterpret_cast<void*>(
            mount->C_00833908::vGetField016c());
        void* targetObject =
            reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, mountObjects, 0)[0x8c / 4])(
                    mountObjects, nullptr, targetIndex);

        void* transform =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, targetObject, 0)[0x18 / 4])(
                    targetObject, nullptr);
        auto* position =
            reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, transform, 0)[0x10 / 4])(
                    transform, nullptr);
        target[0] = position[0];
        target[1] = position[1];
        target[2] = position[2];

        transform =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, targetObject, 0)[0x18 / 4])(
                    targetObject, nullptr);
        targetOrientation = static_cast<float>(
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                CSF_AT(void**, transform, 0)[0x28 / 4])(
                    transform, nullptr));
    }

    entryAddress = CMenuMpGameMode_GetSceneEntry(
        sceneEntries, nullptr,
        CSF_AT(int, CSF_AT(void*, self, 0x0c), 0x10c));
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(entryAddress), 0x0c))
    {
        sceneEntries->vHandleLinkedItemChange(
            CSF_AT(int, CSF_AT(void*, self, 0x0c), 0x10c));
    }

    character = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, character, 0)[0xb4 / 4])(
            character, nullptr, 1);

    C_00834050_SetActionMode(
        CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118), nullptr, 0);

    float orientation = static_cast<float>(
        (C_008342e8_ComputeResolvedObjectScalar(mount, nullptr) +
         targetOrientation) * g_MountDegreesToRadians);
    int orientationParameter;
    std::memcpy(&orientationParameter, &orientation, sizeof(orientationParameter));
    return C_0082f9f8_BeginTaskAtTarget(
        task, nullptr, target, orientationParameter);
}

HOOK(0x004A8380, AiMountTasks_BeginTaskAtMountTarget, "auto");
