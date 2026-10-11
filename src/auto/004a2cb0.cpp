#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008344f0_ForwardSlot1CWithZeroFlag(C_008344f0* self, void* /*edx*/, int a2, int a3)
{
    return self->vSlot1C(a2, 0, a3);
}

HOOK(0x004A2CB0, C_008344f0_ForwardSlot1CWithZeroFlag, "auto");
