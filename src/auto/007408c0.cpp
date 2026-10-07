#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// If the byte flag at +0x18 is set, return 0; otherwise return the dword stored at +0x8.
static int __fastcall GetValueIfNotFlagged(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x18))
        return 0;
    return static_cast<int>(CSF_AT(uint32_t, self, 0x8));
}

HOOK(0x007408C0, GetValueIfNotFlagged, "auto");
