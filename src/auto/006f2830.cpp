#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Reads the 32-bit value stored at offset 0x1C of the object into *out
// and returns the out pointer. __thiscall -> __fastcall(self, edx, out).
static uint32_t* __fastcall GetValueAtOffset1C(void* self, void* /*edx*/, uint32_t* out)
{
    *out = CSF_AT(uint32_t, self, 0x1C);
    return out;
}

HOOK(0x006F2830, GetValueAtOffset1C, "auto");
