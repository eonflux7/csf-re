#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the number of 4-byte elements in the [+0x0C, +0x10) pointer range,
// or 0 when the begin pointer is null.
static int __fastcall C_0086243c_GetElementCount(C_0086243c* self, void* /*edx*/)
{
    uint32_t pBegin = self->field_000c;
    if (pBegin)
        return static_cast<int32_t>(self->field_0010 - pBegin) >> 2;
    return 0;
}

HOOK(0x6F1510, C_0086243c_GetElementCount, "auto");
