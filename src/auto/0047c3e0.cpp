#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiAttackTasks_SetTargetUpdateMode, 0x0048C9D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_UpdateTargetData, 0x004D1730, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_CloseTargetDistanceSquared, 0x00831148);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

[[maybe_unused]] static const auto g_TaskTargetNameBufferVtable = CSF_VTBL(0x0047AE10);
[[maybe_unused]] static const auto g_TargetHandleVtable = CSF_VTBL(0x0048C9F0);

static void __fastcall AiAttackTasks_UpdateAttackTarget(
    void* self, void* /*edx*/, S_0048c9f0* targetHandle)
{
    char* targetData = AiCore_GetTaskTargetNameBuffer(
        CSF_AT(void*, self, 0x08), nullptr);
    AiCore_UpdateTargetData(
        targetData, nullptr, reinterpret_cast<int>(targetHandle));

    void* core = CSF_AT(void*, self, 0x08);
    void* owner = CSF_AT(void*, core, 0x14);
    AiAttackTasks_CopyTargetHandle(
        CSF_AT(void*, owner, 0x204), nullptr,
        reinterpret_cast<int>(targetHandle));

    core = CSF_AT(void*, self, 0x08);
    if (CSF_AT(int, core, 0x20) >= 0)
    {
        void* currentTarget = reinterpret_cast<void*>(
            CHandleTable_Resolve(
                g_pCurrentScene, nullptr,
                reinterpret_cast<int>(
                    static_cast<std::uint8_t*>(core) + 0x18)));

        if (!currentTarget ||
            static_cast<std::uint8_t>(
                SceneObj_IsInactive(currentTarget, nullptr)) != 0)
        {
            core = CSF_AT(void*, self, 0x08);
            CSF_AT(int, core, 0x1C) = -1;
            CSF_AT(int, core, 0x20) = -1;
        }
    }

    if (CSF_AT(int, targetHandle, 0x08) < 0)
        return;

    void* target = reinterpret_cast<void*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(targetHandle)));
    if (!target || CSF_AT(int, target, 0x50) != 0)
        return;

    core = CSF_AT(void*, self, 0x08);
    float* ownerPosition = GetObjectPositionVector(
        CSF_AT(void*, core, 0x14), nullptr);
    float* targetPosition = GetObjectPositionVector(target, nullptr);

    const float dx = targetPosition[0] - ownerPosition[0];
    const float dy = targetPosition[1] - ownerPosition[1];
    const float dz = targetPosition[2] - ownerPosition[2];
    const float distanceSquared = dz * dz + dy * dy + dx * dx;

    if (distanceSquared < g_CloseTargetDistanceSquared)
    {
        core = CSF_AT(void*, self, 0x08);
        CSF_AT(std::uint8_t, self, 0x14) = 1;
        owner = CSF_AT(void*, core, 0x14);
        AiAttackTasks_SetTargetUpdateMode(
            CSF_AT(void*, owner, 0x204), nullptr, 10);
    }
}

HOOK(0x0047C3E0, AiAttackTasks_UpdateAttackTarget, "auto");
