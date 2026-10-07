#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiListBox_GetCellTextColor, 0x00722EE0,
         (CGuiListBox* self, void* edx, int rowIndex, int columnIndex));

static int __fastcall CGuiListBox_DrawCellText(
    CGuiListBox* self, void* /*edx*/,
    const uint32_t* position, const uint32_t* size,
    int widthAdjustment, int rowIndex, int columnIndex)
{
    if (!self->m_pRowText)
        return 0;

    const uint32_t rowOffset = static_cast<uint32_t>(rowIndex) << 4;
    const uint32_t cellOffset = static_cast<uint32_t>(columnIndex) * 60u;

    void* rows = reinterpret_cast<void*>(self->field_0220);
    void* cells = reinterpret_cast<void*>(CSF_AT(uint32_t, rows, rowOffset + 4));
    const uint32_t textFormat = CSF_AT(uint32_t, cells, cellOffset);

    void* text = reinterpret_cast<void*>(self->m_pRowText);
    ((void(__fastcall*)(void*, void*, uint32_t))
        CSF_AT(void**, text, 0)[0x68 / 4])(text, nullptr, textFormat);

    const uint32_t positionY = position[1];
    const int horizontalOffset =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x27C / 4])(self, nullptr);

    uint32_t coordinates[2];
    coordinates[0] = position[0] + static_cast<uint32_t>(horizontalOffset);
    coordinates[1] = positionY;

    text = reinterpret_cast<void*>(self->m_pRowText);
    ((void(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, text, 0)[0x50 / 4])(text, nullptr, coordinates);

    coordinates[0] = size[0] - static_cast<uint32_t>(widthAdjustment);
    coordinates[1] = size[1];

    text = reinterpret_cast<void*>(self->m_pRowText);
    ((void(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, text, 0)[0x58 / 4])(text, nullptr, coordinates);

    void** colorSetterTable =
        CSF_AT(void**, reinterpret_cast<void*>(self->m_pRowText), 0);
    const int cellColor =
        CGuiListBox_GetCellTextColor(self, nullptr, rowIndex, columnIndex);

    text = reinterpret_cast<void*>(self->m_pRowText);
    ((int(__fastcall*)(void*, void*, int))
        colorSetterTable[0x60 / 4])(text, nullptr, cellColor);

    rows = reinterpret_cast<void*>(self->field_0220);
    cells = reinterpret_cast<void*>(CSF_AT(uint32_t, rows, rowOffset + 4));
    auto* cell = static_cast<uint8_t*>(cells) + cellOffset;

    const char* cellText;
    if (CSF_AT(uint32_t, cell, 0x30) < 8u)
        cellText = reinterpret_cast<const char*>(cell + 0x1C);
    else
        cellText = reinterpret_cast<const char*>(CSF_AT(uint32_t, cell, 0x1C));

    text = reinterpret_cast<void*>(self->m_pRowText);
    ((void(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, text, 0)[0x40 / 4])(text, nullptr, cellText);

    text = reinterpret_cast<void*>(self->m_pRowText);
    const void* currentColor =
        ((const void*(__fastcall*)(void*, void*))
            CSF_AT(void**, text, 0)[0x64 / 4])(text, nullptr);

    struct ColorSnapshot {
        uint32_t red;
        uint32_t green;
        uint32_t blue;
        float alpha;
    };

    ColorSnapshot originalColor;
    originalColor.red = CSF_AT(uint32_t, currentColor, 0);
    originalColor.green = CSF_AT(uint32_t, currentColor, 4);
    originalColor.blue = CSF_AT(uint32_t, currentColor, 8);
    originalColor.alpha = CSF_AT(float, currentColor, 12);

    ColorSnapshot fadedColor = originalColor;
    fadedColor.alpha = originalColor.alpha * self->field_022c;

    text = reinterpret_cast<void*>(self->m_pRowText);
    ((int(__fastcall*)(void*, void*, const void*))
        CSF_AT(void**, text, 0)[0x60 / 4])(text, nullptr, &fadedColor);

    text = reinterpret_cast<void*>(self->m_pRowText);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, self, 0)[0x238 / 4])(self, nullptr, text);

    text = reinterpret_cast<void*>(self->m_pRowText);
    return ((int(__fastcall*)(void*, void*, const void*))
        CSF_AT(void**, text, 0)[0x60 / 4])(text, nullptr, &originalColor);
}

HOOK(0x00723230, CGuiListBox_DrawCellText, "auto");
