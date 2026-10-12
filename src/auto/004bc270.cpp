#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

static int __fastcall S_0041e5f0_AppendResetCommand(
    S_0041e5f0* self, void* /*edx*/)
{
    if (self->field_0008 >= self->field_0004)
        S_0041e5f0_GrowArrayCapacity(
            self, nullptr, static_cast<int>(self->field_000c));

    const uint32_t count = self->field_0008;
    auto* entry = reinterpret_cast<uint32_t*>(
        self->field_0000 + 4 * count);
    const uint32_t newCount = count + 1;
    self->field_0008 = newCount;
    *entry = 7;
    return static_cast<int>(newCount);
}

HOOK(0x004BC270, S_0041e5f0_AppendResetCommand, "auto");
