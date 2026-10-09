#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));

// Searches the uint32 array embedded at this+0xB0 for `value`. Returns the index of the
// first match. If not found, appends `value` (growing the array first if full) and
// returns the new element count.
static unsigned int __fastcall CMissionScript_FindOrAppendValueIndex(void* self, void* /*edx*/, int value)
{
    (void)CSF_VTBL(0x0041E5F0);  // array helper's vtable is referenced by the original's call site

    uint8_t* arr = reinterpret_cast<uint8_t*>(self) + 0xB0;
    uint32_t wanted = static_cast<uint32_t>(value);

    uint32_t count = CSF_AT(uint32_t, arr, 0x8);
    if (count != 0)
    {
        uint32_t* data = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, arr, 0x0));
        for (uint32_t index = 0; index < count; ++index)
        {
            if (data[index] == wanted)
                return index;
        }
    }

    // Not found: grow if full, then append.
    if (CSF_AT(uint32_t, arr, 0x8) >= CSF_AT(uint32_t, arr, 0x4))
        GrowButtonArray(arr, nullptr, CSF_AT(int, arr, 0xC));

    uint32_t slot = CSF_AT(uint32_t, arr, 0x8);
    uint32_t* data = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, arr, 0x0));
    uint32_t newCount = slot + 1;
    CSF_AT(uint32_t, arr, 0x8) = newCount;
    data[slot] = wanted;
    return newCount;
}

HOOK(0x468A90, CMissionScript_FindOrAppendValueIndex, "auto");
