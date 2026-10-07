#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00864508_PrepareChildProcessing, 0x0071A940,
         (C_00864508* self, void* edx));

static char __fastcall C_00864508_ProcessChildrenInReverse(
    C_00864508* self, void* /*edx*/, int argument)
{
    const char preparationResult =
        C_00864508_PrepareChildProcessing(self, nullptr);
    if (!preparationResult)
        return preparationResult;

    const std::uint32_t childrenBegin = self->field_0048;
    const std::int32_t childCount = childrenBegin
        ? static_cast<std::int32_t>(self->field_004c - childrenBegin) >> 2
        : 0;

    for (std::int32_t index = childCount - 1; index >= 0; --index)
    {
        const std::uint32_t excludedBegin = self->field_0034;
        void* child = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0048),
            static_cast<std::uint32_t>(index) * 4);

        const std::uint32_t excludedCount = excludedBegin
            ? static_cast<std::uint32_t>(
                  static_cast<std::int32_t>(
                      self->field_0038 - excludedBegin) >> 3)
            : 0;

        bool excluded = false;
        if (excludedCount != 0)
        {
            auto* entry =
                reinterpret_cast<std::uint8_t*>(self->field_0034);
            std::uint32_t position = 0;
            while (position < excludedCount)
            {
                if (CSF_AT(std::int32_t, entry, 0) == index)
                {
                    excluded = true;
                    break;
                }
                ++position;
                entry += 8;
            }
        }

        if (!excluded)
        {
            void* associatedObject =
                ((void*(__fastcall*)(void*, void*))
                     CSF_AT(void**, child, 0)[8])(child, nullptr);

            ((void(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, associatedObject, 0)[11])(
                associatedObject, nullptr, argument);

            void* secondaryBase =
                reinterpret_cast<std::uint8_t*>(child) + 4;
            ((void(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, secondaryBase, 0)[11])(
                secondaryBase, nullptr, argument);
        }

        if (((std::uint8_t(__fastcall*)(void*, void*))
                 CSF_AT(void**, child, 0)[10])(child, nullptr))
        {
            break;
        }
    }

    return static_cast<char>(
        ((int(__fastcall*)(void*, void*))self->vtbl[42])(
            self, nullptr));
}

HOOK(0x0071AC60, C_00864508_ProcessChildrenInReverse, "auto");
