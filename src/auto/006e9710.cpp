#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee bound by address: thiscall-style, one stack argument (ret 4).
CSF_FUNC(int, __fastcall, sub_6E9030, 0x006E9030, (void* self, void* edx, int a2));

// Returns the low bit of byte +0x14 of the object returned by sub_6E9030,
// but only when the field at +0x1C is non-null. Otherwise returns 0.
static char __fastcall C_00861e98_GetLinkedFlagBit0x14(C_00861e98* self, void* /*edx*/, int a2)
{
    char result = 0;
    if (CSF_AT(void*, self, 0x1C))
    {
        int linked = sub_6E9030(self, nullptr, a2);
        result = static_cast<char>(*(reinterpret_cast<uint8_t*>(linked) + 0x14) & 1);
    }
    return result;
}

HOOK(0x006E9710, C_00861e98_GetLinkedFlagBit0x14, "auto");
