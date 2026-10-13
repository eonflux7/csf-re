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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases each entry's sub-object (virtual slot 1 with arg 1), clears the
// entry pointer, then frees the entry array and resets the counters.
static char __fastcall GfxEnvironment_ClearAndFreeEntries(S_006bc310* self, void* /*edx*/)
{
    uint32_t entryIndex = 0;
    if (self->field_009c)
    {
        uint32_t entryOffset = 0;
        do
        {
            // The original reloads the array base and count on every iteration.
            uint32_t subObjectValue = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0094), entryOffset + 16);
            if (subObjectValue)
            {
                void* subObject = reinterpret_cast<void*>(subObjectValue);
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subObject, 0)[1])(subObject, nullptr, 1);
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0094), entryOffset + 16) = 0;
            }
            ++entryIndex;
            entryOffset += 20;
        } while (entryIndex < self->field_009c);
    }

    uint32_t entryArray = self->field_0094;
    self->field_0098 = 0;
    self->field_009c = 0;
    uint32_t result = entryArray;
    if (entryArray)
    {
        result = static_cast<uint32_t>(Mem_Free(reinterpret_cast<void*>(entryArray)));
        self->field_0094 = 0;
    }
    self->field_00a4 = 0;
    return static_cast<char>(result);
}

HOOK(0x6BCD10, GfxEnvironment_ClearAndFreeEntries, "auto");
