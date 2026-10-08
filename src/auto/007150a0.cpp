#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiListBoxBase_DrawVisibleCells(
    CGuiListBoxBase* self, void* /*edx*/)
{
    uint32_t result = self->m_columnsBegin;
    if (result == 0)
        return 0;

    result = static_cast<uint32_t>(
        static_cast<int32_t>(self->m_columnsEnd - result) / 12);
    if (result == 0)
        return 0;

    result = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x264 / 4])(self, nullptr);
    if (static_cast<uint8_t>(result) == 0)
        return static_cast<int32_t>(result);

    result = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x268 / 4])(self, nullptr);
    if (static_cast<uint8_t>(result) == 0)
        return static_cast<int32_t>(result);

    uint32_t origin[2];
    ((void(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, self, 0)[0x160 / 4])(self, nullptr, origin);

    origin[1] += self->field_0178 + self->field_0174;
    uint32_t position[2] = {origin[0], origin[1]};

    uint32_t* controlSize = ((uint32_t*(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
    const uint32_t availableHeight =
        controlSize[1] - self->field_0174 - self->field_0178;

    result = ((uint32_t(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x274 / 4])(self, nullptr);
    const uint32_t rowCount = result;
    uint32_t rowIndex = self->field_0168;

    if (rowIndex >= rowCount)
        return static_cast<int32_t>(result);

    result = position[1];
    do
    {
        uint32_t extent[2];
        extent[0] = 0;
        extent[1] = self->m_rowHeight + 2u * self->m_lineSpacing;

        if (rowIndex > self->field_0168)
        {
            result += extent[1];
            if (static_cast<int32_t>(result) >
                static_cast<int32_t>(availableHeight + origin[1]))
            {
                break;
            }
        }

        position[0] = origin[0];
        const uint32_t columnCount =
            ((uint32_t(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, self, 0)[0x278 / 4])(
                    self, nullptr, rowIndex);

        uint32_t columnIndex = 0;
        if (columnCount != 0)
        {
            uint32_t columnOffset = 0;
            do
            {
                extent[0] = CSF_AT(
                    uint32_t,
                    reinterpret_cast<void*>(self->m_columnsBegin),
                    columnOffset);

                // This virtual call has no stack arguments.
                const uint32_t cell =
                    ((uint32_t(__fastcall*)(void*, void*))
                        CSF_AT(void**, self, 0)[0x27C / 4])(
                            self, nullptr);

                controlSize = ((uint32_t*(__fastcall*)(void*, void*))
                    CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
                const uint32_t rightEdge = origin[0] + controlSize[0];

                bool clipped = false;
                if (static_cast<int32_t>(position[0] + extent[0]) >
                    static_cast<int32_t>(rightEdge))
                {
                    extent[0] = rightEdge - position[0];
                    clipped = true;
                }

                ((void(__fastcall*)(
                    void*, void*, uint32_t*, uint32_t*,
                    uint32_t, uint32_t, uint32_t))
                    CSF_AT(void**, self, 0)[0x280 / 4])(
                        self, nullptr, position, extent,
                        cell, rowIndex, columnIndex);

                if (clipped)
                    break;

                // The callback can modify both pairs.
                position[0] += extent[0];
                ++columnIndex;
                columnOffset += 12;
            }
            while (columnIndex < columnCount);
        }

        result = position[1] + extent[1];
        position[1] = result;
        ++rowIndex;
    }
    while (rowIndex < rowCount);

    return static_cast<int32_t>(result);
}

HOOK(0x007150A0, CGuiListBoxBase_DrawVisibleCells, "auto");
