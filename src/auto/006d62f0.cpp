#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x006D62F0 (7 bytes) — reads the byte flag at offset 0xD8.
static char __fastcall C_00861120_GetField00D8(C_00861120* self, void* /*edx*/)
{
    return static_cast<char>(self->field_00d8);
}

HOOK(0x006D62F0, C_00861120_GetField00D8, "auto");
