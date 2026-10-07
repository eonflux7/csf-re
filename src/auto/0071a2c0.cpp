#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Sets the float at +0x9C to 0 (dword store of 0).
static void __fastcall C_00864508_ResetField009C(C_00864508* self, void* /*edx*/)
{
    self->field_009c = 0.0f;
}

HOOK(0x0071A2C0, C_00864508_ResetField009C, "auto");
