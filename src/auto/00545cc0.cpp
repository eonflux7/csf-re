#include <cstdint>
#include <cstddef>
#include "types/auto_classes.h"
#include "hook.h"

// 0x00545CC0 — zeroes two status fields of a C_00843cb8 object and returns 0.
static int __fastcall C_00843cb8_ResetStatusFields(C_00843cb8* self, void* /*edx*/)
{
    self->field_0010 = 0;
    self->field_0024 = 0;
    return 0;
}

HOOK(0x00545CC0, C_00843cb8_ResetStatusFields, "auto");
