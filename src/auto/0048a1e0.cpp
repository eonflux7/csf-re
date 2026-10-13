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

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Resets the object's entry table: calls slot 0 (destructor-style, flag 1) on each
// non-null entry, zeroes the entry slots, resets each element of the backing array,
// frees the array block and clears the pointers.
static unsigned int __fastcall C_00833708_ReleaseEntriesAndFreeArray(C_00833708* self, void* /*edx*/)
{
    unsigned int result = self->field_0014;
    unsigned int index = 0;

    self->vtbl = CSF_VTBL(0x00833708);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008336E8)));

    if (result)
    {
        uint32_t entryOffset = 0;
        do
        {
            uint8_t* slots = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_000c));
            void* entry = CSF_AT(void*, slots, entryOffset);
            if (entry)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, entry, 0)[0])(entry, nullptr, 1);
                slots = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_000c));
                CSF_AT(uint32_t, slots, entryOffset) = 0;
            }
            result = self->field_0014;
            ++index;
            entryOffset += 96;
        } while (index < result);
    }

    uint32_t arrayBase = self->field_000c;
    if (arrayBase)
    {
        int count = *reinterpret_cast<int*>(static_cast<uintptr_t>(arrayBase) - 4);
        uint32_t arrayEnd = arrayBase + 96u * static_cast<uint32_t>(count);
        int remaining = count - 1;
        if (remaining >= 0)
        {
            uint32_t* cursor = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(arrayEnd + 44));
            int loopCount = remaining + 1;
            do
            {
                cursor -= 24;
                --loopCount;
                cursor[4] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
                cursor[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
                cursor[-7] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
            } while (loopCount);
        }
        result = static_cast<unsigned int>(CoreMemory_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(arrayBase) - 4)));
        self->field_000c = 0;
    }

    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D410)));
    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0048A1E0, C_00833708_ReleaseEntriesAndFreeArray, "auto");
