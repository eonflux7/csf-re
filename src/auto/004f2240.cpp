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

static unsigned int __fastcall HudStringShow_DestroyElement(uint32_t* self, void* /*edx*/)
{
    unsigned int result = CSF_AT(uint32_t, self, 0x34);

    if (result >= 8)
    {
        result = CSF_AT(uint32_t, self, 0x20);
        if (result != 0)
        {
            result = static_cast<unsigned int>(
                Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result))));
        }
    }

    CSF_AT(uint32_t, self, 0x34) = 7;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint16_t, self, 0x20) = 0;

    if (CSF_AT(uint32_t, self, 0x18) >= 0x10)
    {
        result = CSF_AT(uint32_t, self, 0x04);
        if (result != 0)
        {
            result = static_cast<unsigned int>(
                Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result))));
        }
    }

    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0x0F;
    CSF_AT(uint8_t, self, 0x04) = 0;

    return result;
}

HOOK(0x004F2240, HudStringShow_DestroyElement, "auto");
