#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_GLOBAL(float, g_DegreesToRadians, 0x00835440);

static int __fastcall S_0041e5f0_AppendAngularCommand(
    S_0041e5f0* self, void* /*edx*/,
    float firstAngle, float secondAngle, int argument)
{
    // The packet's vtable annotation names the direct-call target.
    (void)CSF_VTBL(0x0041E5F0);

    if (self->field_0008 >= self->field_0004)
        S_0041e5f0_GrowArrayCapacity(
            self, nullptr, static_cast<int>(self->field_000c));

    uint32_t index = self->field_0008;
    auto* commandSlot =
        reinterpret_cast<uint32_t*>(self->field_0000) + index;
    float firstRadians = firstAngle * g_DegreesToRadians;
    self->field_0008 = index + 1;
    *commandSlot = 33;

    auto* parameters = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0x10);

    if (parameters->field_0008 >= parameters->field_0004)
        S_0041e5f0_GrowArrayCapacity(
            parameters, nullptr, static_cast<int>(parameters->field_000c));

    index = parameters->field_0008;
    auto* firstAngleSlot =
        reinterpret_cast<float*>(parameters->field_0000) + index;
    float secondRadians = secondAngle * g_DegreesToRadians;
    parameters->field_0008 = index + 1;
    *firstAngleSlot = firstRadians;

    if (parameters->field_0008 >= parameters->field_0004)
        S_0041e5f0_GrowArrayCapacity(
            parameters, nullptr, static_cast<int>(parameters->field_000c));

    index = parameters->field_0008;
    auto* secondAngleSlot =
        reinterpret_cast<float*>(parameters->field_0000) + index;
    parameters->field_0008 = index + 1;
    *secondAngleSlot = secondRadians;

    if (parameters->field_0008 >= parameters->field_0004)
        S_0041e5f0_GrowArrayCapacity(
            parameters, nullptr, static_cast<int>(parameters->field_000c));

    index = parameters->field_0008;
    auto* argumentSlot =
        reinterpret_cast<int*>(parameters->field_0000) + index;
    parameters->field_0008 = index + 1;
    *argumentSlot = argument;
    return argument;
}

HOOK(0x004BC750, S_0041e5f0_AppendAngularCommand, "auto");
