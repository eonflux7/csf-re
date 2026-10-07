#include <cstdint>

#include "hook.h"
#include "core/memory.h"

// 0x0044F8D0 (__thiscall, ret 0): returns the byte field at offset 0x1A8 of the object.
// The owning class is unknown; the object is treated as an opaque byte buffer.
static char __fastcall C_Unknown_GetByte1A8(void* self, void* /*edx*/)
{
    return *reinterpret_cast<const char*>(static_cast<const uint8_t*>(self) + 0x1A8);
}

HOOK(0x44F8D0, C_Unknown_GetByte1A8, "auto");
