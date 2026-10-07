#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the byte value stored at offset 0x90.
static char __fastcall C_008605d8_GetByte90(C_008605d8* self, void* /*edx*/)
{
    return static_cast<char>(self->field_0090);
}

HOOK(0x6BC0E0, C_008605d8_GetByte90, "auto");
