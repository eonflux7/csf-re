#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiListBox_SetIndexedRecordValue(
    CGuiListBox* self, void* /*edx*/, int recordIndex, int value)
{
    std::uint32_t remaining = static_cast<std::uint32_t>(
        (reinterpret_cast<int(__fastcall*)(CGuiListBox*, void*)>(
            CSF_AT(void**, self, 0)[0x274 / 4]))(self, nullptr));

    if (remaining != 0)
    {
        const std::uint32_t recordOffset =
            static_cast<std::uint32_t>(recordIndex) * 0x3Cu;
        std::uint32_t entryOffset = 0;

        do
        {
            void* entries = reinterpret_cast<void*>(self->field_0220);
            const std::uint32_t records =
                CSF_AT(std::uint32_t, entries, entryOffset + 4u);
            entryOffset += 0x10u;
            --remaining;
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(records), recordOffset) =
                static_cast<std::uint32_t>(value);
        }
        while (remaining != 0);
    }

    return 0;
}

HOOK(0x007231B0, CGuiListBox_SetIndexedRecordValue, "auto");
