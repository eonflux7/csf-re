#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns a pointer to the object's field at offset 0x8c.
// Original: lea eax, [ecx+0x8c]; ret
static char* __fastcall C_008605d8_GetField008c(C_008605d8* self, void* /*edx*/)
{
    return reinterpret_cast<char*>(&self->field_008c);
}

HOOK(0x006BC0A0, C_008605d8_GetField008c, "auto");
