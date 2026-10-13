#include <cstdint>
#include <cstddef>
#include "hook.h"

// Compares two 16-byte (four dword) records for exact equality.
// Thiscall: `this` in ecx, `other` on the stack (ret 4).
static int __fastcall HudStringShow_Equals16Bytes(uint32_t* self, void* /*edx*/, uint32_t* other)
{
    if (self[0] != other[0])
        return 0;
    if (self[1] != other[1])
        return 0;
    if (self[2] != other[2])
        return 0;
    if (self[3] != other[3])
        return 0;
    return 1;
}

HOOK(0x004F41A0, HudStringShow_Equals16Bytes, "auto");
