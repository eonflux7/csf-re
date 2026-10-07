#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiListBoxBase_LayoutColumnHeaders(
    CGuiListBoxBase* self, void* /*edx*/)
{
    uint32_t result = self->m_columnsBegin;
    if (!result)
        return 0;

    result = static_cast<uint32_t>(
        static_cast<int32_t>(self->m_columnsEnd - result) / 12);
    if (!result)
        return 0;

    uint32_t origin[2];
    ((void(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, self, 0)[0x160 / 4])(self, nullptr, origin);

    uint32_t headerY = origin[1];
    const uint32_t verticalOffset = self->field_0174;
    result = self->m_columnsBegin;
    uint32_t headerX = origin[0];
    headerY += verticalOffset;
    origin[1] = headerY;

    uint32_t headerPosition[2] = {headerX, headerY};
    uint32_t columnCount;
    if (result)
    {
        result = static_cast<uint32_t>(
            static_cast<int32_t>(self->m_columnsEnd - result) / 12);
        columnCount = result;
    }
    else
    {
        columnCount = 0;
    }

    uint32_t headerSize[2];
    if (columnCount)
    {
        result = self->m_columnsBegin;
        uint32_t columnOffset = 0;
        uint32_t remaining = columnCount;
        do
        {
            void* header = reinterpret_cast<void*>(
                CSF_AT(uint32_t,
                    reinterpret_cast<void*>(result + columnOffset), 8));

            if (header)
            {
                ((void(__fastcall*)(void*, void*, uint32_t*))
                    CSF_AT(void**, header, 0)[0x1C / 4])(
                        header, nullptr, headerPosition);

                const uint32_t columns = self->m_columnsBegin;
                header = reinterpret_cast<void*>(
                    CSF_AT(uint32_t,
                        reinterpret_cast<void*>(columns + columnOffset), 8));
                const uint32_t width = CSF_AT(uint32_t,
                    reinterpret_cast<void*>(columns + columnOffset), 0);
                headerSize[1] = self->field_0178;
                headerSize[0] = width;

                ((void(__fastcall*)(void*, void*, uint32_t*))
                    CSF_AT(void**, header, 0)[0x24 / 4])(
                        header, nullptr, headerSize);

                header = reinterpret_cast<void*>(
                    CSF_AT(uint32_t,
                        reinterpret_cast<void*>(
                            self->m_columnsBegin + columnOffset), 8));
                ((int(__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, self, 0)[0x238 / 4])(
                        self, nullptr, header);

                result = self->m_columnsBegin;
                headerX = headerPosition[0];
            }

            headerX += CSF_AT(uint32_t,
                reinterpret_cast<void*>(result + columnOffset), 0);
            columnOffset += 12;
            headerPosition[0] = headerX;
            --remaining;
        }
        while (remaining);

        headerY = origin[1];
    }

    headerX = origin[0];
    const uint32_t textHeight = self->field_0178;
    headerPosition[1] = headerY;
    headerPosition[0] = headerX;
    uint32_t textSize[2] = {0, textHeight};
    uint32_t textPosition[2];
    uint32_t columnIndex = 0;
    uint32_t columnOffset = 0;

    if (columnCount)
    {
        do
        {
            const uint32_t columns = self->m_columnsBegin;
            void* text = reinterpret_cast<void*>(
                CSF_AT(uint32_t,
                    reinterpret_cast<void*>(columns + columnOffset), 4));
            uint32_t entryAddress = columns + columnOffset;

            if (text)
            {
                textSize[0] = CSF_AT(uint32_t,
                    reinterpret_cast<void*>(entryAddress), 0);
                text = reinterpret_cast<void*>(
                    CSF_AT(uint32_t,
                        reinterpret_cast<void*>(entryAddress), 4));

                bool clipped = false;
                uint32_t inset = 0;
                if (!((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, text, 0)[0x70 / 4])(text, nullptr))
                {
                    inset = 2;
                    textSize[0] -= 2;
                }

                uint32_t* controlSize =
                    ((uint32_t*(__fastcall*)(void*, void*))
                        CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
                const uint32_t originX = origin[0];
                const uint32_t rightEdge = controlSize[0] + originX;
                const uint32_t textRight = textSize[0] + headerPosition[0];

                if (static_cast<int32_t>(textRight) >
                    static_cast<int32_t>(rightEdge))
                {
                    textSize[0] = rightEdge - headerPosition[0];
                    clipped = true;
                }

                text = reinterpret_cast<void*>(
                    CSF_AT(uint32_t,
                        reinterpret_cast<void*>(
                            self->m_columnsBegin + columnOffset), 4));
                textPosition[1] = headerPosition[1];
                textPosition[0] = headerPosition[0] + inset;

                ((void(__fastcall*)(void*, void*, uint32_t*))
                    CSF_AT(void**, text, 0)[0x50 / 4])(
                        text, nullptr, textPosition);

                text = reinterpret_cast<void*>(
                    CSF_AT(uint32_t,
                        reinterpret_cast<void*>(
                            self->m_columnsBegin + columnOffset), 4));
                ((void(__fastcall*)(void*, void*, uint32_t*))
                    CSF_AT(void**, text, 0)[0x58 / 4])(
                        text, nullptr, textSize);

                text = reinterpret_cast<void*>(
                    CSF_AT(uint32_t,
                        reinterpret_cast<void*>(
                            self->m_columnsBegin + columnOffset), 4));
                result = static_cast<uint32_t>(
                    ((int(__fastcall*)(void*, void*, void*))
                        CSF_AT(void**, self, 0)[0x238 / 4])(
                            self, nullptr, text));

                if (clipped)
                    return static_cast<int32_t>(result);

                entryAddress = self->m_columnsBegin + columnOffset;
                headerX = headerPosition[0];
            }

            const uint32_t width = CSF_AT(uint32_t,
                reinterpret_cast<void*>(entryAddress), 0);
            headerX += width;
            result = columnIndex + 1;
            columnOffset += 12;
            headerPosition[0] = headerX;
            ++columnIndex;
        }
        while (result < columnCount);
    }

    return static_cast<int32_t>(result);
}

HOOK(0x00714E80, CGuiListBoxBase_LayoutColumnHeaders, "auto");
