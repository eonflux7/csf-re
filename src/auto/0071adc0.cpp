#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00864508_PrepareChildProcessing, 0x0071A940,
         (C_00864508* self, void* edx));

// Walks the child list back-to-front; for each child not present in the
// excluded list, forwards `argument` to the child's associated object and to
// its embedded secondary object, then stops as soon as a child signals done.
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

        const std::int32_t excludedCount = excludedBegin
            ? static_cast<std::int32_t>(
                  self->field_0038 - excludedBegin) >> 3
            : 0;

        bool excluded = false;
        if (excludedCount != 0)
        {
            std::uint32_t position = 0;
            auto* entry =
                reinterpret_cast<std::uint8_t*>(self->field_0034);
            while (CSF_AT(std::int32_t, entry, 0) != index)
            {
                ++position;
                entry += 8;
                if (position >= static_cast<std::uint32_t>(excludedCount))
                    break;
            }
            if (position < static_cast<std::uint32_t>(excludedCount))
                excluded = true;
        }

        if (!excluded)
        {
            void* associatedObject =
                ((void*(__fastcall*)(void*, void*))
                     CSF_AT(void**, child, 0)[8])(child, nullptr);

            ((void(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, associatedObject, 0)[13])(
                associatedObject, nullptr, argument);

            void* secondaryBase =
                reinterpret_cast<std::uint8_t*>(child) + 4;
            ((void(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, secondaryBase, 0)[13])(
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

HOOK(0x0071ADC0, C_00864508_ProcessChildrenInReverse, "auto");
