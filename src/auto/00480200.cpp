#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x00480200: sets field_0028 to -1 (0xFFFFFFFF). __thiscall, no stack args -> ret 0.
static void __fastcall C_00833228_ResetField0028(C_00833228* self, void* /*edx*/)
{
    self->field_0028 = 0xFFFFFFFF;
}

HOOK(0x480200, C_00833228_ResetField0028, "auto");
