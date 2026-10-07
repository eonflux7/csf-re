#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the byte flag stored at offset 0x115.
static char __fastcall C_0082dfd0_GetField0115(C_0082dfd0* self, void* /*edx*/)
{
    return static_cast<char>(self->field_0115);
}

HOOK(0x00412060, C_0082dfd0_GetField0115, "auto");
