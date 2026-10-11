#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall C_008670f0_UpdateProgressState(
    C_008670f0* self, void* /*edx*/)
{
    if (!self->field_00fe)
        return;

    const int32_t firstSpan =
        static_cast<int32_t>(self->field_00a0 - self->field_0098);
    const int32_t secondSpan =
        static_cast<int32_t>(self->field_00a4 - self->field_009c);

    void* source = reinterpret_cast<void*>(self->field_0014);
    const float increment = static_cast<float>(
        (reinterpret_cast<double(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, source, 0)[0x1C / 4]))(source, nullptr, 1));

    if (firstSpan > 0 && firstSpan <= self->field_00ec)
    {
        self->field_00ec = 1.0f;
        self->field_00f0 = increment + self->field_00c4 + self->field_00f0;
    }

    if (secondSpan > 0 &&
        static_cast<int32_t>(static_cast<int64_t>(self->field_00f0)) >= secondSpan)
    {
        self->field_00ec = -1.0f;
        self->field_00f0 = -1.0f;
    }
}

HOOK(0x00733310, C_008670f0_UpdateProgressState, "auto");
