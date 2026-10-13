#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuMessageBox_ForwardToFlaggedChild, 0x0052A680, (void* self, void*));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660, (CGuiSlider* self, void*));

static int __fastcall CMenuMessageBox_LayoutButtons(
    CMenuMessageBox* self, void* /*edx*/, unsigned int width, int left)
{
    const uint32_t buttonCount = self->field_095c;
    if (!buttonCount)
        return 0;

    if (self->field_08a8)
        CMenuMessageBox_ForwardToFlaggedChild(self, nullptr);
    else
        CGuiSlider_ForwardSelfToParent(
            *reinterpret_cast<CGuiSlider**>(self->field_0954), nullptr);

    const int remainder = buttonCount & 3;
    const uint32_t rowCount = (buttonCount >> 2) + (remainder != 0);
    int column = 0;
    uint32_t row = 0;
    alignas(8) uint8_t positionBytes[0x40];

    for (uint32_t index = 0;; ++index)
    {
        const int columnsInRow =
            row == rowCount - 1 && remainder != 0 ? remainder : 4;

        int32_t rightIndex;
        int32_t leftIndex;
        uint32_t rightOffset;

        if (column == 0)
        {
            rightIndex = static_cast<int32_t>(index + 1);
            leftIndex = static_cast<int32_t>(columnsInRow + 4 * row - 1);
            rightOffset = 4 * (index + 1);
        }
        else
        {
            leftIndex = static_cast<int32_t>(index - 1);
            if (column == columnsInRow - 1)
            {
                rightIndex = static_cast<int32_t>(4 * row);
                rightOffset = 16 * row;
            }
            else
            {
                rightIndex = static_cast<int32_t>(index + 1);
                rightOffset = 4 * (index + 1);
            }
        }

        const uint32_t currentRow = index >> 2;
        const uint32_t previousRow = currentRow - 1;
        const uint32_t nextRow =
            static_cast<int32_t>(currentRow + 1) > static_cast<int32_t>(rowCount)
                ? 0 : currentRow + 1;

        int32_t upIndex = 0;
        int32_t downIndex = 0;
        if (currentRow != previousRow && currentRow != nextRow)
        {
            const int nextColumns =
                nextRow == rowCount && remainder != 0 ? remainder : 4;
            downIndex = static_cast<int32_t>(
                column / nextColumns + 4 * nextRow);

            const int previousColumns =
                previousRow == rowCount && remainder != 0 ? remainder : 4;
            upIndex = static_cast<int32_t>(
                column / previousColumns + 4 * previousRow);
        }

        if (rightIndex >= 0 &&
            rightIndex < static_cast<int32_t>(buttonCount))
        {
            auto* buttons = reinterpret_cast<uint8_t*>(self->field_0954);
            void* neighbor = CSF_AT(void*, buttons, rightOffset);
            void* button = CSF_AT(void*, buttons, 4 * index);
            void** buttonVtable = CSF_AT(void**, button, 0);
            const int neighborId =
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, neighbor, 0)[0x74 / 4])(neighbor, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                buttonVtable[0x7c / 4])(button, nullptr, neighborId);
        }

        if (leftIndex >= 0 &&
            leftIndex < static_cast<int32_t>(buttonCount))
        {
            auto* buttons = reinterpret_cast<uint8_t*>(self->field_0954);
            void* neighbor = CSF_AT(void*, buttons, 4 * leftIndex);
            void* button = CSF_AT(void*, buttons, 4 * index);
            void** buttonVtable = CSF_AT(void**, button, 0);
            const int neighborId =
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, neighbor, 0)[0x74 / 4])(neighbor, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                buttonVtable[0x78 / 4])(button, nullptr, neighborId);
        }

        if (upIndex >= 0 &&
            upIndex < static_cast<int32_t>(buttonCount))
        {
            auto* buttons = reinterpret_cast<uint8_t*>(self->field_0954);
            void* neighbor = CSF_AT(void*, buttons, 4 * upIndex);
            void* button = CSF_AT(void*, buttons, 4 * index);
            void** buttonVtable = CSF_AT(void**, button, 0);
            const int neighborId =
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, neighbor, 0)[0x74 / 4])(neighbor, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                buttonVtable[0x80 / 4])(button, nullptr, neighborId);
        }

        if (downIndex >= 0 &&
            downIndex < static_cast<int32_t>(buttonCount))
        {
            auto* buttons = reinterpret_cast<uint8_t*>(self->field_0954);
            void* neighbor = CSF_AT(void*, buttons, 4 * downIndex);
            void* button = CSF_AT(void*, buttons, 4 * index);
            void** buttonVtable = CSF_AT(void**, button, 0);
            const int neighborId =
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, neighbor, 0)[0x74 / 4])(neighbor, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                buttonVtable[0x84 / 4])(button, nullptr, neighborId);
        }

        void* button = CSF_AT(
            void*, reinterpret_cast<uint8_t*>(self->field_0954), 4 * index);
        CSF_AT(int32_t, positionBytes, 0) = static_cast<int32_t>(
            static_cast<uint32_t>(left) +
            (column + 1) * (width / (columnsInRow + 1)) -
            (static_cast<int32_t>(self->field_0968) >> 1));
        CSF_AT(int32_t, positionBytes, 4) = static_cast<int32_t>(
            self->field_0964 + row * self->field_096c);
        reinterpret_cast<void(__fastcall*)(void*, void*, int32_t*)>(
            CSF_AT(void**, button, 0)[0x98 / 4])(
                button, nullptr, reinterpret_cast<int32_t*>(positionBytes));

        if (++column == 4)
        {
            column = 0;
            ++row;
        }
        if (index + 1 >= buttonCount)
            return static_cast<int32_t>(index);
    }
}

HOOK(0x0052AAA0, CMenuMessageBox_LayoutButtons, "auto");
