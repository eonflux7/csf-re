#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"
#include "globals.h"

CSF_FUNC(int*, __fastcall, EraseWideStringRange, 0x004F21D0,
         (legacy::wstring* text, void* edx, int* result,
          std::uint32_t first, std::uint32_t last));

static int __fastcall CGuiEdit_ClearTextAndPositions(
    CGuiEdit* self, void* /*edx*/)
{
    int iteratorResult;

    {
        auto* text = &self->m_wtext0;
        const std::uint32_t capacity =
            CSF_AT(std::uint32_t, text, 0x18);
        const std::uint32_t inlineAddress =
            reinterpret_cast<std::uint32_t>(
                reinterpret_cast<std::uint8_t*>(text) + 4);

        const std::uint32_t endBase = capacity < 8
            ? inlineAddress
            : CSF_AT(std::uint32_t, text, 0x04);
        const std::uint32_t end =
            endBase + 2u * CSF_AT(std::uint32_t, text, 0x14);
        const std::uint32_t begin = capacity < 8
            ? inlineAddress
            : CSF_AT(std::uint32_t, text, 0x04);

        EraseWideStringRange(text, nullptr, &iteratorResult, begin, end);
    }

    {
        auto* text = &self->m_wtext1;
        const std::uint32_t capacity =
            CSF_AT(std::uint32_t, text, 0x18);
        const std::uint32_t inlineAddress =
            reinterpret_cast<std::uint32_t>(
                reinterpret_cast<std::uint8_t*>(text) + 4);

        const std::uint32_t endBase = capacity < 8
            ? inlineAddress
            : CSF_AT(std::uint32_t, text, 0x04);
        const std::uint32_t end =
            endBase + 2u * CSF_AT(std::uint32_t, text, 0x14);
        const std::uint32_t begin = capacity < 8
            ? inlineAddress
            : CSF_AT(std::uint32_t, text, 0x04);

        EraseWideStringRange(text, nullptr, &iteratorResult, begin, end);
    }

    self->field_01a8 = 0;
    self->field_01ac = 0;
    self->field_01b0 = 0;
    return 0;
}

HOOK(0x007213C0, CGuiEdit_ClearTextAndPositions, "auto");
