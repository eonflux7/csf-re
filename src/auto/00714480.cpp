#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall CGuiListBoxBase_SnapHeightToRowIncrement(
    CGuiListBoxBase* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_01dc);
    if (result != 0)
    {
        const auto* size =
            ((std::uint32_t* (__fastcall*)(void*, void*))
                CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);

        const std::uint32_t height = size[1];
        const std::uint32_t inset178 = self->field_0178;
        const std::uint32_t inset174 = self->field_0174;
        const std::uint32_t rowHeight = self->m_rowHeight;
        const std::uint32_t lineSpacing = self->m_lineSpacing;

        const std::int32_t remainder =
            static_cast<std::int32_t>(height - inset178 - inset174) %
            static_cast<std::int32_t>(rowHeight + lineSpacing * 2u);

        size = ((std::uint32_t* (__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
        const std::uint32_t adjustedHeight =
            size[1] - static_cast<std::uint32_t>(remainder);

        size = ((std::uint32_t* (__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
        std::uint32_t adjustedSize[2] = {size[0], adjustedHeight};

        result = static_cast<char>(
            ((int (__fastcall*)(void*, void*, std::uint32_t*))
                CSF_AT(void**, self, 0)[0xA0 / 4])(
                    self, nullptr, adjustedSize));

        self->field_01dc = 0;
    }
    return result;
}

HOOK(0x00714480, CGuiListBoxBase_SnapHeightToRowIncrement, "auto");
