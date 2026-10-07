#include "types/auto_classes.h"
#include "hook.h"

// Returns field at offset 0x148 (no stack arguments, plain getter).
static int __fastcall C_00865e38_GetField0148(C_00865e38* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0148);
}

HOOK(0x00730110, C_00865e38_GetField0148, "auto");
