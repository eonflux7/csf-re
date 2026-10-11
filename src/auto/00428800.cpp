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

// Resets the 93-entry table embedded in a cheats menu object. Each 20-byte
// entry is { uint32 0 at +0x0C, float 1.0f at +0x10 } relative to the object,
// stepping by 0x14. Returns a pointer just past the last written entry.
static uint32_t* __fastcall CheatTable_ResetEntries(void* self, void* /*edx*/)
{
    constexpr int kEntryCount = 93;
    constexpr int kEntryStride = 0x14;

    uint8_t* cursor = static_cast<uint8_t*>(self) + 0x10;
    for (int remaining = kEntryCount; remaining != 0; --remaining)
    {
        *reinterpret_cast<uint32_t*>(cursor - 4) = 0;
        *reinterpret_cast<float*>(cursor) = 1.0f;
        cursor += kEntryStride;
    }
    return reinterpret_cast<uint32_t*>(cursor);
}

HOOK(0x00428800, CheatTable_ResetEntries, "auto");
