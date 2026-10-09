#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_45AA50, 0x0045AA50, (void* self, void* edx));

[[maybe_unused]] static const auto s_ButtonArrayVtbl = CSF_VTBL(0x0041E5F0);

// Walks the owner's linked-object pointer table (count at +0x30, array at +0x28).
// Every non-null entry whose sub_45AA50 result equals filterValue is appended to the
// output list a3. Returns 1 if the output list is non-empty afterwards, else 0.
static int __fastcall MissionScript_CollectLinkedObjectPointers(void* self, void* /*edx*/, int filterValue, S_0041e5f0* outList)
{
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x30);

    for (uint32_t i = 0; i < entryCount; ++i)
    {
        const uint32_t entryOffset = i * 4;
        uint8_t* arrayBase = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x28));
        uint32_t entryValue = *reinterpret_cast<uint32_t*>(arrayBase + entryOffset);

        if (entryValue != 0 && sub_45AA50(reinterpret_cast<void*>(entryValue), nullptr) == filterValue)
        {
            uint32_t* sourceSlot = reinterpret_cast<uint32_t*>(arrayBase + entryOffset);

            if (outList->field_0008 >= outList->field_0004)
                GrowButtonArray(outList, nullptr, static_cast<int>(outList->field_000c));

            uint32_t writeIndex = outList->field_0008;
            uint32_t* destSlot = reinterpret_cast<uint32_t*>(outList->field_0000 + 4 * writeIndex);
            outList->field_0008 = writeIndex + 1;
            *destSlot = *sourceSlot;
        }
    }

    return outList->field_0008 != 0 ? 1 : 0;
}

HOOK(0x468A00, MissionScript_CollectLinkedObjectPointers, "auto");
