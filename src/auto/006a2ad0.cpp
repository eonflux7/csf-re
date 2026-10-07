#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Returns whether the two stored values (fields at +0x0C and +0x10) are equal.
// Typical use: begin/end cursor comparison - equal means the range is empty.
static int __fastcall C_0085edfc_IsValueAt000cEqualValueAt0010(C_0085edfc* self, void* /*edx*/)
{
    return self->field_000c == self->field_0010;
}

HOOK(0x006A2AD0, C_0085edfc_IsValueAt000cEqualValueAt0010, "auto");
