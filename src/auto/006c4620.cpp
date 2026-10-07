#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x006C4620: stores the argument into field_000c and returns it.
static int __fastcall C_008670f0_SetField0C(C_008670f0* self, void* /*edx*/, int value)
{
    self->field_000c = value;
    return value;
}

HOOK(0x006C4620, C_008670f0_SetField0C, "auto");
