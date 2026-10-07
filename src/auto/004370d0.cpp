#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the dword stored at offset 0x128 of the object.
static int __fastcall C_00864288_GetField0128(C_00864288* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0128);
}

HOOK(0x4370D0, C_00864288_GetField0128, "auto");
