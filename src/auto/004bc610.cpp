#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall S_0041e5f0_AppendOffsetParameters(
    S_0041e5f0* self, void* /*edx*/, float offset, int argument)
{
    // The packet also lists the grow-function address as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);

    // Command 28 when offset > 0, otherwise 27 (NaN also yields 27, as in the original fcomp/test ah,0x41).
    int command = 28;
    if (!(offset > g_fZero))
        command = 27;

    if (self->field_0008 >= self->field_0004)
        S_0041e5f0_GrowArrayCapacity(
            self, nullptr, static_cast<int>(self->field_000c));

    uint32_t index = self->field_0008;
    int* commandSlot =
        reinterpret_cast<int*>(self->field_0000 + 4 * index);
    self->field_0008 = index + 1;
    *commandSlot = command;

    const float absoluteOffset = std::fabs(offset);

    auto* parameters = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0x10);

    if (parameters->field_0008 >= parameters->field_0004)
        S_0041e5f0_GrowArrayCapacity(
            parameters, nullptr, static_cast<int>(parameters->field_000c));

    index = parameters->field_0008;
    float* offsetSlot =
        reinterpret_cast<float*>(parameters->field_0000 + 4 * index);
    parameters->field_0008 = index + 1;
    *offsetSlot = absoluteOffset;

    if (parameters->field_0008 >= parameters->field_0004)
        S_0041e5f0_GrowArrayCapacity(
            parameters, nullptr, static_cast<int>(parameters->field_000c));

    index = parameters->field_0008;
    int* argumentSlot =
        reinterpret_cast<int*>(parameters->field_0000 + 4 * index);
    const uint32_t result = index + 1;
    parameters->field_0008 = result;
    *argumentSlot = argument;
    return static_cast<int>(result);
}

HOOK(0x004BC610, S_0041e5f0_AppendOffsetParameters, "auto");
