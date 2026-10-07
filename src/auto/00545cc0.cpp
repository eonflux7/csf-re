#include <cstdint>
#include <cstddef>
#include "types/auto_classes.h"
#include "hook.h"

// 0x00545CC0 — zeroes two status fields of a CMenuExtras object and returns 0.
static int __fastcall CMenuExtras_ResetStatusFields(CMenuExtras* self, void* /*edx*/)
{
    self->field_0010 = 0;
    self->field_0024 = 0;
    return 0;
}

HOOK(0x00545CC0, CMenuExtras_ResetStatusFields, "auto");
