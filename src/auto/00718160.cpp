#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00718160: returns the 32-bit field at offset 0x00A4 of C_00864288.
static int __fastcall C_00864288_GetField00a4(C_00864288* self, void* /*edx*/)
{
    return static_cast<int>(self->field_00a4);
}

HOOK(0x00718160, C_00864288_GetField00a4, "auto");
