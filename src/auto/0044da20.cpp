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

CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);

// Walks the 12-byte entry array from the last entry back to the first and removes every
// entry whose float at +4 (plus g_MessageBoxAnimationScale) is below field_0004, shifting
// the tail down with memcpy and decrementing the count.
static void __fastcall S0044da20_RemoveEntriesBelowThreshold(S_0044da20* self, void* /*edx*/)
{
    int index = static_cast<int>(self->field_0014) - 1;
    if (index >= 0)
    {
        int byteOffset = 12 * index;
        do
        {
            uint8_t* entries = reinterpret_cast<uint8_t*>(self->field_000c);
            float entryY = *reinterpret_cast<float*>(entries + byteOffset + 4);
            if (entryY + g_MessageBoxAnimationScale < self->field_0004)
            {
                uint32_t newCount = self->field_0014 - 1;
                self->field_0014 = newCount;
                if (index != static_cast<int>(newCount))
                {
                    std::size_t copyBytes =
                        static_cast<std::size_t>(newCount - static_cast<uint32_t>(index)) * 12;
                    uint8_t* dest = reinterpret_cast<uint8_t*>(self->field_000c) + byteOffset;
                    uint8_t* src = reinterpret_cast<uint8_t*>(self->field_000c) + byteOffset + 12;
                    _memcpy(dest, src, copyBytes);
                }
            }
            --index;
            byteOffset -= 12;
        } while (index >= 0);
    }
}

HOOK(0x0044DA20, S0044da20_RemoveEntriesBelowThreshold, "auto");
