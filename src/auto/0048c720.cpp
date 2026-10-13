#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, S_0048c6c0_EvaluateDistance, 0x0048C6C0, (void* self, void* edx, float a2));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

[[maybe_unused]] static auto const kDistanceCheckVtable = CSF_VTBL(0x0048C6C0);

static bool __fastcall S_0048c6c0_CheckTarget(
    S_0048c6c0* self, void* /*edx*/, int sourceHandle, int targetHandle)
{
    auto* source = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, sourceHandle));

    if (!source || CSF_AT(int, reinterpret_cast<void*>(targetHandle), 0x08) < 0)
        return false;

    auto* target = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, targetHandle));

    if (!target)
        return false;

    const float* sourcePosition = GetObjectPositionVector(source, nullptr);
    const float* targetPosition = GetObjectPositionVector(target, nullptr);

    const double dx = static_cast<double>(targetPosition[0]) - sourcePosition[0];
    const double dy = static_cast<double>(targetPosition[1]) - sourcePosition[1];
    const double dz = static_cast<double>(targetPosition[2]) - sourcePosition[2];
    const float distance = static_cast<float>(
        std::sqrt(dz * dz + dy * dy + dx * dx));

    if (!target->field_0050)
        return static_cast<std::uint8_t>(
            S_0048c6c0_EvaluateDistance(self, nullptr, distance)) == 0;

    if (CSF_AT(float, self, 0x04) < CSF_AT(float, self, 0x0C))
        return true;

    return static_cast<std::uint8_t>(
        S_0048c6c0_EvaluateDistance(self, nullptr, distance)) == 0;
}

HOOK(0x0048C720, S_0048c6c0_CheckTarget, "auto");
