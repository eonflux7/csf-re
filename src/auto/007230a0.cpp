#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiListBox_CopyRowAttributesToAllColumns(
    CGuiListBox* self, void* /*edx*/, int rowIndex,
    const std::uint32_t* attributes)
{
    std::uint32_t remaining = static_cast<std::uint32_t>(
        (reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x274 / 4]))(self, nullptr));

    if (remaining != 0) {
        const std::uint32_t rowOffset =
            static_cast<std::uint32_t>(rowIndex) * 0x3Cu;
        std::uint32_t columnOffset = 0;

        do {
            const std::uint32_t rowStorage = CSF_AT(
                std::uint32_t,
                reinterpret_cast<void*>(self->field_0220),
                columnOffset + 4u);
            auto* destination = reinterpret_cast<std::uint32_t*>(
                rowStorage + rowOffset + 8u);

            destination[0] = attributes[0];
            destination[1] = attributes[1];
            destination[2] = attributes[2];
            destination[3] = attributes[3];

            columnOffset += 0x10u;
            --remaining;
        } while (remaining != 0);
    }

    return 0;
}

HOOK(0x007230A0, CGuiListBox_CopyRowAttributesToAllColumns, "auto");
