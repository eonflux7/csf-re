#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00713280: stores the float constant 0.2f (0x3E4CCCCD) into field_0c94.
static void __fastcall CInputDirectInput_SetFloat0c94(CInputDirectInput* self, void* /*edx*/)
{
    self->field_0c94 = 0x3E4CCCCDu;
}

HOOK(0x00713280, CInputDirectInput_SetFloat0c94, "auto");
