#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets a run of 124-byte string-display entries from a1 up to a2.
// Pointer esi tracks the entry's field block at entry+0x34, so every displacement
// below matches the original's [esi + disp] addressing exactly.
static int __cdecl HudStringShow_ProcessPair(int a1, int a2)
{
    int result = a2;
    int entry = a1;

    if (a1 != a2)
    {
        uint8_t* esi = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a1 + 0x34));
        do
        {
            if (CSF_AT(uint32_t, esi, 0) >= 8u && CSF_AT(uint32_t, esi, -0x14) != 0)
            {
                Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, esi, -0x14))));
            }
            CSF_AT(uint32_t, esi, 0) = 7;
            CSF_AT(uint32_t, esi, -0x4) = 0;
            CSF_AT(uint16_t, esi, -0x14) = 0;

            if (CSF_AT(uint32_t, esi, -0x1C) >= 0x10u)
            {
                if (CSF_AT(uint32_t, esi, -0x30) != 0)
                {
                    Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, esi, -0x30))));
                }
            }
            result = a2;
            CSF_AT(uint32_t, esi, -0x1C) = 15;
            CSF_AT(uint32_t, esi, -0x20) = 0;
            CSF_AT(uint8_t, esi, -0x30) = 0;

            entry += 0x7C;
            esi += 0x7C;
        } while (entry != a2);
    }
    return result;
}

HOOK(0x004F23A0, HudStringShow_ProcessPair, "auto");
