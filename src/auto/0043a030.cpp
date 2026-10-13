#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, IsValueInWrappedRange, 0x00438740, (float rangeStart, float rangeEnd, float value));
CSF_FUNC(double, __cdecl, NormalizeDegreesAngle, 0x00439B20, (float angleDegrees));
CSF_FUNC(int, __fastcall, S_00439ba0_UpdateFourSlots, 0x00439BA0, (S_00439ba0* self, void*));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_FullRotationDegrees, 0x00830D04);

static char __fastcall S_00439ba0_RefreshSceneMenuPanel(
    S_00439ba0* self, void* /*edx*/)
{
    // The packet also lists this direct callee address as a vtable reference.
    (void)CSF_VTBL(0x00439BA0);

    const char enabled = static_cast<char>(self->field_004d);
    if (!enabled)
        return enabled;

    const float angle =
        static_cast<float>(NormalizeDegreesAngle(self->field_0048));
    const float halfSpan = self->field_0010 * g_HalfDurationScale;

    float rangeStart = angle - halfSpan;
    if (rangeStart < g_fZero)
    {
        rangeStart = static_cast<float>(
            NormalizeDegreesAngle(rangeStart * g_MissingMetricValue));
    }

    float rangeEnd = halfSpan + angle;
    if (rangeEnd > g_FullRotationDegrees)
        rangeEnd -= g_FullRotationDegrees;

    auto* slot = reinterpret_cast<uint8_t*>(self) + 0x60;
    for (int remaining = 4; remaining; --remaining, slot += 0x20)
    {
        if (IsValueInWrappedRange(
                rangeStart, rangeEnd, CSF_AT(float, slot, 0x00)))
        {
            const float slotAngle = CSF_AT(float, slot, 0x00);
            const float angleOffset = slotAngle < rangeStart
                ? slotAngle + g_FullRotationDegrees - rangeStart
                : slotAngle - rangeStart;

            const int64_t horizontalOffset = static_cast<int64_t>(
                static_cast<float>(static_cast<int32_t>(self->field_0008))
                / self->field_0010 * angleOffset);
            const int64_t halfSlotWidth = static_cast<int64_t>(
                static_cast<float>(static_cast<int32_t>(self->field_00d4))
                * g_HalfDurationScale);

            CSF_AT(uint32_t, slot, 0x04) =
                static_cast<uint32_t>(horizontalOffset)
                - static_cast<uint32_t>(halfSlotWidth)
                + self->field_0000 + 1;
            CSF_AT(uint32_t, slot, 0x08) =
                self->field_0004 - self->field_00d8;
            CSF_AT(uint8_t, slot, 0x0C) = 1;
        }
        else
        {
            CSF_AT(uint8_t, slot, 0x0C) = 0;
        }
    }

    return static_cast<char>(S_00439ba0_UpdateFourSlots(self, nullptr));
}

HOOK(0x0043A030, S_00439ba0_RefreshSceneMenuPanel, "auto");
