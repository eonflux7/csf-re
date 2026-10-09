#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_47A1E0, 0x0047A1E0, (void* self, void* edx, void* a2));

// Walks the object array at field_0078 (up to field_007c). For each element whose
// vtable slot 0x98 reports active, compares the element stored at the same slot
// index against this->vSlotDC() via sub_47A1E0 and counts the matches.
static int __fastcall CCharActionFsm_CountMatchingSlots(CCharActionFsm* self, void* /*edx*/)
{
    uint32_t index = 0;
    int matchCount = 0;

    while (true)
    {
        uint32_t slotBase = self->field_0078;
        uint32_t currentIndex = index;
        if (!slotBase ||
            index >= (uint32_t)(((int)(self->field_007c - slotBase)) >> 2))
            break;

        uint32_t byteOffset = index * 4;
        void* element = reinterpret_cast<void*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(slotBase), byteOffset));

        bool elementActive = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, element, 0)[0x98 / 4])(element, nullptr);

        if (elementActive)
        {
            uint32_t slotAddress = self->field_0078 + byteOffset;
            void* dcValue = reinterpret_cast<void*>(
                static_cast<uintptr_t>(self->vSlotDC()));
            void* slotObject = reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(slotAddress), 0));

            if (sub_47A1E0(slotObject, nullptr, dcValue))
                ++matchCount;
        }

        index = currentIndex + 1;
    }

    return matchCount;
}

HOOK(0x00485AC0, CCharActionFsm_CountMatchingSlots, "auto");
