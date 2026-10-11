#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall FindRangeAssociatedObject(void* self, void* /*edx*/,
                                               unsigned int position)
{
    const uint32_t rangeCount = CSF_AT(uint32_t, self, 0xE4);
    if (rangeCount != 0)
    {
        uint32_t* range = CSF_AT(uint32_t*, self, 0xDC);
        uint32_t index = 0;
        for (;;)
        {
            const uint32_t start = range[0];
            if (position >= start && position < start + range[1])
            {
                return reinterpret_cast<int>(
                    CSF_AT(uint8_t*, self, 0x08) + 0x24);
            }

            if (position < start + range[1])
                break;

            ++index;
            range += 2;
            if (index >= rangeCount)
                break;
        }
    }

    return reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0xA8);
}

HOOK(0x00733720, FindRangeAssociatedObject, "auto");
