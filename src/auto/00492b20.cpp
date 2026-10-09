#include <cstdint>
#include "hook.h"

// Returns a pointer to the object/field located at offset 0x1B4 within `this`.
// Original: lea eax, [ecx+0x1B4] ; ret
static char* __fastcall S_00492b20_GetPtr0x1B4(void* self, void* /*edx*/)
{
    return reinterpret_cast<char*>(self) + 0x1B4;
}

HOOK(0x00492B20, S_00492b20_GetPtr0x1B4, "auto");
