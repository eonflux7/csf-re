#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: C_0082d5e0 sub-object reset helper, bound by address (__fastcall, edx unused).
CSF_FUNC(int, __fastcall, sub_408850, 0x00408850, (void* self, void* edx));

// Clears the owner pointer, resets the helper at +0x0C, then releases every
// non-null entry of the slot array at +0x1C (calling its vtable slot 0 with 1)
// and zeroes that slot. Returns the final slot count.
static unsigned int __fastcall C_0082d5e0_ResetSlotsAndGetCount(C_0082d5e0* self, void* /*edx*/)
{
    self->field_0008 = 0;
    sub_408850(reinterpret_cast<uint8_t*>(self) + 0x0C, nullptr);

    uint32_t slotCount = self->field_0024;
    for (uint32_t i = 0; i < slotCount; ++i)
    {
        uint32_t* slotArray = reinterpret_cast<uint32_t*>(self->field_001c);
        void* entry = reinterpret_cast<void*>(slotArray[i]);
        if (entry)
        {
            using ReleaseFn = void(__fastcall*)(void* self, void* edx, int flag);
            ReleaseFn release = reinterpret_cast<ReleaseFn>(CSF_AT(void**, entry, 0)[0]);
            release(entry, nullptr, 1);

            reinterpret_cast<uint32_t*>(self->field_001c)[i] = 0;
        }
        slotCount = self->field_0024;
    }
    return slotCount;
}

HOOK(0x408A40, C_0082d5e0_ResetSlotsAndGetCount, "auto");
