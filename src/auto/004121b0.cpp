#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Geometry_BuildPlaneFromTriangle, 0x004023F0, (void* self, void* edx, float* arg0, float* a3, float* a4));
CSF_FUNC(int, __fastcall, GfxMatrix_AreVectorsDifferent, 0x0040FB20, (void* self, void* edx, float* a2));
CSF_FUNC(float*, __fastcall, C_0082dfd0_GetPosition, 0x00411EF0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, GetRecordAddressByIndex, 0x0044FDB0, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, Geometry_FindIntersectingQuad, 0x0044FDD0, (void* self, void* , float* ellipsoid));
CSF_FUNC(bool, __fastcall, C_00833908_IsGeometryInteractionEnabled, 0x00493CD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetProviderPosition, 0x004F4EB0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(float*, __fastcall, S_00526d40_ProjectPointToPlane, 0x006AD830, (S_00526d40* self, void* , float* a2, float* a3));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static void __fastcall C_0082dfd0_AdjustMovementToIntersectingPlane(
    C_0082dfd0* self, void* /*edx*/, S_00526d40* movement, int /*unused*/)
{
    if (!C_00833908_IsGeometryInteractionEnabled(
            reinterpret_cast<void*>(self->field_0004), nullptr))
    {
        self->field_0089 = 0;
        return;
    }

    void* geometryObject = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self), nullptr));

    void* boundsObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, geometryObject, 0)[0x34 / 4])(
                geometryObject, nullptr);

    float ellipsoid[6];
    std::memcpy(ellipsoid,
                reinterpret_cast<uint8_t*>(boundsObject) + 8,
                0x18);

    const int quadIndex =
        Geometry_FindIntersectingQuad(g_pCurrentScene, nullptr, ellipsoid);
    self->field_0089 = quadIndex >= 0;
    if (quadIndex < 0)
        return;

    if (static_cast<uint8_t>(
            GfxMatrix_AreVectorsDifferent(movement, nullptr, &g_ZeroVectorX)) == 0)
        return;

    float* triangle = reinterpret_cast<float*>(
        GetRecordAddressByIndex(g_pCurrentScene, nullptr, quadIndex));

    alignas(4) uint8_t planeStorage[0x2C] = {};
    auto* plane = reinterpret_cast<S_00526d40*>(planeStorage);
    Geometry_BuildPlaneFromTriangle(
        plane, nullptr, triangle, triangle + 3, triangle + 6);

    float destination[3] = {};
    float* direction = reinterpret_cast<float*>(
        S_00526d40_ConvertDirection(
            movement, nullptr, reinterpret_cast<uint32_t*>(destination)));

    const float normalDot =
        CSF_AT(float, plane, 0x10) * direction[1] +
        CSF_AT(float, plane, 0x0C) * direction[0] +
        CSF_AT(float, plane, 0x14) * direction[2];
    if (!(normalDot > g_fZero))
        return;

    float position[3] = {};
    C_0082dfd0_GetPosition(self, nullptr, position);

    float movementX;
    std::memcpy(&movementX, &movement->field_0000, sizeof(movementX));
    const float movementY = CSF_AT(float, movement, 0x04);
    const float movementZ = CSF_AT(float, movement, 0x08);
    const float distance = std::sqrt(
        movementX * movementX +
        movementY * movementY +
        movementZ * movementZ);

    const int globalObject = GetGlobalValue_4FBDE0();
    const int firstElement =
        GetFirstElement(reinterpret_cast<void*>(globalObject), nullptr);
    void* provider =
        GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
    float* providerPosition = reinterpret_cast<float*>(
        MpSpawnPoints_GetProviderPosition(provider, nullptr));

    float steering[3] = {
        providerPosition[0], providerPosition[1], providerPosition[2]
    };
    if (steering[1] > g_fZero)
        steering[1] = 1.0f;
    else if (steering[1] < g_fZero)
        steering[1] = -1.0f;

    NormalizeVector3(steering, nullptr);

    float source[3] = {position[0], position[1], position[2]};
    float projectedDestinationBuffer[3] = {};
    direction = reinterpret_cast<float*>(
        S_00526d40_ConvertDirection(
            movement, nullptr,
            reinterpret_cast<uint32_t*>(projectedDestinationBuffer)));

    float combinedDirection[3] = {
        steering[0] + direction[0],
        steering[1] + direction[1],
        steering[2] + direction[2]
    };
    float projectedSourceBuffer[3] = {};
    float* adjustedDirection = reinterpret_cast<float*>(
        S_00526d40_ConvertDirection(
            reinterpret_cast<S_00526d40*>(combinedDirection), nullptr,
            reinterpret_cast<uint32_t*>(projectedSourceBuffer)));

    destination[0] = position[0] + distance * adjustedDirection[0];
    destination[1] = position[1] + distance * adjustedDirection[1];
    combinedDirection[2] = distance * adjustedDirection[2];
    destination[2] = position[2] + combinedDirection[2];

    float* projectedSource = S_00526d40_ProjectPointToPlane(
        plane, nullptr, projectedSourceBuffer, source);
    float* projectedDestination = S_00526d40_ProjectPointToPlane(
        plane, nullptr, projectedDestinationBuffer, destination);

    const float deltaX = projectedDestination[0] - projectedSource[0];
    const float deltaY = projectedDestination[1] - projectedSource[1];
    const float deltaZ = projectedDestination[2] - projectedSource[2];

    std::memcpy(&movement->field_0000, &deltaX, sizeof(deltaX));
    CSF_AT(float, movement, 0x04) = deltaY;
    CSF_AT(float, movement, 0x08) = deltaZ;
}

HOOK(0x004121B0, C_0082dfd0_AdjustMovementToIntersectingPlane, "auto");
