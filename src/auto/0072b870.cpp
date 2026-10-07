#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiPanel_ForwardToChildrenIfAllowed(
    CGuiPanel* self, void* /*edx*/, int argument)
{
    int result =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);

    if (static_cast<uint8_t>(result) != 0)
    {
        const uint32_t childrenBegin = self->field_0138;
        result = static_cast<int32_t>(childrenBegin);

        uint32_t childCount = 0;
        if (childrenBegin != 0)
        {
            const uint32_t byteDifference = self->field_013c - childrenBegin;
            childCount = static_cast<uint32_t>(
                static_cast<int32_t>(byteDifference) >> 2);
        }

        for (uint32_t index = 0; index < childCount; ++index)
        {
            void* child =
                reinterpret_cast<void**>(self->field_0138)[index];

            result =
                reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, child, 0)[0x40 / 4])(
                        child, nullptr, argument);
        }
    }

    return result;
}

HOOK(0x0072B870, CGuiPanel_ForwardToChildrenIfAllowed, "auto");
