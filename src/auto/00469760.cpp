#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl MissionScript_DestroyElementRange(int* first, int* last)
{
    int result = 0;
    for (int* element = first; element != last; element += 7)
    {
        if (CSF_AT(uint32_t, element, 0x18) >= 0x10)
        {
            result = CSF_AT(int, element, 0x04);
            if (result)
                result = Mem_FreeDebug(
                    reinterpret_cast<void*>(CSF_AT(int, element, 0x04)));
        }
        CSF_AT(uint32_t, element, 0x18) = 15;
        CSF_AT(int, element, 0x14) = 0;
        CSF_AT(uint8_t, element, 0x04) = 0;
    }
    return result;
}

HOOK(0x00469760, MissionScript_DestroyElementRange, "auto");
