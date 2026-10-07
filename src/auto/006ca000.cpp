#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns field_0010 clamped to zero: 0 when the signed value is negative,
// otherwise the value itself.
static int __fastcall C_00862598_GetNonNegativeField0010(C_00862598* self, void* /*edx*/)
{
    int value = static_cast<int>(self->field_0010);
    return value < 0 ? 0 : value;
}

HOOK(0x6CA000, C_00862598_GetNonNegativeField0010, "auto");
