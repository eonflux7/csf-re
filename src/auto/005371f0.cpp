#include <cstdint>
#include "hook.h"

// 0x005371F0 (7 bytes) - __thiscall, ret 0: returns the byte field at this+0x181.
// No class is known for this object, so the field is read by raw offset.
static char __fastcall C_Unknown_GetByte_0181(void* self, void* /*edx*/)
{
    return *reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(self) + 0x181);
}

HOOK(0x5371F0, C_Unknown_GetByte_0181, "auto");
