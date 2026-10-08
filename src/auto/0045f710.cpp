#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies a [key, lo, hi] record reached through a double pointer into the object at
// offsets 0x60/0x64, sets the state at 0x18 to 4 and clears the byte at 0x82. Only does
// that when the record's key at +12 is non-negative; the return value is that key, or the
// record address + 4 when the copy is skipped.
static int __fastcall CopyNodeRangeIntoObject(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, void* a4)
{
    uint8_t* entry = *reinterpret_cast<uint8_t**>(a4);
    uint8_t* node = *reinterpret_cast<uint8_t**>(entry + 4);
    int32_t key = *reinterpret_cast<int32_t*>(node + 12);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(node + 4));

    if (key >= 0)
    {
        CSF_AT(uint32_t, self, 0x60) = *reinterpret_cast<uint32_t*>(node + 8);
        result = *reinterpret_cast<int32_t*>(node + 12);
        CSF_AT(int32_t, self, 0x64) = result;
        CSF_AT(int32_t, self, 0x18) = 4;
        CSF_AT(uint8_t, self, 0x82) = 0;
    }
    return result;
}

HOOK(0x45F710, CopyNodeRangeIntoObject, "auto");
