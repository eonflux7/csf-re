#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// __thiscall char sub_6F2870(_BYTE* this, char a2)
// Stores the incoming byte at this+0x28 and returns it unchanged.
static char __fastcall SetByteField28(void* self, void* /*edx*/, char value)
{
    CSF_AT(uint8_t, self, 0x28) = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x006F2870, SetByteField28, "auto");
