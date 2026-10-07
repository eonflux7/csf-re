#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x0072FF90: returns the dword at offset 0x13c of the object.
static int __fastcall C_00865e38_GetField013c(C_00865e38* self, void* /*edx*/)
{
    return static_cast<int>(self->field_013c);
}

HOOK(0x72FF90, C_00865e38_GetField013c, "auto");
