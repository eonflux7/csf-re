#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores the byte flag at +0x8c and returns it (original: mov al, [ebp+8]; mov [ecx+0x8c], al; ret 4).
static char __fastcall C_00867260_SetFlagAndReturn(C_00867260* self, void* /*edx*/, char value)
{
    CSF_AT(uint8_t, self, 0x8c) = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x737440, C_00867260_SetFlagAndReturn, "auto");
