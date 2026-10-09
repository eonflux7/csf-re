#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees every heap-allocated entry of a pointer vector stored at +4 (begin), +8 (end), +0xC (capacity),
// then frees the vector buffer itself and resets the three pointers.
static int InputBindings_ClearBindingList(void* self, void* /*edx*/)
{
    uint32_t count;
    if (CSF_AT(uint32_t, self, 4) != 0)
        count = (uint32_t)((CSF_AT(int, self, 8) - CSF_AT(int, self, 4)) >> 2);
    else
        count = 0;

    for (uint32_t i = 0; i < count; i++)
    {
        uint32_t* begin = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 4));
        uint32_t entry = begin[i];
        if (entry != 0)
        {
            if (CSF_AT(uint32_t, reinterpret_cast<void*>(entry), 0x10) != 0)
            {
                Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(entry), 0x10)));
                CSF_AT(uint32_t, reinterpret_cast<void*>(entry), 0x10) = 0;
            }
            Mem_Free(reinterpret_cast<void*>(entry));
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 4))[i] = 0;
        }
    }

    int result = CSF_AT(int, self, 4);
    if (result != 0)
        result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 4)));
    CSF_AT(uint32_t, self, 4) = 0;
    CSF_AT(uint32_t, self, 8) = 0;
    CSF_AT(uint32_t, self, 0xC) = 0;
    return result;
}

HOOK(0x00769A60, InputBindings_ClearBindingList, "auto");
