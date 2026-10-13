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

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the first occurrence of `targetId` from the uint32 array at +0x58 (count at +0x60),
// shifting the remaining entries down by one. Does nothing if the value is not present.
static void __fastcall C_00862e88_RemoveIdFromList(C_00862e88* self, void* /*edx*/, int targetId)
{
    uint32_t count = self->field_0060;
    uint32_t index = 0;

    if (count != 0)
    {
        uint32_t* cursor = reinterpret_cast<uint32_t*>(self->field_0058);

        while (*cursor != static_cast<uint32_t>(targetId))
        {
            ++index;
            if (index >= count)
                return;
            ++cursor;
        }

        if (index != 0xFFFFFFFFu)
        {
            uint32_t newCount = self->field_0060 - 1;
            self->field_0060 = newCount;

            if (index != newCount)
            {
                uint8_t* base = reinterpret_cast<uint8_t*>(self->field_0058);
                _memcpy(base + 4 * index,
                        base + 4 * index + 4,
                        static_cast<std::size_t>(4 * (newCount - index)));
            }
        }
    }
}

HOOK(0x006FC580, C_00862e88_RemoveIdFromList, "auto");
