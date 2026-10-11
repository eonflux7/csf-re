#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original game heap free (module core/memory). Callee: __cdecl, one argument.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Tears down the three pointer-array groups of the scene loader (groups of 16 bytes starting at +0x24).
// Each group: +0x00 array pointer, +0x04 unused (cleared), +0x08 element count (cleared).
// Every non-null element gets its virtual slot 1 called with arg 1, then is nulled; the array is freed.
static int __fastcall C_00860bb8_EndRenderReset(C_00860bb8* self, void* /*edx*/)
{
    uint32_t groupOffset = 0x24;
    int groupsLeft = 3;
    int result = 0;

    do
    {
        // Element count is read once before the loop, as in the original.
        uint32_t elementCount = CSF_AT(uint32_t, self, groupOffset + 8);

        for (uint32_t i = 0; i < elementCount; ++i)
        {
            // The array pointer is re-read on every iteration.
            uint32_t element = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, groupOffset))[i];
            if (element != 0)
            {
                void* object = reinterpret_cast<void*>(element);
                // vtbl slot +4 (index 1), __thiscall with one argument (1).
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
                reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, groupOffset))[i] = 0;
            }
        }

        uint32_t arrayBlock = CSF_AT(uint32_t, self, groupOffset);
        CSF_AT(uint32_t, self, groupOffset + 4) = 0;
        CSF_AT(uint32_t, self, groupOffset + 8) = 0;
        if (arrayBlock != 0)
        {
            Mem_Free(reinterpret_cast<void*>(arrayBlock));
            CSF_AT(uint32_t, self, groupOffset) = 0;
        }

        groupOffset += 0x10;
        result = --groupsLeft;
    } while (groupsLeft != 0);

    return result;
}

HOOK(0x006CBC10, C_00860bb8_EndRenderReset, "auto");
