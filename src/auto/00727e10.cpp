#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x00727E10
// Returns the signed count of 36-byte elements stored between the array
// begin/end pointers at +0x48 / +0x4c, or 0 when the array is empty.
static int __fastcall C_008655e8_CountArrayElements(C_008655e8* self, void* /*edx*/)
{
    const uint32_t begin = self->field_0048;
    if (begin == 0)
        return 0;
    return static_cast<int>(self->field_004c - begin) / 36;
}

HOOK(0x00727E10, C_008655e8_CountArrayElements, "auto");
