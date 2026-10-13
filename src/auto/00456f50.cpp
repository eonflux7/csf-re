#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Growth routine of the S_0041e5f0 array (appends to a2's storage when full).
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// Appends to the output list every entry of the owner's entry array whose
// descriptor byte at +0x4C (reached via entry->+0x2C) equals the requested
// character code. Returns the entry count (the original loop's `i`).
static int __fastcall MenuMissionInfo_AppendCharacterEntries(void* self, void* /*edx*/, S_0041e5f0* a2, char a3)
{
    // The original references S_0041e5f0's vtable at the growth call site.
    (void)CSF_VTBL(0x0041E5F0);

    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x28);

    for (uint32_t i = 0; i < entryCount; ++i)
    {
        // Entry array is re-read on every iteration, as in the original.
        uint32_t* entrySlot = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x20)) + i;
        uint8_t* entry = reinterpret_cast<uint8_t*>(*entrySlot);
        uint8_t* descriptor = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, entry, 0x2C));
        uint8_t descriptorCode = CSF_AT(uint8_t, descriptor, 0x4C);

        if (static_cast<uint8_t>(a3) == descriptorCode)
        {
            if (a2->field_0008 >= a2->field_0004)
            {
                S_0041e5f0_GrowArrayCapacity(a2, nullptr, static_cast<int>(a2->field_000c));
            }

            uint32_t size = a2->field_0008;
            uint32_t* dst = reinterpret_cast<uint32_t*>(a2->field_0000 + 4 * size);
            a2->field_0008 = size + 1;
            *dst = *entrySlot;
        }
    }

    return static_cast<int>(entryCount);
}

HOOK(0x456F50, MenuMissionInfo_AppendCharacterEntries, "auto");
