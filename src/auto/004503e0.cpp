#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, CalculateSquaredVectorLength, 0x0068E300, (void* self, void* edx));

CSF_GLOBAL(float, g_MinimumSquaredDistance, 0x00831270);

static char __fastcall S_004503e0_IsSelectedObjectWithinRange(
    S_004503e0* self, void* /*edx*/, C_008342e8* object,
    float range, int result)
{
    if (!object || (object->field_0170 & 1) == 0 || object->vSlot7C())
        return 0;

    void* objectData = reinterpret_cast<void*>(object->vGetField016c());
    if (CSF_AT(int32_t, objectData, 0x78) == -1082130432)
        return 0;

    const uint32_t selectedIndex = self->field_0070;
    if (selectedIndex == static_cast<uint32_t>(-1) ||
        selectedIndex >= self->field_0068)
        return 0;

    auto* objects = reinterpret_cast<C_008342e8**>(self->field_0060);
    C_008342e8* selectedObject = objects[selectedIndex];
    if (!selectedObject || selectedObject == object)
        return 0;

    float* selectedPosition = reinterpret_cast<float*>(
        GetObjectPositionVector(selectedObject, nullptr));
    float facingVector[3];
    void* objectPosition = reinterpret_cast<void*>(
        GetObjectPositionVector(object, nullptr));
    BuildFacingVectorBetween(
        objectPosition, nullptr, facingVector, selectedPosition);

    const double measuredSquaredDistance =
        CalculateSquaredVectorLength(facingVector, nullptr);
    const float squaredDistance = static_cast<float>(measuredSquaredDistance);

    if (measuredSquaredDistance > g_MinimumSquaredDistance &&
        range * range > squaredDistance)
    {
        CSF_AT(float, reinterpret_cast<void*>(result), 0x4) = squaredDistance;
        return 1;
    }

    return 0;
}

HOOK(0x004503E0, S_004503e0_IsSelectedObjectWithinRange, "auto");
