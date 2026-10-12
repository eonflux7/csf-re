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

// Bound by address: the original calls the CRT memcpy at 0x7AEE60.
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the first dword equal to `value` from a dword array.
// List layout: [0x0] = data pointer, [0x8] = element count.
// The element count is stored back as count - 1 and the tail is shifted down by one slot.
static void __cdecl GfxLights_RemoveFirstEntry(uint32_t* list, uint32_t value)
{
    uint32_t count = CSF_AT(uint32_t, list, 0x8);
    uint32_t index = 0;

    if (count == 0)
        return;

    uint32_t* data = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, list, 0x0));
    uint32_t* cursor = data;

    while (*cursor != value)
    {
        if (++index >= CSF_AT(uint32_t, list, 0x8))
            return;
        ++cursor;
    }

    if (index != static_cast<uint32_t>(-1))
    {
        CSF_AT(uint32_t, list, 0x8) = count - 1;
        if (index != count - 1)
        {
            _memcpy(&data[index], &data[index + 1], 4 * (count - 1 - index));
        }
    }
}

HOOK(0x6D1670, GfxLights_RemoveFirstEntry, "auto");
