#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Growth routine for the S_0041e5f0 index array (ret 4, __thiscall).
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// The original's 0x49F300 site references S_0041e5f0's vtable address.
[[maybe_unused]] static void* const s_GrowArrayFrameListVtable = CSF_VTBL(0x0041E5F0);

// Scans the owner's record table (stride 0x1C, base at +0x198, count at +0x1A0)
// and appends the index of every record whose tag (+0x0C) equals matchValue
// into the frame list. Returns the record count as last read from the owner.
static unsigned int __fastcall AiMountTasks_CollectEntryFrames(void* self, void* /*edx*/,
                                                               int matchValue,
                                                               S_0041e5f0* frameList)
{
    uint32_t result = CSF_AT(uint32_t, self, 0x1A0);
    uint32_t recordIndex = 0;

    if (result != 0) {
        int recordOffset = 0;
        do {
            uint8_t* records = reinterpret_cast<uint8_t*>(
                static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x198)));

            if (CSF_AT(int, records, recordOffset + 12) == matchValue) {
                if (frameList->field_0008 >= frameList->field_0004) {
                    S_0041e5f0_GrowArrayCapacity(frameList, nullptr,
                                                 static_cast<int>(frameList->field_000c));
                }
                uint32_t slot = frameList->field_0008;
                uint32_t* indices = reinterpret_cast<uint32_t*>(
                    static_cast<uintptr_t>(frameList->field_0000));
                indices[slot] = recordIndex;
                frameList->field_0008 = slot + 1;
            }

            result = CSF_AT(uint32_t, self, 0x1A0);
            ++recordIndex;
            recordOffset += 0x1C;
        } while (recordIndex < result);
    }

    return result;
}

HOOK(0x0049F2C0, AiMountTasks_CollectEntryFrames, "auto");
