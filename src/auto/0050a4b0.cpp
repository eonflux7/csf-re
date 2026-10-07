#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x0050A4B0 (ret 4): stores the incoming byte into the object's
// field at offset +4 and returns that same byte.
static char __fastcall SetByteFlagAt4(uint8_t* self, void* /*edx*/, char value)
{
    CSF_AT(uint8_t, self, 4) = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x0050A4B0, SetByteFlagAt4, "auto");
