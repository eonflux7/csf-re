#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x00528830: reads the flag byte at +4 and, when it is set, clears the byte at +5.
// Returns the value read at +4 (upper bits of eax are left as the original does).
static char __fastcall TakeFlagAndClearPending(uint8_t* self, void* /*edx*/)
{
    const char flag = CSF_AT(char, self, 4);
    if (flag)
        CSF_AT(uint8_t, self, 5) = 0;
    return flag;
}

HOOK(0x528830, TakeFlagAndClearPending, "auto");
