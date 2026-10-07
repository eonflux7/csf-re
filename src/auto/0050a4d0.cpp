#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0050A4D0 (__thiscall, ret 4) — stores a byte argument into the object at +5
// and returns that same byte in al.
static char __fastcall SetByteField5(void* self, void* /*edx*/, char value)
{
    CSF_AT(uint8_t, self, 5) = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x0050A4D0, SetByteField5, "auto");
