// Commandos: Strike Force (Pyro Studios) — 0x004E1450
// Resets the object's two extra fields to zero and returns 0.

#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// __thiscall method: ecx = this, edx unused; ret 0 (no stack args).
static int __fastcall C_0083e330_ResetExtraFields(C_0083e330* self, void* /*edx*/)
{
    self->field_004c = 0;
    self->field_0050 = 0;
    return 0;
}

HOOK(0x4E1450, C_0083e330_ResetExtraFields, "auto");
