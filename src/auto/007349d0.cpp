#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the uint32_t stored at offset 0x98 inside the object.
// Original: lea eax, [ecx + 0x98] / ret
static char* __fastcall C_008670f0_GetField0098(C_008670f0* self, void* /*edx*/)
{
    return reinterpret_cast<char*>(&self->field_0098);
}

HOOK(0x007349D0, C_008670f0_GetField0098, "auto");
