#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiListBox_GetColumnCount, 0x00714830,
         (CGuiListBox* self, void* edx));

static int __fastcall CGuiListBox_SetRowColor(
    CGuiListBox* self, void* /*edx*/, int rowIndex,
    const uint32_t* color)
{
    uint32_t remaining =
        static_cast<uint32_t>(CGuiListBox_GetColumnCount(self, nullptr));

    if (remaining != 0) {
        const uint32_t rowOffset = static_cast<uint32_t>(rowIndex) << 4;
        uint32_t cellOffset = 0;

        do {
            auto* rows = reinterpret_cast<uint8_t*>(self->field_0220);
            auto* cells = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, rows, rowOffset + 4));
            auto* destination = cells + cellOffset + 8;

            CSF_AT(uint32_t, destination, 0) = color[0];
            CSF_AT(uint32_t, destination, 4) = color[1];
            CSF_AT(uint32_t, destination, 8) = color[2];

            const uint32_t fourthComponent = color[3];
            cellOffset += 60;
            --remaining;
            CSF_AT(uint32_t, destination, 12) = fourthComponent;
        } while (remaining != 0);
    }

    return static_cast<int>(remaining);
}

HOOK(0x00723150, CGuiListBox_SetRowColor, "auto");
