#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the number of 4-byte elements in the [field_0024, field_0028) range,
// or 0 when the range is empty (field_0024 is null).
static int __fastcall CountRangeElements24(C_00864508* self, void* /*edx*/)
{
    uint32_t pBegin = self->field_0024;
    if (!pBegin)
        return 0;

    return static_cast<int>(self->field_0028 - pBegin) >> 2;
}

HOOK(0x71A320, CountRangeElements24, "auto");
