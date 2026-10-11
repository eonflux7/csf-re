#include "hook.h"
#include "types/auto_classes.h"

// Returns true when the virtual slot +0x14 result is not -1 (the "invalid" sentinel).
static bool __fastcall C_008344f0_IsSlot14ResultNotMinusOne(C_008344f0* self, void* /*edx*/, int a2)
{
    return self->vSlot14(a2) != -1;
}

HOOK(0x4A2C60, C_008344f0_IsSlot14ResultNotMinusOne, "auto");
