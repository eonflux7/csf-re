#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the value stored at offset 0x284 (int).
static int __fastcall C_00845438_GetField0284(C_00845438* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0284);
}

HOOK(0x0055C430, C_00845438_GetField0284, "auto");
