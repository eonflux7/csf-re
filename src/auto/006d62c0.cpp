#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the member at offset 0xdc (220) of this object.
// Original: lea eax, [ecx + 0xdc] ; ret
static char* __fastcall C_00861120_GetField00dcPtr(C_00861120* self, void* /*edx*/)
{
    return reinterpret_cast<char*>(&self->field_00dc);
}

HOOK(0x6D62C0, C_00861120_GetField00dcPtr, "auto");
