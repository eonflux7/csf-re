#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the number of 4-byte elements in the [field_0048, field_004c) range,
// or 0 when the range is empty (field_0048 is null).
static int __fastcall CountRangeElements(C_00864508* self, void* /*edx*/)
{
    uint32_t pBegin = self->field_0048;
    if (!pBegin)
        return 0;

    return static_cast<int>(self->field_004c - pBegin) >> 2;
}

HOOK(0x71A300, CountRangeElements, "auto");
