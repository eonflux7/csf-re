#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Initializes a 36-byte-element array header: count, zero, extra value, then allocates
// count elements from the game heap and zeroes the used fields of each element.
static uint32_t* __fastcall HAnimChannelArray_Init(uint32_t* self, void* /*edx*/, int count, int extraValue)
{
    self[1] = static_cast<uint32_t>(count);
    self[2] = 0;
    self[3] = static_cast<uint32_t>(extraValue);

    if (count != 0)
    {
        void* block = Mem_Alloc(static_cast<std::size_t>(count) * 36);
        if (block != nullptr)
        {
            if (count - 1 >= 0)
            {
                uint8_t* element = static_cast<uint8_t*>(block);
                int remaining = count;
                do
                {
                    *reinterpret_cast<uint32_t*>(element + 0x00) = 0;
                    *reinterpret_cast<uint32_t*>(element + 0x04) = 0;
                    *reinterpret_cast<uint32_t*>(element + 0x14) = 0;
                    *reinterpret_cast<uint32_t*>(element + 0x18) = 0;
                    *reinterpret_cast<uint32_t*>(element + 0x1C) = 0;
                    *reinterpret_cast<uint32_t*>(element + 0x20) = 0;
                    element += 36;
                    --remaining;
                } while (remaining != 0);
            }
            self[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(block));
            return self;
        }
    }

    self[0] = 0;
    return self;
}

HOOK(0x006E8E30, HAnimChannelArray_Init, "auto");
