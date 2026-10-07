#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"
#include "types/auto_classes.h"

static int __fastcall CGuiRadioGroup_FindSelectedChild(
    CGuiRadioGroup* self, void* /*edx*/)
{
    const std::uint32_t childrenBegin = self->field_0138;
    std::uint32_t childCount = 0;
    if (childrenBegin != 0) {
        childCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_013c - childrenBegin) >> 2);
    }

    int selectedChild = 0;
    std::uint32_t index = 0;
    while (index < childCount) {
        if (selectedChild != 0)
            break;

        void* child =
            reinterpret_cast<void**>(self->field_0138)[index];

        const auto isSelected =
            reinterpret_cast<std::uint8_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0x298 / 4]);

        if (isSelected(child, nullptr) != 0)
            selectedChild = reinterpret_cast<int>(child);

        ++index;
    }

    return selectedChild;
}

HOOK(0x0072E900, CGuiRadioGroup_FindSelectedChild, "auto");
