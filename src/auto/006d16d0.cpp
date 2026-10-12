#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the first occurrence of `target` from the pointer array at +0x74 (count at +0x7C),
// shifting the remaining entries down by one. Does nothing if the value is not present.
static void __fastcall GfxLights_RemovePointerFromList(void* self, void* /*edx*/, int target)
{
    uint32_t count = CSF_AT(uint32_t, self, 0x7C);
    uint32_t index = 0;

    if (count != 0)
    {
        uint32_t* items = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x74));
        uint32_t* cursor = items;

        while (*cursor != static_cast<uint32_t>(target))
        {
            ++index;
            if (index >= count)
                return;
            ++cursor;
        }

        if (index != 0xFFFFFFFFu)
        {
            uint32_t newCount = CSF_AT(uint32_t, self, 0x7C) - 1;
            CSF_AT(uint32_t, self, 0x7C) = newCount;

            if (index != newCount)
            {
                uint8_t* base = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x74));
                _memcpy(base + 4 * index,
                        base + 4 * index + 4,
                        static_cast<std::size_t>(4 * (newCount - index)));
            }
        }
    }
}

HOOK(0x006D16D0, GfxLights_RemovePointerFromList, "auto");
