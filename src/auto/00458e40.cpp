#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Clears the HUD status-icon list: stamps every 40-byte icon entry with the
// icon vtable (C_0082d410), frees the entry array, and zeroes the three
// begin/end/capacity pointers.
static int __fastcall HudStatusIcons_ClearIconList(int* self, void* /*edx*/)
{
    int result = 0;

    if (CSF_AT(uint32_t, self, 0x04) != 0)
    {
        uintptr_t endAddress = CSF_AT(uint32_t, self, 0x08);
        for (uintptr_t cursor = CSF_AT(uint32_t, self, 0x04); cursor != endAddress; cursor += 40)
        {
            CSF_AT(void*, reinterpret_cast<void*>(cursor), 0x1C) = CSF_VTBL(0x0082D410);
        }

        if (CSF_AT(uint32_t, self, 0x04) != 0)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x04))));
        }
    }

    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    return result;
}

HOOK(0x00458E40, HudStatusIcons_ClearIconList, "auto");
