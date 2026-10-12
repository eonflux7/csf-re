#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetPathRegionEntry, 0x00425B40,
         (void* self, void* edx, int a2));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));

static void __fastcall C_0082faf8_UpdatePathRegionTransitions(
    C_0082faf8* self, void* /*edx*/)
{
    const float* position = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0014), nullptr);
    const float positionX = position[0];
    const float positionZ = position[2];

    if (self->field_0068 != self->field_0064)
    {
        const float* region = reinterpret_cast<const float*>(
            GetPathRegionEntry(
                reinterpret_cast<std::uint8_t*>(self) + 0x24,
                nullptr, static_cast<std::int32_t>(self->field_0064)));
        const float deltaX = region[0] - positionX;
        const float deltaZ = region[2] - positionZ;

        if (region[3] * region[3] > deltaZ * deltaZ + deltaX * deltaX)
            self->field_0068 = self->field_0064;
    }

    if (static_cast<std::int32_t>(self->field_0068) <=
            static_cast<std::int32_t>(self->field_0064) &&
        self->field_006c != self->field_0068)
    {
        const float* region = reinterpret_cast<const float*>(
            GetPathRegionEntry(
                reinterpret_cast<std::uint8_t*>(self) + 0x24,
                nullptr, static_cast<std::int32_t>(self->field_0068)));
        const float deltaX = region[0] - positionX;
        const float deltaZ = region[2] - positionZ;

        if (region[3] * region[3] < deltaZ * deltaZ + deltaX * deltaX)
        {
            const bool captureTransition = self->field_0050 < self->field_0054;
            self->field_006c = self->field_0068;

            if (captureTransition)
            {
                const float ownerValue = CSF_AT(
                    float, reinterpret_cast<void*>(self->field_0014), 0x194);
                const float targetValue = self->field_0054;
                self->field_0058 = ownerValue;
                self->field_005c = targetValue;
            }
        }
    }
}

HOOK(0x004241F0, C_0082faf8_UpdatePathRegionTransitions, "auto");
