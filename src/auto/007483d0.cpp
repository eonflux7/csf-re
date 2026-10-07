#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Reads the byte flag stored at offset +0x332 of the object and clears it,
// returning the previous value.
// Original: __thiscall, `mov al, [ecx+0x332]` / `mov byte ptr [ecx+0x332], 0` / `ret`
// (byte return: eax's upper bits are left as-is, so the return type is char, not int)
static char __fastcall ConsumeFlagByteAndClear(void* self, void* /*edx*/)
{
    const char previousValue = static_cast<char>(CSF_AT(uint8_t, self, 0x332));
    CSF_AT(uint8_t, self, 0x332) = 0;
    return previousValue;
}

HOOK(0x7483D0, ConsumeFlagByteAndClear, "auto");
