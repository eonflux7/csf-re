// Function at 0x0041E230: clears three member fields and returns 0.
// __thiscall(this) -> written as __fastcall(self, edx).

#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

static int __fastcall C_0082e598_ClearStateFields(C_0082e598* self, void* /*edx*/)
{
    self->field_0010 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    return 0;
}

HOOK(0x41E230, C_0082e598_ClearStateFields, "auto");
