#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "hook.h"

// Returns the number of 4-byte elements in the [+0x18, +0x1c) pointer range,
// or 0 when the begin pointer is null.
static int __fastcall CCharWeaponFsm_GetPointerRangeElementCount(CCharWeaponFsm* self, void* /*edx*/)
{
    uint32_t pBegin = self->field_0018;
    if (!pBegin)
        return 0;

    uint32_t pEnd = self->field_001c;
    return static_cast<int32_t>(pEnd - pBegin) >> 2;
}

HOOK(0x480D70, CCharWeaponFsm_GetPointerRangeElementCount, "auto");
