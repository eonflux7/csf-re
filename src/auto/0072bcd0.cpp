#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the float stored at offset 0x184, promoted to double (x87 st(0)).
static double __fastcall C_00865b40_GetField0184(C_00865b40* self, void* /*edx*/)
{
    return (double)self->field_0184;
}

HOOK(0x0072BCD0, C_00865b40_GetField0184, "auto");
