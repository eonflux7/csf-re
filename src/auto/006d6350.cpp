#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Getter for the one-byte flag at offset 0xDA (this+218).
// Original:
//   006d6350  mov al, byte ptr [ecx + 0xda]
//   006d6356  ret
static uint8_t __fastcall C_00861120_GetFlag00DA(C_00861120* self, void* /*edx*/)
{
    return self->field_00da;
}

HOOK(0x6D6350, C_00861120_GetFlag00DA, "auto");
