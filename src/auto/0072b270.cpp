#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiPanel_ForwardOperationToChildren(
    CGuiPanel* self, void* /*edx*/, int argument)
{
    int result =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);

    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    result =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0xC4 / 4])(self, nullptr);

    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    const std::uint32_t childrenBegin = self->field_0138;
    result = static_cast<std::int32_t>(childrenBegin);

    const std::uint32_t childCount = childrenBegin != 0
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  self->field_013c - childrenBegin) >> 2)
        : 0;

    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        void* child =
            reinterpret_cast<void**>(self->field_0138)[index];

        result =
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, child, 0)[0x2C / 4])(
                    child, nullptr, argument);
    }

    return result;
}

HOOK(0x0072B270, CGuiPanel_ForwardOperationToChildren, "auto");
