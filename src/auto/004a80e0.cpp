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
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(uint32_t*, __fastcall, C_008342e8_CopyControlPositionToOutput, 0x0049EDC0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCore_GetSceneEntryCount, 0x0052E420, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_MountDegreesToRadians, 0x00834818);

static char __fastcall C_00834fa8_BeginTaskAtMountPosition(
    C_00834fa8* self, void* /*edx*/, int positionIndex)
{
    const int handle = CSF_AT(int, self, 0x0C) + 256;
    CSF_AT(uint8_t, self, 0x14) = 0;

    auto* mount = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!mount)
        return 0;

    alignas(8) uint8_t controlPositionBytes[0xC];
    alignas(8) uint8_t targetBytes[0x14];
    auto* controlPosition = reinterpret_cast<uint32_t*>(controlPositionBytes);
    auto* target = reinterpret_cast<uint32_t*>(targetBytes);

    auto* scene = CSF_AT(CMenuMpGameMode*, mount, 0x4C);
    CSF_AT(CMenuMpGameMode*, targetBytes, 0x10) = scene;
    AiCore_GetSceneEntryCount(scene, nullptr);
    C_008342e8_ComputeResolvedObjectScalar(mount, nullptr);

    target[0] = 0;
    target[1] = 0;
    target[2] = 0;
    C_008342e8_CopyControlPosition(mount, nullptr, positionIndex, controlPosition);

    auto* entry = reinterpret_cast<void*>(
        CMenuMpGameMode_GetSceneEntry(scene, nullptr, static_cast<int>(controlPosition[0])));

    auto* controls = reinterpret_cast<void*>(mount->vGetField016c());
    const int controlIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controls, 0)[0x98 / 4])(
                controls, nullptr, CSF_AT(int, entry, 0x04));

    if (controlIndex == -1)
        return static_cast<char>(CharTask_LookupByOffset(
            self, nullptr, CSF_AT(int, self, 0x0C) + 236));

    controls = reinterpret_cast<void*>(mount->vGetField016c());
    auto* control = reinterpret_cast<void*>(
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, controls, 0)[0x8C / 4])(
                controls, nullptr, controlIndex));

    if (!control)
    {
        C_00833908_GetTargetName(
            CSF_AT(C_00833908*, CSF_AT(void*, self, 0x08), 0x118), nullptr);
        return static_cast<char>(CharTask_LookupByOffset(
            self, nullptr, CSF_AT(int, self, 0x0C) + 236));
    }

    auto* transform =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x18 / 4])(control, nullptr);
    auto* position =
        reinterpret_cast<uint32_t*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);
    target[0] = position[0];
    target[1] = position[1];
    target[2] = position[2];

    transform =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x18 / 4])(control, nullptr);
    CSF_AT(float, targetBytes, 0x0C) = static_cast<float>(
        reinterpret_cast<double(__fastcall*)(void*, void*)>(
            CSF_AT(void**, transform, 0)[0x28 / 4])(transform, nullptr));

    if (positionIndex == -1)
        return static_cast<char>(CharTask_LookupByOffset(
            self, nullptr, CSF_AT(int, self, 0x0C) + 236));

    scene = CSF_AT(CMenuMpGameMode*, targetBytes, 0x10);
    CSF_AT(uint32_t, CSF_AT(void*, self, 0x0C), 0x10C) = controlPosition[0];
    CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x110) = positionIndex;

    entry = reinterpret_cast<void*>(CMenuMpGameMode_GetSceneEntry(
        scene, nullptr, CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x10C)));

    if (!CSF_AT(uint8_t, entry, 0x0C) &&
        !CSF_AT(uint8_t, entry, 0x0D) &&
        (CSF_AT(int, entry, 0x10) != -1 || CSF_AT(int, entry, 0x14) != -1))
    {
        scene->vHandleLinkedItemChange(
            CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x10C));
    }

    const int outputAddress =
        CSF_AT(int, CSF_AT(void*, self, 0x08), 0x118) + 28;
    const int selectedPosition = CSF_AT(int, CSF_AT(void*, self, 0x0C), 0x110);
    C_008342e8_CopyControlPositionToOutput(
        mount, nullptr, selectedPosition, outputAddress);

    auto* actor = CSF_AT(
        C_0082f9f8*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118), 0x30);
    const double mountAngle = C_008342e8_ComputeResolvedObjectScalar(mount, nullptr);
    const float angle = static_cast<float>(
        (mountAngle + CSF_AT(float, targetBytes, 0x0C)) * g_MountDegreesToRadians);
    int angleParameter;
    std::memcpy(&angleParameter, &angle, sizeof(angleParameter));
    return C_0082f9f8_BeginTaskAtTarget(actor, nullptr, target, angleParameter);
}

HOOK(0x004A80E0, C_00834fa8_BeginTaskAtMountPosition, "auto");
