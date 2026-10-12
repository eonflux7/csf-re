#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the number of 4-byte elements in the [+0x4, +0x8) pointer range,
// or 0 when the begin pointer is null.
static int __fastcall S_00403a70_GetElementCount(S_00403a70* self, void* /*edx*/)
{
    uint32_t pBegin = self->field_0004;
    if (pBegin)
        return static_cast<int32_t>(self->field_0008 - pBegin) >> 2;
    return 0;
}

HOOK(0x4022C0, S_00403a70_GetElementCount, "auto");
