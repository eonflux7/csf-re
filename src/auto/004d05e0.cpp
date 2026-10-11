#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(C_00833b88*, __fastcall, SpatialQuery_BuildContext, 0x0040FDA0, (C_00833b88* self, void* , uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __stdcall, FindNearbyFacingPosition, 0x00450140, (void* object, int target, float maximumDistance, std::uint32_t* output));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00862490_GetPosition, 0x004760B0, (C_00862490* self, void* , std::uint32_t* position));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, C_00835228_ValidateTarget, 0x004D0410, (void* self, void* edx, int a2, int a3));

static bool __fastcall C_00835228_IsTargetPositionClear(
    C_00835228* self, void* /*edx*/, int targetHandle, int positionIndex)
{
    if (CSF_AT(int, reinterpret_cast<void*>(targetHandle), 0x8) < 0)
        return false;

    if (!C_00835228_ValidateTarget(self, nullptr, targetHandle, positionIndex))
        return false;

    auto* target = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, targetHandle));
    if (!target)
        return false;

    void* positionProvider = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0014), 0x44);
    if (!positionProvider)
        return false;

    void** providerVtable = CSF_AT(void**, positionProvider, 0);
    int positionKey =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            providerVtable[0xBC / 4])(positionProvider, nullptr, positionIndex);
    auto* positionHandle =
        reinterpret_cast<C_00862490* (__fastcall*)(void*, void*, int)>(
            providerVtable[0xCC / 4])(positionProvider, nullptr, positionKey);
    if (!positionHandle)
        return false;

    auto* positionData = reinterpret_cast<std::uint8_t*>(
        Handle_GetValuePointer(positionHandle, nullptr)) + 4;

    alignas(8) std::uint8_t originBytes[0x1C];
    CSF_AT(float, originBytes, 0x0) = 0.0f;
    CSF_AT(float, originBytes, 0x4) = 0.0f;
    CSF_AT(float, originBytes, 0x8) = 0.0f;
    C_00862490_GetPosition(
        positionHandle, nullptr, reinterpret_cast<std::uint32_t*>(originBytes));

    float* targetPosition = GetObjectPositionVector(target, nullptr);
    float targetX = targetPosition[0];
    float targetY = targetPosition[1];
    float targetZ = targetPosition[2];

    alignas(8) std::uint8_t directionBytes[0xC];
    if ((target->field_0170 & 0x2000) != 0)
    {
        float* adjustedPosition =
            reinterpret_cast<float* (__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, target, 0)[0xA4 / 4])(
                    target, nullptr, reinterpret_cast<float*>(directionBytes));
        targetX = adjustedPosition[0];
        targetY = adjustedPosition[1];
        targetZ = adjustedPosition[2];
    }

    CSF_AT(float, directionBytes, 0x0) =
        targetX - CSF_AT(float, originBytes, 0x0);
    CSF_AT(float, directionBytes, 0x4) =
        targetY - CSF_AT(float, originBytes, 0x4);
    CSF_AT(float, directionBytes, 0x8) =
        targetZ - CSF_AT(float, originBytes, 0x8);

    alignas(8) std::uint8_t contextBytes[0x18];
    C_00833b88& context = *reinterpret_cast<C_00833b88*>(contextBytes);
    SpatialQuery_BuildContext(
        &context, nullptr,
        reinterpret_cast<std::uint32_t*>(originBytes),
        reinterpret_cast<std::uint32_t*>(directionBytes));

    alignas(8) std::uint8_t resultBytes[0xC];
    std::memset(resultBytes, 0, sizeof(resultBytes));

    return !FindNearbyFacingPosition(
        reinterpret_cast<void*>(self->field_0014),
        reinterpret_cast<int>(&context),
        CSF_AT(float, positionData, 0xE8),
        reinterpret_cast<std::uint32_t*>(resultBytes));
}

HOOK(0x004D05E0, C_00835228_IsTargetPositionClear, "auto");
