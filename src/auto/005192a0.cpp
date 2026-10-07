#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// If the object's state/id (offset 0xC) equals 3, raise the bool flag at
// offset 0x25. Second argument is ignored (kept for the original ABI).
static void __fastcall SetFlagWhenStateEquals3(void* self, void* /*edx*/, int /*unused*/)
{
    if (CSF_AT(int, self, 0xC) == 3)
        CSF_AT(uint8_t, self, 0x25) = 1;
}

HOOK(0x005192A0, SetFlagWhenStateEquals3, "auto");
