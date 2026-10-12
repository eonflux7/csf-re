#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082faf8_FinishPath, 0x004240A0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0082faf8_UpdateWaypoint, 0x004242C0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetPathWaypoint, 0x00425B40,
         (void* self, void* edx, int a2));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));

static void __fastcall C_0082faf8_CheckWaypointProgress(
    C_0082faf8* self, void* /*edx*/)
{
    const float* position = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0014), nullptr);
    const float positionX = CSF_AT(float, position, 0);
    const float positionZ = CSF_AT(float, position, 0x08);

    const int32_t lastSegment =
        static_cast<int32_t>(self->field_0040 - 2);
    const int32_t currentWaypoint =
        static_cast<int32_t>(self->field_0064);

    if (currentWaypoint > lastSegment)
        return;

    const bool finalSegment = currentWaypoint == lastSegment;
    const int nextWaypoint =
        static_cast<int32_t>(static_cast<uint32_t>(currentWaypoint) + 1);
    const void* waypoint = reinterpret_cast<const void*>(
        GetPathWaypoint(reinterpret_cast<uint8_t*>(self) + 0x24,
                        nullptr, nextWaypoint));

    const float deltaZ = CSF_AT(float, waypoint, 0x08) - positionZ;
    const float deltaX = CSF_AT(float, waypoint, 0) - positionX;

    if (!finalSegment &&
        CSF_AT(int32_t, waypoint, 0x10) == -1082130432)
    {
        ++self->field_0064;
        return;
    }

    const float radius = CSF_AT(float, waypoint, 0x10);
    if (radius * radius > deltaZ * deltaZ + deltaX * deltaX)
    {
        ++self->field_0064;
        if (finalSegment)
            C_0082faf8_FinishPath(self, nullptr);
        else
            C_0082faf8_UpdateWaypoint(self, nullptr);
    }
}

HOOK(0x00424360, C_0082faf8_CheckWaypointProgress, "auto");
