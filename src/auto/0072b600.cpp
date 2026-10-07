#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_CheckProcessingEligibility, 0x00718700,
         (CGuiControl* self, void* edx));

static int __fastcall CGuiPanel_ForwardToChildrenIfEligible(
    CGuiPanel* self, void* /*edx*/, int argument)
{
    int result =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);

    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    result = CGuiControl_CheckProcessingEligibility(self, nullptr);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    const std::uint32_t childrenBegin = self->field_0138;
    result = static_cast<std::int32_t>(childrenBegin);

    std::uint32_t childCount = 0;
    if (childrenBegin != 0) {
        const std::uint32_t byteDifference =
            self->field_013c - childrenBegin;
        childCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteDifference) >> 2);
    }

    for (std::uint32_t index = 0; index < childCount; ++index) {
        void* child =
            reinterpret_cast<void**>(self->field_0138)[index];
        result =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, child, 0)[0x10 / 4])(
                    child, nullptr, argument);
    }

    return result;
}

HOOK(0x0072B600, CGuiPanel_ForwardToChildrenIfEligible, "auto");
